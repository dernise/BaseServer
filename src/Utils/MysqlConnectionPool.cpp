#include "MysqlConnectionPool.hpp"

void MysqlConnectionPool::initializePool(string username, string password, string host, string keep_alive){
	username_=username;
	password_=password;
	host_=host;
	keep_alive_=keep_alive;
	loop_ = true;

	try{
		driver_ = get_driver_instance();
	}catch(sql::SQLException &e){
		std::cout<<"SQL error "<<e.getErrorCode()<<" at "<<__FUNCTION__
		<<std::cout<<"\nDescription :"<<e.what()<<std::endl;
	}

	startConnections(3); //Starting with 3 connections

	pingThread_ = new boost::thread(boost::bind(&MysqlConnectionPool::pingPool,this));

	sLog.outString("Initialized database connection pool");
}

void MysqlConnectionPool::startConnections(int number){
	connection_number_=number;
	sLog.outString("Starting %d mysql connections", number);

	for(int i=0; i<number;i++){
		pool_[i] = createConnection();
	}
}

connection_ MysqlConnectionPool::createConnection(){
	connection_ connection_struct;
	try{
		connection_struct.locked = false;
		connection_struct.conn = driver_->connect(host_, username_, password_);
		connection_struct.conn->setAutoCommit(false);
		connection_struct.conn->setSchema("warleague_realmd");
		pingConnection(connection_struct.conn);
	}catch(sql::SQLException &e){
	    std::cout<<"SQL error "<<e.getErrorCode()<<" at "<<__FUNCTION__
	    <<std::cout<<"\nDescription :"<<e.what()<<std::endl;
	}

	return connection_struct;
}

sql::Connection* MysqlConnectionPool::getConnection(){
	map_lock_.lock();
	int i;
	for(i=0; i<connection_number_;i++){
		if(!pool_[i].locked){
			pool_[i].locked = true;
			map_lock_.unlock();
			return pool_[i].conn;
		}
	}
	pool_[i]=createConnection();
	pool_[i].locked=true;
	connection_number_++;
	map_lock_.unlock();
	return pool_[i].conn;
}

void MysqlConnectionPool::releaseConnection(sql::Connection* conn){
	map_lock_.lock();
	for(int i=0; i<connection_number_;i++){
		if(pool_[i].conn == conn){
			pool_[i].locked = false;
		}
	}
	map_lock_.unlock();
}

bool MysqlConnectionPool::pingConnection(sql::Connection *conn){
	try{
		sql::Statement* stmt = conn->createStatement();
		stmt->executeQuery(keep_alive_);
		delete stmt;
		return true;
	}catch(sql::SQLException &e){
		std::cout<<"SQL error "<<e.getErrorCode()<<" at "<<__FUNCTION__
		<<std::cout<<"\nDescription :"<<e.what()<<std::endl;
		return false;
	}
}

void MysqlConnectionPool::pingPool(){
	while(loop_){
		boost::this_thread::sleep(boost::posix_time::minutes(5));
		sLog.outString("Pinging the whole mysql connection pool");
		map_lock_.lock();
		for(int i=0; i<connection_number_;i++){
			if(!pool_[i].locked){
				pingConnection(pool_[i].conn);
			}
		}
		map_lock_.unlock();
	}
}