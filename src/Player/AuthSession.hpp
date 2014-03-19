#ifndef _AUTHSESSION_H
#define _AUTHSESSION_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <string.h>
#include "../Network/Messages.hpp"
#include "Players.hpp"
#include "../Network/Opcodes.hpp"
#include "../Utils/HandshakeParser.hpp"
#include "../Database/DatabaseQuery.hpp"

using boost::asio::ip::tcp;

class AuthServer;

class AuthSession
    : public Client,
      public boost::enable_shared_from_this<AuthSession>
{
public:
    AuthSession(boost::asio::io_service& io_service, ClientList& client_list, PlayerList& player_list, AuthServer& server)
    : socket_(io_service), client_list_(client_list), player_list_(player_list), server_(server)
    {
    }

	~AuthSession(){
	}

    tcp::socket& socket()
    {
        return socket_;
    }
   
	std::string get_account_name(){
        return informations_.account_name;
    }
    
	int get_account_id(){
        return informations_.account_id;
    }

    void start();
    void deliver(const AuthMessage& msg);
    void handle_write(const boost::system::error_code& error);
    void parseHanshake(const boost::system::error_code& error);
    void sendHandshakeAnswer(std::string handshakeKey);
    void decodeHeader(const boost::system::error_code& error);
    void decodeMask(const boost::system::error_code& error);
    void decodeData(const boost::system::error_code& error);
	void kick();
    void handleLoginChallenge(AuthMessage& recvPacket);
    void handleRegisterChallenge(AuthMessage& recvPacket);
    void handleNull(AuthMessage& recvPacket);
	void handleMessage(AuthMessage& recvPacket);
private:
	void connectPlayer(int id, std::string username);

	void set_account_id(int id){
        informations_.account_id = id;
    }
    
    void set_account_name(std::string name){
        informations_.account_name = name;
    }
  
    void set_online(){
        informations_.logged_in = true;
    }

    tcp::socket socket_;
    ClientList& client_list_;
	PlayerList& player_list_;
	AuthServer& server_;
	char buffer_[1024];
    boost::asio::streambuf handshake_buffer_; //We cannot use a char* buffer with async_read_until :(
	std::string handshake_answer_;
    WebSocketMessage* received_message_;
};

typedef boost::shared_ptr<AuthSession> auth_session_ptr;

#endif