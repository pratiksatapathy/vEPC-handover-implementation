#include "ran_simulator.h"

int g_threads_count;
vector<thread> g_threads;

void simulator(int num) {
	Ran ran;
	

}

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<ran_simulator_exec> UE_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error");
	}
}

void init(char *argv[]) {
	g_threads_count = atoi(argv[1]);
	g_threads.resize(g_threads_count);
}

void run() {
	int i;

	for (i = 0; i < g_threads_count; i++) {
		g_threads[i] = thread(simulator, i);
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