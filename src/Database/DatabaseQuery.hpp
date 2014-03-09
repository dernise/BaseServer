#ifndef _DATABASE_QUERY_H
#define _DATABASE_QUERY_H

#include <cppconn\connection.h>
#include <cppconn\prepared_statement.h>
#include "../Utils/MysqlConnectionPool.hpp"

class DatabaseQuery{
public:
	DatabaseQuery();
	
	void releaseConnection();
	void createAccount(string username, string password, string email);
private:
	sql::Connection *conn_;
};

#endif