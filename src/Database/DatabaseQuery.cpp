#include "DatabaseQuery.hpp"

DatabaseQuery::DatabaseQuery() { 
	conn_ = MysqlConnectionPool::getInstance().getConnection();
}

void DatabaseQuery::releaseConnection(){
	MysqlConnectionPool::getInstance().releaseConnection(conn_); 
}

void DatabaseQuery::createAccount(string username, string password, string email){
	sql::PreparedStatement  *prep_stmt;
	prep_stmt = conn_->prepareStatement("INSERT INTO accounts(username, password, email) VALUES (?, ?, ?)");

	prep_stmt->setString(1, username);
	prep_stmt->setString(2, password);
	prep_stmt->setString(3, email);
	prep_stmt->executeQuery();
	conn_->commit();
}