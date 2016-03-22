#ifndef MYSQL_H
#define MYSQL_H

#include "utils.h"

/* (C) MYSQL, MYSQL_RES, mysql_init */
#include <mysql/mysql.h>

class ConnDetails {
public:	
	char *server;
	char *user;
	char *password;
	char *db;

	ConnDetails();
	ConnDetails(const ConnDetails&);
	friend void swap(ConnDetails&, ConnDetails&);
	ConnDetails& operator=(ConnDetails);
	ConnDetails(ConnDetails&&);
	~ConnDetails();
};

class MySql {
public:
	MYSQL *conn;
	MYSQL_RES *result;

	MySql();
	void setup_conn();
	void perform_query(const char*);
	void report_db_error(MYSQL*);
	~MySql();
};

extern ConnDetails g_conn_details;

void setup_conn_details();

#endif //MYSQL_H