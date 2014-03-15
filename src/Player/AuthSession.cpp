#include "AuthSession.hpp"
#include "../Network/AuthServer.hpp"

void AuthSession::start()
{
    client_list_.join(shared_from_this());
    sLog.outString("A new client connected : %s", socket_.remote_endpoint().address().to_string().c_str());
    
    boost::asio::async_read_until(socket_, handshake_buffer_, "\r\n\r\n",
        boost::bind(&AuthSession::parseHanshake, this,
            boost::asio::placeholders::error));
    
}

void AuthSession::deliver(const AuthMessage& msg)
{
    ByteBuffer packet;
    uint32 length = msg.size();
    
    packet << (uint8)130;
    if(length <= 125){
        packet << (uint8)length;
    }
    else if(length >= 126 && length <= 65535 )
    {
        packet << (uint8)126;
        packet << (uint8)((length >> 8) & 255);
        packet << (uint8)((length) & 255);
    }
    else{
        packet << (uint8)127;
        packet << (uint8)((length >> 56) & 255);
        packet << (uint8)((length >> 48) & 255);
        packet << (uint8)((length >> 40) & 255);
        packet << (uint8)((length >> 32) & 255);
        packet << (uint8)((length >> 24) & 255);
        packet << (uint8)((length >> 16) & 255);
        packet << (uint8)((length >> 8) & 255);
        packet << (uint8)((length) & 255);
    }
    

    packet.appendData((const char*)msg.contents(), length);
    
    boost::asio::async_write(socket_,
                             boost::asio::buffer(packet.contents(),
                                                 packet.size()),
                             boost::bind(&AuthSession::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error));
}

