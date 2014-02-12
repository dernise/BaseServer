#ifndef _AUTHSESSION_H
#define _AUTHSESSION_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string.h>
#include "Messages.hpp"
#include "Players.hpp"
#include "Opcodes.hpp"

using boost::asio::ip::tcp;

class AuthSession
    : public Player,
      public boost::enable_shared_from_this<AuthSession>
{
public:
    AuthSession(boost::asio::io_service& io_service, Players& room)
    : socket_(io_service),
        playerList(room)
    {
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        playerList.join(shared_from_this());
        sLog.outString("A new client connected : %s", socket_.remote_endpoint().address().to_string().c_str());
		deliver(welcomeMessage());
		boost::asio::async_read(socket_,
            boost::asio::buffer((char*)receivedMessage.contents(), 4), // 4 is the length of the header
                boost::bind(&AuthSession::handle_read_header, shared_from_this(),
                    boost::asio::placeholders::error));
    
    }

	AuthPacket welcomeMessage(){
        AuthPacket welcomeMessage(CTS_WELCOME,1);
		char jungle[20] = "Welcome";
		welcomeMessage << jungle;
		return welcomeMessage;
	}

    void deliver(const AuthPacket& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
			ByteBuffer packet = write_msgs_.front().getPacket();
            boost::asio::async_write(socket_,
				boost::asio::buffer(packet.contents(),
				    packet.size()),
                        boost::bind(&AuthSession::handle_write, shared_from_this(),
                            boost::asio::placeholders::error));
        }
    }

	bool decodeHeader(){
		uint16 length, opCode;
		receivedMessage >> length;
		receivedMessage >> opCode;
		receivedMessage.setSize(length);
		receivedMessage.SetOpcode(opCode);
		return true;
	}

    void handle_read_header(const boost::system::error_code& error)
    {
        if (!error && decodeHeader())
        {
          /*boost::asio::async_read(socket_,
            boost::asio::buffer(receivedMessage.body(), read_msg_.body_length()),
              boost::bind(&AuthSession::handle_read_body, shared_from_this(),
                boost::asio::placeholders::error));*/
			sLog.outString("Received authmessage lenght : %d, opCode : %d", receivedMessage.getSize(), receivedMessage.GetOpcode());
        }
        else
        {
            playerList.remove(shared_from_this());
        }
    }

    void handle_read_body(const boost::system::error_code& error)
    {
        /*if (!error)
        {
            playerList.deliver(read_msg_);
            boost::asio::async_read(socket_,
                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                    boost::bind(&AuthSession::handle_read_header, shared_from_this(),
                        boost::asio::placeholders::error));
        }
        else
        {
            playerList.remove(shared_from_this());
        }*/
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
				ByteBuffer packet = write_msgs_.front().getPacket();
				boost::asio::async_write(socket_,
					boost::asio::buffer(packet.contents(),
						packet.size()),
							boost::bind(&AuthSession::handle_write, shared_from_this(),
								boost::asio::placeholders::error));
            }
        }
        else
        {
            playerList.remove(shared_from_this());
        }
    }
    
private:
    tcp::socket socket_;
    Players& playerList;
    AuthPacket receivedMessage;
    auth_message_queue write_msgs_;
};

typedef boost::shared_ptr<AuthSession> auth_session_ptr;

#endif