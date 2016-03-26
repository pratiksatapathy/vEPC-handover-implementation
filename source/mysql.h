#ifndef MYSQL_H
#define MYSQL_H

/* (C) MYSQL, MYSQL_RES, mysql_init */
#include <mysql/mysql.h>

#include "utils.h"

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
	ConnInfo conn_info;

	MySql();
	void conn();
	void handle_query(const char*, MYSQL_RES**);
	void handle_db_error();
	~MySql();
};

#endif //MYSQL_H