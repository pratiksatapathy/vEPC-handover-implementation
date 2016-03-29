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

	/* SGW S11 server */
	cout << "SGW S11 server started" << endl;
	g_sgw.s11_server.run(g_sgw_s11_ip_addr.c_str(), g_sgw_s11_port);
	for (i = 0; i < g_s11_server_threads_count; i++) {
		g_s11_server_threads[i] = thread(handle_s11_traffic);
	}	

	/* SGW S1 server */
	cout << "SGW S1 server started" << endl;
	g_sgw.s1_server.run(g_sgw_s1_ip_addr.c_str(), g_sgw_s1_port);
	for (i = 0; i < g_s1_server_threads_count; i++) {
		g_s1_server_threads[i] = thread(handle_s1_traffic);
	}

	/* SGW S5 server */
	cout << "SGW S5 server started" << endl;
	g_sgw.s5_server.run(g_sgw_s5_ip_addr.c_str(), g_sgw_s5_port);
	for (i = 0; i < g_s5_server_threads_count; i++) {
		g_s5_server_threads[i] = thread(handle_s5_traffic);
	}

	/* Joining all threads */
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

void handle_s11_traffic() {
	struct sockaddr_in src_sock_addr;
	Packet pkt;

	while (1) {
		g_sgw.s11_server.rcv(src_sock_addr, pkt);
		pkt.extract_gtpc_hdr();
		switch(pkt.gtpc_hdr.msg_type) {
			/* Create session */
			case 1:
				g_sgw.handle_create_session(src_sock_addr, pkt);
				break;

			/* Modify bearer */
			case 2:
				g_sgw.handle_modify_bearer(src_sock_addr, pkt);
				break;

			/* For error handling */
			default:
				break;
		}		
	}
}

void handle_s1_traffic() {
	
}

void handle_s5_traffic() {
	
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}