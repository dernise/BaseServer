#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "Messages.hpp"
#include "Log.hpp"
#include "AuthServer.hpp"


using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
		sLog.Initialize("server.log", LOG_LVL_DEBUG);
		sLog.outString("Starting server EternalKingdom");
	  	sLog.outString("   ___      __  __           ");
        sLog.outString("  / __|___ / _|/ _|___ ___   ");
        sLog.outString(" | (__/ _ \\  _|  _/ -_) -_)  ");
        sLog.outString("  \\___\\___/_| |_| \\___\\___| AuthServer 0.1");
		boost::asio::io_service io_service;
        tcp::endpoint endpoint(tcp::v4(), 12121);
        chat_server_ptr server(new chat_server(io_service, endpoint));
        sLog.outString("Done");
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}