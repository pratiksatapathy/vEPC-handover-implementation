#include "mysql.h"

ConnInfo::ConnInfo() {
	server = "localhost";
	user = "root";
	passwd = "mysql";
	db = "hss";
}

ConnInfo::~ConnInfo() {

}

MySql::MySql() {
	conn_fd = mysql_init(NULL);
}

void MySql::conn() {
	if (!mysql_real_connect(conn_fd, conn_info.server.c_str(), conn_info.user.c_str(), conn_info.passwd.c_str(), conn_info.db.c_str(), 0, NULL, 0)) {
		handle_db_error();
	}
}

void MySql::handle_query(const char *query, MYSQL_RES **result) {
	if (mysql_query(conn_fd, query)) {
		handle_db_error();
	}
	*result = mysql_store_result(conn_fd);
	if (*result == NULL) {
		handle_db_error();
	}

}

void MySql::handle_db_error() {
	cout << mysql_error(conn_fd) << endl;
	g_utils.handle_type1_error(-1, "mysql error: mysql_handledberror");
}

MySql::~MySql() {
	mysql_close(conn_fd);
}