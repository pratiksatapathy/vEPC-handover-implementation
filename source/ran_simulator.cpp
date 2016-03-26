#include "ran_simulator.h"

time_t g_start_time;
int g_threads_count;
double g_req_duration;
vector<thread> g_threads;

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
		ran.setup_security();
		ran.setup_eps_session();
		ran.transfer_data();
		ran.detach();
	// }
}

void check_usage(int argc) {
	if (argc < 3) {
		cout << "Usage: ./<ran_simulator_exec> THREADS_COUNT DURATION" << endl;
		handle_type1_error(-1, "Invalid usage error: ransimulator_checkusage");
	}
}

void init(char *argv[]) {
	g_start_time = time(0);
	g_threads_count = atoi(argv[1]);
	g_req_duration = atof(argv[2]);
	g_threads.resize(g_threads_count);
}

void run() {
	int i;

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