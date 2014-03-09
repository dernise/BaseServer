#ifndef _AUTHSESSION_H
#define _AUTHSESSION_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <string.h>
#include "../Network/Messages.hpp"
#include "Players.hpp"
#include "../Network/Opcodes.hpp"
#include "../Utils/HandshakeParser.hpp"
#include "../Database/DatabaseQuery.hpp"

using boost::asio::ip::tcp;

typedef struct {
    int account_id;
    std::string account_name;
    bool logged_in;
} player_infos;

class AuthSession
    : public Player,
      public boost::enable_shared_from_this<AuthSession>
{
public:
    AuthSession(boost::asio::io_service& io_service, PlayerList& player_list)
    : socket_(io_service),
        player_list_(player_list)
    {
    }

	~AuthSession(){
	}

    tcp::socket& socket()
    {
        return socket_;
    }
    
    void set_account_id(int id){
        informations_.account_id = id;
    }
    
    int get_account_id(){
        return informations_.account_id;
    }
    
    void set_account_name(std::string name){
        informations_.account_name = name;
    }
    
    std::string get_account_name(){
        return informations_.account_name;
    }
    
    void set_online(){
        informations_.logged_in = true;
    }

    void start();
    void deliver(const AuthMessage& msg);
    void handle_write(const boost::system::error_code& error);
    void parseHanshake(const boost::system::error_code& error);
    void sendHandshakeAnswer(std::string handshakeKey);
    void decodeHeader(const boost::system::error_code& error);
    void decodeMask(const boost::system::error_code& error);
    void decodeData(const boost::system::error_code& error);
    void handleLoginChallenge(AuthMessage& recvPacket);
    void handleRegisterChallenge(AuthMessage& recvPacket);
    void handleNull(AuthMessage& recvPacket);

private:
    player_infos informations_;
    tcp::socket socket_;
    PlayerList& player_list_;
	char buffer_[1024];
    boost::asio::streambuf handshake_buffer_; //We cannot use a char* buffer with async_read_until :(
	std::string handshake_answer_;
    WebSocketMessage* received_message_;
};

typedef boost::shared_ptr<AuthSession> auth_session_ptr;

#endif