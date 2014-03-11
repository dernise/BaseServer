#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "Network/Messages.hpp"
#include "Utils/Log.hpp"
#include "Network/AuthServer.hpp"
#include "Network/Opcodes.hpp"
#include "Utils/MysqlConnectionPool.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
		sLog.Initialize("server.log", LOG_LVL_DEBUG);
		
        sLog.outString("Starting Warleague Server");
        
        
        sLog.outString("__  _  _______ _______");
        sLog.outString("\\ \\/ \\/ /\\__  \\\\_  __ \\");
        sLog.outString(" \\     /  / __ \\|  | \\/");
        sLog.outString("  \\/\\_/  (____  /__|");
        sLog.outString("              \\/");
        
		MysqlConnectionPool::getInstance().initializePool("server","n6q48D5746G6s3r","tcp://warleague.fr:3306","select 0  from dual;");

		boost::asio::io_service io_service;
        tcp::endpoint endpoint(tcp::v4(), 8080);
        
        AuthServer_ptr server(new AuthServer(io_service, endpoint));
		server->initGameAccounts();
		sLog.outString("Initialized server on port 8080");
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}