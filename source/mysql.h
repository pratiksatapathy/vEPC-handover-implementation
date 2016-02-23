#ifndef MYSQL_H
#define MYSQL_H

#include "utils.h"

//(C) Operations: MySQL_DB functions
#include <mysql/mysql.h>
#include <mysql/my_global.h>

struct ConnDetails {
	char *server;
	char *user;
	char *password;
	char *db;

	ConnDetails();
	ConnDetails(const ConnDetails&);
	friend void swap(ConnDetails&, ConnDetails&);
	ConnDetails& operator=(ConnDetails);
	ConnDetails(ConnDetails&&);
	void setup_details();
	~ConnDetails();
};

extern struct ConnDetails g_conn_details;

struct MySql {
	MYSQL *conn;
	MYSQL_RES *result;

	MySql();
	void setup_conn();
	void perform_query(const char*);
	void report_db_error(MYSQL*);
	~MySql();
};

void setup_conn_details();

#endif //MYSQL_H