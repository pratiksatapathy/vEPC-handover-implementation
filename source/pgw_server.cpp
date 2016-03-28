#include "pgw_server.h"

Pgw g_pgw;
int g_s5_server_threads_count;
int g_sgi_server_threads_count;
vector<thread> g_s5_server_threads;
vector<thread> g_sgi_server_threads;

void check_usage(int argc) {
	if (argc < 3) {
		cout << "Usage: ./<pgw_server_exec> S5_SERVER_THREADS_COUNT SGI_SERVER_THREADS_COUNT" << endl;
		g_utils.handle_type1_error(-1, "Invalid usage error: pgwserver_checkusage");
	}
}

void init(char *argv[]) {
	g_s5_server_threads_count = atoi(argv[1]);
	g_sgi_server_threads_count = atoi(argv[2]);
	g_s5_server_threads.resize(g_s5_server_threads_count);
	g_sgi_server_threads.resize(g_sgi_server_threads_count);
}

void run() {
	int i;

	for (i = 0; i < g_s5_server_threads_count; i++) {
		g_s5_server_threads[i] = thread(handle_s5_server);
	}
	for (i = 0; i < g_sgi_server_threads_count; i++) {
		g_sgi_server_threads[i] = thread(handle_sgi_server);
	}
	for (i = 0; i < g_s5_server_threads_count; i++) {
		if (g_s5_server_threads[i].joinable()) {
			g_s5_server_threads[i].join();
		}
	}	
	for (i = 0; i < g_sgi_server_threads_count; i++) {
		if (g_sgi_server_threads[i].joinable()) {
			g_sgi_server_threads[i].join();
		}
	}				
}

void handle_s5_server() {
	
}

void handle_sgi_server() {
	
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}