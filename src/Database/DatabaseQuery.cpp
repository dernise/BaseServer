#include "DatabaseQuery.hpp"

DatabaseQuery::DatabaseQuery() { 
	conn_ = MysqlConnectionPool::getInstance().getConnection();
}

void DatabaseQuery::releaseConnection(){
	MysqlConnectionPool::getInstance().releaseConnection(conn_); 
}

sql::ResultSet* DatabaseQuery::getAccountList(){
	sql::Statement *stmt;
	sql::ResultSet *res;

	stmt = conn_->createStatement();
	res = stmt->executeQuery("SELECT * FROM accounts");
	return res;
}

sql::ResultSet* DatabaseQuery::getFriendships(){
	sql::Statement *stmt;
	sql::ResultSet *res;

	stmt = conn_->createStatement();
	res = stmt->executeQuery("SELECT * FROM friendships");
	return res;
}

void DatabaseQuery::createAccount(int id, string username, string password, string email){
	sql::PreparedStatement  *prep_stmt;
	prep_stmt = conn_->prepareStatement("INSERT INTO accounts(id, username, password, email) VALUES (?, ?, ?, ?)");

	prep_stmt->setInt(1, id);
	prep_stmt->setString(2, username);
	prep_stmt->setString(3, password);
	prep_stmt->setString(4, email);
	prep_stmt->executeQuery();
	
	conn_->commit();
}