#include "mysql.h"

struct ConnDetails g_conn_details;

ConnDetails::ConnDetails() {
	
	server = allocate_str_mem(BUF_SIZE);
	user = allocate_str_mem(BUF_SIZE);
	password = allocate_str_mem(BUF_SIZE);
	db = allocate_str_mem(BUF_SIZE);
}

ConnDetails::ConnDetails(const ConnDetails &src_obj) {

	server = allocate_str_mem(BUF_SIZE);
	user = allocate_str_mem(BUF_SIZE);
	password = allocate_str_mem(BUF_SIZE);
	db = allocate_str_mem(BUF_SIZE);
	strcpy(server, src_obj.server);
	strcpy(user, src_obj.user);
	strcpy(password, src_obj.password);
	strcpy(db, src_obj.db);
}

void swap(ConnDetails &src_obj, ConnDetails &dst_obj) {
	using std::swap;

	swap(src_obj.server, dst_obj.server);
	swap(src_obj.user, dst_obj.user);
	swap(src_obj.password, dst_obj.password);
	swap(src_obj.db, dst_obj.db);
}

ConnDetails& ConnDetails::operator=(ConnDetails src_obj) {

	swap(*this, src_obj);
	return *this;
}

ConnDetails::ConnDetails(ConnDetails &&src_obj)
	:ConnDetails() {

	swap(*this, src_obj);
}

void ConnDetails::setup_details() {

	strcpy(server, "localhost");
	strcpy(user, "root");
	strcpy(password, "mysql");
	strcpy(db, "NFV");
}

ConnDetails::~ConnDetails() {

	free(server);
	free(user);
	free(password);
	free(db);
}

MySql::MySql() {

	conn = NULL;
	result = NULL;
}

void MySql::setup_conn() {
	
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, g_conn_details.server, g_conn_details.user, g_conn_details.password, g_conn_details.db, 0, NULL, 0))
		report_db_error(conn);
	print_message("Connected to database");
}

void MySql::perform_query(const char *query) {
	
	if (mysql_query(conn, query))
		report_db_error(conn);
	result = mysql_store_result(conn);
}

void MySql::report_db_error(MYSQL *conn) {
	
	cout << mysql_error(conn) << endl;
	exit(EXIT_FAILURE);
}

MySql::~MySql() {

	//mysql_free_result(result);
	mysql_close(conn);
}

void setup_conn_details() {

	g_conn_details.setup_details();
}
