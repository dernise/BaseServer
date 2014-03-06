#include "AuthServer.hpp"

void AuthServer::start_accept()
{
    auth_session_ptr new_session(new AuthSession(io_service_, players));
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&AuthServer::handle_accept, this, new_session,
                                       boost::asio::placeholders::error));
}

void AuthServer::handle_accept(auth_session_ptr session,const boost::system::error_code& error)
{
    if (!error)
    {
        session->start();
    }
    
    start_accept();
}

