#include "mme_server.h"

SctpServer g_mme_server;
int g_workers_count;

int handle_ue(int conn_fd) {
	return 0;
}

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> WORKER_THREADS_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
}

void run() {
	g_mme_server.run(g_mme_ip_addr.c_str(), g_mme_port, g_workers_count, handle_ue);
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}


