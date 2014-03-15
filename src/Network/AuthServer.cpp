#include "AuthServer.hpp"

void AuthServer::start_accept()
{
    auth_session_ptr new_session(new AuthSession(io_service_, clients, players, *this));
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

void AuthServer::initGameAccounts(){
	sql::ResultSet *result;
	
	DatabaseQuery query;
	result = query.getAccountList();

	while(result->next()){
		game_account account;
		account.username = result->getString("username");
		account.password = result->getString("password");
		account.email = result->getString("email");
		accounts_map[result->getInt("id")] = account;
	}
	
	sLog.outString("Loaded accounts from the database");
}
