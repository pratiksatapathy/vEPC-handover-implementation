#include "hss_server.h"

Hss g_hss;
int g_workers_count;

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<hss_server_exec> THREADS_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error: hssserver_checkusage");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
	if (mysql_library_init(0, NULL, NULL)) {
		handle_type1_error(-1, "mysql_library_init error: hssserver_init");
	}
}

void run() {
	g_hss.server.run(g_hss_ip_addr.c_str(), g_hss_port, g_workers_count, handle_mme);
}

void handle_mme(int conn_fd) {
	Packet pkt;

	g_hss.server.rcv(conn_fd, pkt);
	pkt.extract_diameter_hdr();
	switch (pkt.diameter_hdr.msg_type) {
		case 1:
			g_hss.handle_autn_info_req(conn_fd, pkt);
			break;
		default:
			break;
	}
}

void finish() {
	mysql_library_end();		
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	finish();
	return 0;
}