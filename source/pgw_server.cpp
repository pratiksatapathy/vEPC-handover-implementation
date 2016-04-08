#include "pgw_server.h"

int g_s5_server_threads_count;
int g_sgi_server_threads_count;
vector<thread> g_s5_server_threads;
vector<thread> g_sgi_server_threads;
Pgw g_pgw;

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
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	int i;

	/* PGW S5 server */
	cout << "PGW S5 server started" << endl;
	g_pgw.s5_server.run(g_pgw_s5_ip_addr, g_pgw_s5_port);
	for (i = 0; i < g_s5_server_threads_count; i++) {
		g_s5_server_threads[i] = thread(handle_s5_traffic);
	}

	/* PGW SGI server */
	cout << "PGW SGI server started" << endl;
	g_pgw.sgi_server.run(g_pgw_sgi_ip_addr, g_pgw_sgi_port);
	for (i = 0; i < g_sgi_server_threads_count; i++) {
		g_sgi_server_threads[i] = thread(handle_sgi_traffic);
	}

	/* Joining all threads */
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

void handle_s5_traffic() {
	struct sockaddr_in src_sock_addr;
	Packet pkt;

	while (1) {
		g_pgw.s5_server.rcv(src_sock_addr, pkt);
		pkt.extract_gtp_hdr();
		switch(pkt.gtp_hdr.msg_type) {
			/* Create session */
			case 1:
				cout << "pgwserver_handles5traffic:" << " case 1:" << endl;	
				g_pgw.handle_create_session(src_sock_addr, pkt);
				break;

			/* Uplink userplane data */
			case 2:
				cout << "pgwserver_handles5traffic:" << " case 2:" << endl;	
				g_pgw.handle_uplink_udata(pkt);
				break;

			/* Detach */
			case 4:
				cout << "pgwserver_handles5traffic:" << " case 4:" << endl;	
				g_pgw.handle_detach(src_sock_addr, pkt);
				break;

			/* For error handling */
			default:
				cout << "pgwserver_handles5traffic:" << " default case:" << endl;	
		}		
	}
}

void handle_sgi_traffic() {
	struct sockaddr_in src_sock_addr;
	Packet pkt;

	while (1) {
		g_pgw.sgi_server.rcv(src_sock_addr, pkt);

		/* Downlink userplane data */
		cout << "pgwserver_handlesgitraffic:" << endl;	
		g_pgw.handle_downlink_udata(pkt);
	}	
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}