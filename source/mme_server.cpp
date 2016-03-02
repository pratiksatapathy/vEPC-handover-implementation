#include "mme_server.h"

SctpServer g_mme_server;

int handle_ue(int conn_fd) {

}

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> WORKER_THREADS_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error")
	}
}

void init(char *argv[], int &workers_count) {
	workers_count = atoi(argv[1]);
}

void run(int workers_count) {
	g_mme_server.run(g_mme_ip_addr.c_str(), g_mme_port, workers_count, handle_ue);
}

int main(int argc, char *argv[]) {
	int workers_count;

	check_usage(argc);
	init(argv, workers_count);
	run(workers_count);
	return 0;
}


