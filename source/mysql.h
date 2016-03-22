#ifndef MYSQL_H
#define MYSQL_H

#include "utils.h"

/* (C) MYSQL, MYSQL_RES, mysql_init */
#include <mysql/mysql.h>

class ConnInfo {
public:
	string server;
	string user;
	string passwd;
	string db;

	ConnInfo();
	~ConnInfo();
};

class MySql {
public:
	MYSQL *conn_fd;
	MYSQL_RES *result;
	ConnInfo conn_info;

	MySql();
	void conn();
	void handle_query(const char*);
	void handle_db_error();
	~MySql();
};

#endif //MYSQL_H