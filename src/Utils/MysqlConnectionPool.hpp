#ifndef _MYSQL_CONNECTION_POOL_H
#define _MYSQL_CONNECTION_POOL_H

#include <map>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Log.hpp"

using namespace std;

typedef struct{
	sql::Connection* conn;
	bool locked;
} connection_; 

class MysqlConnectionPool{
public: 
	static MysqlConnectionPool &getInstance(){
		static MysqlConnectionPool pool;
		return pool;
	}

	void initializePool(string, string, string, string);
	sql::Connection* getConnection();
	void releaseConnection(sql::Connection* conn);
private:
	MysqlConnectionPool(){ }
	void startConnections(int);
	bool pingConnection(sql::Connection *conn);
	connection_ createConnection();
	void pingPool();

	boost::thread *pingThread_;
	boost::mutex map_lock_;
	std::map<short, connection_> pool_;
	sql::Driver *driver_;
	std::string username_;
	std::string password_;
	std::string host_;
	std::string keep_alive_;
	bool loop_;
	int connection_number_;
};

#endif
