#ifndef _DATABASE_QUERY_H
#define _DATABASE_QUERY_H

#include <cppconn\connection.h>
#include <cppconn\prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "../Utils/MysqlConnectionPool.hpp"

class DatabaseQuery{
public:
	DatabaseQuery();
	
	sql::ResultSet* getAccountList();
	void releaseConnection();
	void createAccount(int id, string username, string password, string email);
private:
	sql::Connection *conn_;
};

#endif