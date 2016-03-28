#include "sgw_server.h"

Sgw g_sgw;
int g_s11_server_threads_count;
int g_s1_server_threads_count;
int g_s5_server_threads_count;
vector<thread> g_s11_server_threads;
vector<thread> g_s1_server_threads;
vector<thread> g_s5_server_threads;

void check_usage(int argc) {
	if (argc < 4) {
		cout << "Usage: ./<sgw_server_exec> S11_SERVER_THREADS_COUNT S1_SERVER_THREADS_COUNT S5_SERVER_THREADS_COUNT" << endl;
		g_utils.handle_type1_error(-1, "Invalid usage error: sgwserver_checkusage");
	}
}

void init(char *argv[]) {
	g_s11_server_threads_count = atoi(argv[1]);
	g_s1_server_threads_count = atoi(argv[2]);
	g_s5_server_threads_count = atoi(argv[3]);
	g_s11_server_threads.resize(g_s11_server_threads_count);
	g_s1_server_threads.resize(g_s1_server_threads_count);
	g_s5_server_threads.resize(g_s5_server_threads_count);
}

void run() {
	int i;

	for (i = 0; i < g_s11_server_threads_count; i++) {
		g_s11_server_threads[i] = thread(handle_s11_server);
	}	
	for (i = 0; i < g_s1_server_threads_count; i++) {
		g_s1_server_threads[i] = thread(handle_s1_server);
	}
	for (i = 0; i < g_s5_server_threads_count; i++) {
		g_s5_server_threads[i] = thread(handle_s5_server);
	}
	for (i = 0; i < g_s11_server_threads_count; i++) {
		if (g_s11_server_threads[i].joinable()) {
			g_s11_server_threads[i].join();
		}
	}
	for (i = 0; i < g_s1_server_threads_count; i++) {
		if (g_s1_server_threads[i].joinable()) {
			g_s1_server_threads[i].join();
		}
	}	
	for (i = 0; i < g_s5_server_threads_count; i++) {
		if (g_s5_server_threads[i].joinable()) {
			g_s5_server_threads[i].join();
		}
	}				
}

void handle_s11_server() {

}

void handle_s1_server() {
	
}

void handle_s5_server() {
	
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}