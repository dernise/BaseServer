#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "AuthSession.hpp"

using boost::asio::ip::tcp;

class AuthServer
{
public:
  AuthServer(boost::asio::io_service& io_service,
      const tcp::endpoint& endpoint)
    : io_service_(io_service),
      acceptor_(io_service, endpoint)
  {
    start_accept();
  }

  void start_accept()
  {
    auth_session_ptr new_session(new AuthSession(io_service_, players));
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&AuthServer::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(auth_session_ptr session,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      session->start();
    }

    start_accept();
  }

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  Players players;
};

typedef boost::shared_ptr<AuthServer> AuthServer_ptr;