void AuthSession::decodeHeader(const boost::system::error_code& error){
    if (!error)
    {
    ByteBuffer packet;
    received_message_ = new WebSocketMessage(BINARY_MESSAGE, 1); //Creating the new packet
    packet.appendData(buffer_, 2);
    
    uint8 opCode = 0, length = 0;
    
    packet >> opCode; //Get the opcode
    received_message_->SetOpcode(opCode);
        
    if(received_message_->GetOpcode() != 130) //Check if it's the right opcode
    {
        kick();
        return;
    }
        
    packet >> length; //Get the length
    length &= 127; //Remove the first bit
    received_message_->setSize(length);
    
    if(length<=125){ //read the mask ( 4 bytes ) + the additionnal length if needed
        received_message_->SetMaskIndex(0);
        boost::asio::async_read(socket_,
                                boost::asio::buffer(buffer_, 4),
                                boost::bind(&AuthSession::decodeMask, shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    else if(length == 126){
        received_message_->SetMaskIndex(2);
        boost::asio::async_read(socket_,
                                boost::asio::buffer(buffer_, 6),
                                boost::bind(&AuthSession::decodeMask, shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    else{ // 127
        received_message_->SetMaskIndex(8);
        boost::asio::async_read(socket_,
                                boost::asio::buffer(buffer_, 12),
                                boost::bind(&AuthSession::decodeMask, shared_from_this(),
                                            boost::asio::placeholders::error));
    }
    }
    else{
        kick();
    }
}

void AuthSession::decodeMask(const boost::system::error_code& error){
    if (!error)
    {
        int maskIndex = received_message_->GetMaskIndex();
        ByteBuffer mask;
        mask.appendData(buffer_, maskIndex+4);
     
        //fix length
        if(maskIndex == 2){
            uint16 length = 0;
            mask >> length;
            received_message_->setSize(length);
        }
        else if(maskIndex == 8){
            uint32 length = 0; //Well reading only 4 bytes, if it's > 1024 its going to kick anyway
            mask >> length;
            received_message_->setSize(length);
        }

        //Set max size
        if(received_message_->getSize()>1024)
        {
            kick();
            return;
        }
    
        //get mask
        static uint8 maskArray[3];
        for(int i=0;i<4;i++){
            uint8 value = 0;
            mask >> value;
            maskArray[i] = value;
        }
        received_message_->setMask(maskArray);

        //Read the actual data
        boost::asio::async_read(socket_,
                                boost::asio::buffer(buffer_, received_message_->getSize()),
                                boost::bind(&AuthSession::decodeData, shared_from_this(),
                                            boost::asio::placeholders::error));
    
    }
    else{
        kick();
    }
}

void AuthSession::decodeData(const boost::system::error_code& error){
    if (!error)
    {
        const uint32 size=received_message_->getSize();
		uint8* mask = received_message_->getMask(), byte=0;
		char message[1024];
        ByteBuffer extractor;
        
        //Extract and decrypt received data
        extractor.appendData(buffer_, received_message_->getSize());
        for(int i = 0;i<size;i++)
        {
            extractor >> byte;
            message[i] = byte ^ mask[(i)%4];
        }
        
        //Form packet
        AuthMessage packet;
        packet.appendData(message, received_message_->getSize());
        packet.set_opcode_(packet.read<uint8>());
        packet.set_length_(received_message_->getSize() - 1);
        
        OpcodeHandler const& opHandle = sOpcodeTable[packet.getOpcode_()];
        (this->*opHandle.handler)(packet);
        
        delete received_message_; //Freeee bird
        
        //Loop, reading new data :)
        boost::asio::async_read(socket_,
                                boost::asio::buffer(buffer_, 2),
                                boost::bind(&AuthSession::decodeHeader, shared_from_this(),
                                            boost::asio::placeholders::error));

    }
    else{
        kick();
    }
}

void AuthSession::parseHanshake(const boost::system::error_code& error){
    if(!error) {
        std::string command;
        std::istream is(&handshake_buffer_);
        
        getline(is, command);

        //If using wrong HTTP version
        if(command != "GET / HTTP/1.1\r")
        {
            sLog.outWarning("A nasty guy tried to connect outside of a browser or is using internet explorer");
            kick();
            return;
        }
        
        HandshakeParser parser(is);
        std::string handshakeSecKey = parser.getValue("Sec-WebSocket-Key");
        
        if(handshakeSecKey == "unknown"){
            kick();
            return;
        }
        
        std::string secWebSocketAccept = parser.getWebSocketAcceptKey(handshakeSecKey);

        handshake_answer_ = parser.getHandshakeAnswer(secWebSocketAccept);
		
        boost::asio::async_write(socket_,
			boost::asio::buffer(handshake_answer_,
                handshake_answer_.length()),
                    boost::bind(&AuthSession::handle_write, shared_from_this(),
					boost::asio::placeholders::error));
        
        boost::asio::async_read(socket_,
            boost::asio::buffer(buffer_, 2),
                boost::bind(&AuthSession::decodeHeader, shared_from_this(),
                   boost::asio::placeholders::error));
    }
    else
    {
        kick();
    }
}

void AuthSession::handle_write(const boost::system::error_code& error)
{
    if (!error){
    }
    else
    {
        kick();
    }
}

void AuthSession::handleLoginChallenge(AuthMessage& recvPacket){
    std::string username, password;
    AuthMessage answer;
    recvPacket >> username;
    recvPacket >> password;
    
    string lowerUsername = username;
    std::transform(lowerUsername.begin(), lowerUsername.end(), lowerUsername.begin(), ::tolower);
    
    answer << (uint8)STC_LOGIN_ANSWER;

    map<int, game_account>::const_iterator itr;
    
    for(itr = server_.getAccountList()->begin();itr != server_.getAccountList()->end(); ++itr){
		if((*itr).second.username == username || (*itr).second.username == lowerUsername)
		{
            if((*itr).second.password == password){
                answer << (uint8)1;
				connectPlayer((*itr).first, (*itr).second.username);
                deliver(answer);
                return;
            }
            else
            {
                answer << (uint8)2;
                deliver(answer);
                return;
            }
		}
	}
    
    answer << (uint8)2;
    deliver(answer);
}

void AuthSession::connectPlayer(int id, std::string username){
	player_list_.join(shared_from_this()); // Add the account to the player list
	
	//Set the account informations
	informations_.account_id = id;
	informations_.account_name = username;
	informations_.logged_in = true;

	sLog.outString("Connected player %s", username.c_str());
}

void AuthSession::kick(){
	if(informations_.logged_in){ //remove from the player list
		player_list_.remove(shared_from_this());
	}

	client_list_.remove(shared_from_this()); //remove from the whole client list
}

void AuthSession::handleRegisterChallenge(AuthMessage& recvPacket){
    bool verificationPassed = true;
    game_account newAcc;
    int accountId;
    
    recvPacket >> newAcc.username;
    recvPacket >> newAcc.password;
    recvPacket >> newAcc.email;
    string lowerUsername = newAcc.username;
    std::transform(lowerUsername.begin(), lowerUsername.end(), lowerUsername.begin(), ::tolower);
    
    AuthMessage answer;
    answer << (uint8)STC_REGISTER_ANSWER;
    
	map<int, game_account>::const_iterator itr;
    
	for(itr = server_.getAccountList()->begin();itr != server_.getAccountList()->end(); ++itr){
		if((*itr).second.username == newAcc.username || (*itr).second.username == lowerUsername) //Username already taken
		{
            answer << (uint8)2;
            verificationPassed = false;
		}
        if((*itr).second.email == newAcc.email) //Email already taken
		{
            answer << (uint8)3;
            verificationPassed = false;
        }
	}

	// Mail Regex
	boost::regex validation = boost::regex("[a-zA-Z0-9._%+-]+@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,4}");
	if(!boost::regex_match(newAcc.email, validation)) //Mail is invalid
		return;
    
    // Username Regex
    validation = boost::regex("[a-zA-Z]*");
    if(!boost::regex_match(newAcc.username, validation))
       return;
    
    // Check if verigication is ok
    if(!verificationPassed){
        deliver(answer); // Send answer
        return;
    }
    
    answer << (uint8)1;
    deliver(answer);
    
    if(!server_.getAccountList()->empty())
	  accountId = server_.getAccountList()->rbegin()->first + 1;
    else
      accountId = 1;
    
	server_.getAccountList()->insert(std::pair<int,game_account>(accountId, newAcc));

    /* Todo : Do not autosave */
	DatabaseQuery query;
	query.createAccount(accountId, newAcc.username, newAcc.password, newAcc.email);
	query.releaseConnection();

    sLog.outString("Received register username : %s password : %s email : %s", newAcc.username.c_str(), newAcc.password.c_str(), newAcc.email.c_str());
}



void AuthSession::handleNull(AuthMessage& recvPacket){
    sLog.outError("Received an unknow packet");
}
