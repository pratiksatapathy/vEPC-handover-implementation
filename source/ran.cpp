#include "ran.h"

EnodeB g_enodeb;

int g_total_connections;
double g_req_duration;
time_t g_start_time;
unsigned long long g_run_dur;

vector<int> g_ue_num;
pthread_t g_mon_tid;
vector<pthread_t> g_tid;

pthread_t g_ping_tid;

void setup_tun() {

	system("sudo openvpn --rmtun --dev tun1");
	system("sudo openvpn --mktun --dev tun1");
	system("sudo ip link set tun1 up");
	system("sudo ip addr add 172.16.0.1/16 dev tun1");
}

void* monitor_traffic(void *arg) {
	fd_set read_set;
	int max_fd;
	int status;

	g_enodeb.attach_to_tun();
	g_enodeb.setup_tosgw();
	max_fd = max(g_enodeb.tun_fd, g_enodeb_server.server_socket);

	while (1) {
		FD_ZERO(&read_set);
		FD_SET(g_enodeb.tun_fd, &read_set); 
		FD_SET(g_enodeb_server.server_socket, &read_set); 

		status = select(max_fd + 1, &read_set, NULL, NULL, NULL);
		report_error(status, "Select-process failure\tTry again");		

		if (FD_ISSET(g_enodeb.tun_fd, &read_set)) {
			uplink_data_transfer();
		}
		else if (FD_ISSET(g_enodeb_server.server_socket, &read_set)) {
			downlink_data_transfer();
		}
	}
	return NULL;
}

void* ping_traffic(void *arg){
	string cmd;
	
	cmd = "ping -I 172.16.1.3 172.16.0.2 -c 290 | grep \"^rtt\" >> ping.txt";
	
	cout << cmd << endl;
	system(cmd.c_str());
}

void uplink_data_transfer(){

	g_enodeb.read_tun();
	g_enodeb.set_ue_num();
	if(!g_enodeb.success){
		return;
	}

	// if(g_ran_data[g_enodeb.ue_num].valid == true){
		g_enodeb.make_uplink_data();
		g_enodeb.send_sgw();
	// }
}

void downlink_data_transfer(){

	g_enodeb.recv_sgw();
	g_enodeb.set_metadata();
	// if(g_ran_data[g_enodeb.ue_num].valid == true){
		g_enodeb.make_downlink_data();
		g_enodeb.write_tun();		
	// }
}

void* generate_traffic(void *arg) {
	int ue_num;
	int status;
	bool time_exceeded;
	
	ue_num = *((int*)arg);
	time_exceeded = false;
	UE ue(ue_num);
	ue.set_mme_details();
	ue.startup_mme_client();

	int count = 1;
	while(1){
		time_check(g_start_time, g_req_duration, time_exceeded);
		if (time_exceeded) {
			break;
		}	
		
		authenticate(ue);
		if(!ue.success)
			continue;

		setup_tunnel(ue);
		if(!ue.success)
			continue;
		
		if(count == 1 && ue.ue_num == 0){
			count++;
			status = pthread_create(&g_ping_tid, NULL, ping_traffic, NULL);
			report_error(status);
		}
		
		send_traffic(ue);

		/*detach(ue);
		if(!ue.success)
			continue;*/
	}
		
	return NULL;
}

void authenticate(UE &ue) {

	ue.send_attach_req();
	ue.recv_autn_req();
	if(!ue.success){
		return;
	}
	ue.set_autn_res();
	ue.send_autn_res();
	ue.recv_autn_check();
	if(!ue.success){
		return;
	}
}

void setup_tunnel(UE &ue){

	set_tun_data(ue);
	ue.send_tun_data();
	ue.set_sgw_details();
	ue.recv_tun_data();
	if(!ue.success){
		return;
	}
	ue.add_map_entry();
}

void set_tun_data(UE &ue){
	int status;

	status = pthread_mutex_lock(&g_arr_lock);
	report_error(status, "Error in thread locking");

	g_ran_data[ue.ue_num].enodeb_uteid = g_enodeb.generate_uteid(ue.ue_num);

	status = pthread_mutex_unlock(&g_arr_lock);
	report_error(status, "Error in thread unlocking");	
}

void send_traffic(UE &ue) {

	ue.turn_up_interface();
	ue.setup_sink();
	ue.send_iperf3_traffic();
	ue.turn_down_interface();
}

void detach(UE &ue) {

	ue.send_detach_req();
	ue.recv_detach_res();
	if(ue.success == 1){
		ue.delete_session_data();
	}
	else{
		return;
	}
}

void startup_ran(char *argv[]) {

	g_start_time = time(0);
	g_total_connections = atoi(argv[1]);
	g_req_duration = atof(argv[2]);
	g_ue_num.resize(g_total_connections);
	g_tid.resize(g_total_connections);
	g_enodeb_server.bind_server(g_enodeb_port, g_enodeb_addr.c_str());
	g_enodeb_server.print_status("ENODEB");
}

int main(int argc, char *argv[]) {
	int status;
	int i;
	
	check_client_usage(argc, argv);
	startup_ran(argv);
	setup_ran_data();
	setup_tun();
	
	status = pthread_create(&g_mon_tid, NULL, monitor_traffic, NULL);
	report_error(status);
	
	for (i = 0; i < g_total_connections; i++) {
		g_ue_num[i] = i;
		status = pthread_create(&g_tid[i], NULL, generate_traffic, &g_ue_num[i]);
		report_error(status);
	}
	for (i = 0; i < g_total_connections; i++) {
		pthread_join(g_tid[i],NULL);
	}
	
	free_ran_data();
	cout << "Requested duration has ended. Finishing the program." << endl;

	g_run_dur = difftime(time(0), g_start_time);
	cout << "Total number of registrations is " << g_total_regs << endl;
	cout << "Total time for registrations is " << g_total_regstime << endl;
	cout << "Latency is " << (((double)g_total_regstime/g_total_regs) * 1e-6) << endl;
	cout << "Throughput is " << ((double)g_total_regs/g_run_dur) << endl;
	return 0;
}
