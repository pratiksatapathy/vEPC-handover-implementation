#include "ran_simulator.h"

time_t g_start_time;
int g_threads_count;
double g_req_dur;
uint64_t g_run_dur;
int g_tot_regs;
uint64_t g_tot_regstime;
pthread_mutex_t g_mux;
thread g_mon_thread;
vector<thread> g_threads;
TrafficMonitor g_traf_mon;

Ran ranS; //source Ran
Ran ranT; //target Ran


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
			g_traf_mon.handle_downlink_udata();
		}
	}
}

/* handover changes starts */


int handle_mme_conn(int conn_fd) {

	bool res;
	Packet pkt;

	server.rcv(conn_fd, pkt);

	pkt.extract_s1ap_hdr();
	if (pkt.s1ap_hdr.mme_s1ap_ue_id > 0) {
		switch (pkt.s1ap_hdr.msg_type) {

		case 7:
			cout << "Target Ran: handle handover request:" << " case 7:" << endl;
			ranT.handle_handover(pkt);

			break;
		case 8:
			cout << "Source Ran:receive indirect teid from sgw:" << " case 8:" << endl;
			ranS.indirect_tunnel_complete(pkt);
			break;
		case 9:
			cout << "Source Ran to intiate a teardown for indirect teid:" << " case 9:" << endl;
			ranS.request_tear_down(pkt);

			break;
			/* For error handling */
		default:
			cout << "ran_simulator_handle_mme:" << " default case: handover" << endl;
			break;
		}
	}
	return 1;
}
//server created to receive mme initiated incoming connection to Ran
void start_signal_monitor(int a) {

	int threadCount = 2;//some value,1 is also enough
	server.run(g_ran_sctp_ip_addr, g_ran_port, threadCount, handle_mme_conn);

}

void simulate(int arg) {
	CLOCK::time_point	start_time;
	CLOCK::time_point stop_time;
	MICROSECONDS time_diff_us;	
	Ran ran;
	int status;
	int ran_num;
	bool ok;
	bool time_exceeded;

	ran_num = arg;

	// HO changes start
	//	ran.ran_context.eNodeB_id = ran_num;
	// HO changes end

	time_exceeded = false;
	ran.init(ran_num);
	ran.conn_mme();
	while (1) {
		/* Run duration check */
		g_utils.time_check(g_start_time, g_req_dur, time_exceeded);
		if (time_exceeded) {
			break;
		}
		
		/* Start time */
		start_time = CLOCK::now();	

		/* Attach */
		ran.initial_attach();
		ok = ran.authenticate();
		if (!ok) {
			continue;
		}
		ok = ran.set_security();
		if (!ok) {
			continue;
		}
		ok = ran.set_eps_session(g_traf_mon);
		if (!ok) {
			continue;
		}

		/* Data transfer */
		// ran.transfer_data();

		/* Detach */
		ok = ran.detach();
		if (!ok) {
			continue;
		}

		/* Stop time */
		stop_time = CLOCK::now();

		/* Response time */
		time_diff_us = std::chrono::duration_cast<MICROSECONDS>(stop_time - start_time);

		/* Updating performance metrics */
		g_sync.mlock(g_mux);
		g_tot_regs++;
		g_tot_regstime += time_diff_us.count();		
		g_sync.munlock(g_mux);
	}
}
void simulateHandover(int arg) {

	cout<<"simulating.Handover between sRan and tRan\n";

	CLOCK::time_point start_time;
	CLOCK::time_point stop_time;
	MICROSECONDS time_diff_us;

	int status;
	int ran_num;
	bool ok;
	bool time_exceeded;
	ranS.handover_state = 0; //not in handover;

	time_exceeded = false;

	ranS.init(1);// source enodeb intialization
	ranT.init(2); // target enodeb initialization

	ranS.conn_mme();


	//attaching ranS


		ranS.initial_attach();
		ok = ranS.authenticate();
		if (!ok) {
			cout<<"auth_error";
		}
		ok = ranS.set_security();
		if (!ok) {
			cout<<"set_Sec_error";
		}
		ok = ranS.set_eps_session(g_traf_mon);
		if (!ok) {
			cout<<"set_eps_error";
		}
	//attach complete
		cout<<"initiate Handover\n";

		ranS.initiate_handover();
		//sleep for some time
		usleep(10000000);

		//here ranT signals that ue has connected to tRan and its ready to take over
		//this resultss in switching of down link to target enodeb and tearing down of indirect tunnel
		ranT.complete_handover();

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
	g_req_dur = atof(argv[2]);
	g_tot_regs = 0;
	g_tot_regstime = 0;
	g_sync.mux_init(g_mux);	
	g_threads.resize(g_threads_count);
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	int i;
	g_threads_count = 1;
	cout<<"inside run  \n";
	/* Tun */
	// g_traf_mon.tun.set_itf("tun1", "172.16.0.1/16");
	// g_traf_mon.tun.conn("tun1");

	/* Traffic monitor server */
	// cout << "Traffic monitor server started" << endl;
	// g_traf_mon.server.run(g_trafmon_ip_addr, g_trafmon_port);	

	// g_mon_thread = thread(traffic_monitor);
	//	 g_mon_thread.detach();

		g_threads[0] = thread(start_signal_monitor, 0);
		g_threads[1] = thread(simulateHandover, 0);



	for (i = 0; i < 3; i++) {
		if (g_threads[i].joinable()) {
			g_threads[i].join();
		}
	}	
}



void print_results() {
	g_run_dur = difftime(time(0), g_start_time);
	cout << "Requested duration has ended. Finishing the program." << endl;
	cout << "Total number of registrations is " << g_tot_regs << endl;
	cout << "Total time for registrations is " << g_tot_regstime << endl;
	cout << "Latency is " << ((double)g_tot_regstime/g_tot_regs)*1e-6 << endl;
	cout << "Throughput is " << ((double)g_tot_regs/g_run_dur) << endl;
}

int main(int argc, char *argv[]) {
	cout<<"simulation start...\n"<<endl;
	//check_usage(argc);
	init(argv);
	run();

	cout<<"261..<<endl";

	//print_results();
	return 0;
}
