#include "ran_simulator.h"

time_t g_start_time;
int g_threads_count;
double g_req_duration;
vector<thread> g_threads;
thread g_mon_thread;
TrafficMonitor g_traf_mon;

void traffic_monitor() {
	fd_set rcv_set;
	int max_fd;
	int status;

	max_fd = max(g_traf_mon.tun.conn_fd, g_traf_mon.server.conn_fd);
	while (1) {
		FD_ZERO(&rcv_set);
		FD_SET(g_traf_mon.tun.conn_fd, &rcv_set); 
		FD_SET(g_traf_mon.server.conn_fd, &rcv_set); 
		status = select(max_fd + 1, &rcv_set, NULL, NULL, NULL);
		g_utils.handle_type1_error(status, "select error: ransimulator_trafficmonitor");
		if (FD_ISSET(g_traf_mon.tun.conn_fd, &rcv_set)) {
			g_traf_mon.handle_uplink_udata();
		}
		else if (FD_ISSET(g_traf_mon.server.conn_fd, &rcv_set)) {
			g_traf_mon.handle_downlink_udata();	
		}
	}
}

void simulate(int arg) {
	Ran ran;
	int ran_num;
	int status;
	bool time_exceeded;
	
	ran_num = arg;
	time_exceeded = false;
	ran.init(arg);
	// while (1) {
	// 	time_check(g_start_time, g_req_duration, time_exceeded);
	// 	if (time_exceeded) {
	// 		break;
	// 	}	
		ran.initial_attach();
		ran.authenticate();
		ran.set_security();
		ran.set_eps_session(g_traf_mon);
		ran.transfer_data();
		ran.detach();
	// }
}

void check_usage(int argc) {
	if (argc < 3) {
		cout << "Usage: ./<ran_simulator_exec> THREADS_COUNT DURATION" << endl;
		g_utils.handle_type1_error(-1, "Invalid usage error: ransimulator_checkusage");
	}
}

void init(char *argv[]) {
	g_start_time = time(0);
	g_threads_count = atoi(argv[1]);
	g_req_duration = atof(argv[2]);
	g_threads.resize(g_threads_count);
	g_traf_mon.tun.set_itf("tun1", "172.16.0.1/16");
	g_traf_mon.tun.conn("tun1");
}

void run() {
	int i;

	g_mon_thread = thread(traffic_monitor);
	g_mon_thread.detach();
	for (i = 0; i < g_threads_count; i++) {
		g_threads[i] = thread(simulate, i);
	}	
	for (i = 0; i < g_threads_count; i++) {
		if (g_threads[i].joinable()) {
			g_threads[i].join();
		}
	}	
}

void print_results() {

}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	print_results();
	return 0;
}