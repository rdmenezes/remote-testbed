#include "mcs/database.h"

namespace remote { namespace mcs {

ManagedConnection::ManagedConnection() : sqlConn(true)
{}

ManagedConnection::~ManagedConnection()
{
	if (sqlConn.connected()) sqlConn.close();
}

bool ManagedConnection::connect(std::string db, std::string host, std::string user, std::string password)
{
	this->db = db; this->host = host; this->user = user; this->password = password;
	return sqlConn.connect(db.c_str(),host.c_str(),user.c_str(),password.c_str());
}

mysqlpp::Connection& ManagedConnection::getConnection(bool check)
{
	if (check) checkDBConnection();
	return sqlConn;
}

void ManagedConnection::checkDBConnection()
{
	try
	{
		if (sqlConn.ping() !=0)
		{
			sqlConn.connect(db.c_str(),host.c_str(),user.c_str(),password.c_str());
		}
	}
	catch ( mysqlpp::Exception e)
	{
		sqlConn.connect(db.c_str(),host.c_str(),user.c_str(),password.c_str());
	}
}

ManagedConnection dbConn;

}}
