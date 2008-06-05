#ifndef REMOTE_MCS_DATABASE_H
#define REMOTE_MCS_DATABASE_H

#include <mysql++/mysql++.h>

namespace remote { namespace mcs {

//using namespace mysqlpp;

/** The mysql++ database connection handler class handles the database connection and
 * reconnects when needed.
**/
class ManagedConnection
{
	public:
		/** Constructor **/
		ManagedConnection();
		/** Destructor **/
		virtual ~ManagedConnection();
		/** Connect to the specified database
		 * \param db Database name
		 * \param host Host name
		 * \param user User name
		 * \param password Password
		 * \returns true if connection is successful
		**/
		bool connect (std::string db, std::string host, std::string user, std::string password);
		/**
		 * \param check If true, check if the connection is alive.
		 * \returns A reference to the current connection object
		**/
		mysqlpp::Connection& getConnection(bool check=true);
	private:
		/** Check if the database connection is still alive and try to reconnect if not. **/
		void checkDBConnection();
		/** The database connection object **/
		mysqlpp::Connection sqlConn;
		/** Database name **/
		std::string db;
		/** Host name **/
		std::string host;
		/** User name **/
		std::string user;
		/** Password **/
		std::string password;
};

extern ManagedConnection dbConn;

}}




#endif
