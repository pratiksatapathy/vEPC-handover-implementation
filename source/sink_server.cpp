#include "ran_simulator.h"

int g_threads_count;
vector<thread> g_threads;
thread g_mon_thread;
TrafficMonitor g_traf_mon;

void traffic_monitor() {

}

void simulate( ) {

}

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<sink_server_exec> THREADS_COUNT" << endl;
		g_utils.handle_type1_error(-1, "Invalid usage error: sinkserver_checkusage");
	}
}

void init(char *argv[]) {
	g_threads_count = atoi(argv[1]);
	g_threads.resize(g_threads_count);
	g_traf_mon.tun.set_itf("tun1", "172.16.0.1/16");
	g_traf_mon.tun.conn("tun1");
}

void run() {
	int i;

	g_mon_thread = thread(traffic_monitor);
	g_mon_thread.detach();
	for (i = 0; i < g_threads_count; i++) {
		g_threads[i] = thread(sink, i);
	}	
	for (i = 0; i < g_threads_count; i++) {
		if (g_threads[i].joinable()) {
			g_threads[i].join();
		}
	}	
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}