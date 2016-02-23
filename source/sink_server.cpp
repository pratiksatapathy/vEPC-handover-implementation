#include "sink_server.h"

SinkMonitor g_sink_monitor;

int g_total_connections;
vector<int> g_conn_num;
pthread_t g_mon_tid;
vector<pthread_t> g_tid;

void setup_interface() {
	string cmd;

	cmd = "sudo ifconfig eth0:0 172.16.0.2/8";
	cout << cmd << endl;

	system(cmd.c_str());
	cout << "Interface successfullly created for Sink" << endl;
}

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

	g_sink_monitor.attach_to_tun();
	g_sink_monitor.setup_topgwdlink();
	max_fd = max(g_sink_monitor.tun_fd, g_public_sink_server.server_socket);

	while (1) {
		FD_ZERO(&read_set);
		FD_SET(g_sink_monitor.tun_fd, &read_set); 
		FD_SET(g_public_sink_server.server_socket, &read_set); 

		status = select(max_fd + 1, &read_set, NULL, NULL, NULL);
		report_error(status, "Select-process failure\tTry again");		

		if (FD_ISSET(g_public_sink_server.server_socket, &read_set)) {
			uplink_data_transfer();
		}
		else if (FD_ISSET(g_sink_monitor.tun_fd, &read_set)) {
			downlink_data_transfer();
		}
	}
	return NULL;
}

void uplink_data_transfer(){
	
	g_sink_monitor.read_pgw();
	g_sink_monitor.make_uplink_data();
	g_sink_monitor.write_tun();
}

void downlink_data_transfer(){

	g_sink_monitor.read_tun();
	g_sink_monitor.make_downlink_data();
	g_sink_monitor.send_pgw_dlink();
}

void* server_iperf3(void *arg) {
	string cmd;
	int conn_num;
	int port;

	conn_num = *((int*)arg);
	port = (conn_num + 55000);

	cmd = "iperf3 -s -B " + g_private_sink_addr + " -p " + to_string(port);
	cout << cmd << endl;
	
	system(cmd.c_str());
}

void startup_sink(char *argv[]) {

	g_total_connections = atoi(argv[1]);
	g_conn_num.resize(g_total_connections);
	g_tid.resize(g_total_connections);
	g_public_sink_server.bind_server(g_public_sink_port, g_public_sink_addr.c_str());
	g_public_sink_server.print_status("PUBLIC SINK");
}

int main(int argc, char *argv[]) {
	int status;
	int i;

	check_server_usage(argc, argv);
	startup_sink(argv);
	setup_interface();
	setup_tun();

	status = pthread_create(&g_mon_tid, NULL, monitor_traffic, NULL);
	report_error(status);	

	for (i = 0; i < g_total_connections; i++) {
		g_conn_num[i] = i;
		status = pthread_create(&g_tid[i], NULL, server_iperf3, &g_conn_num[i]);
		report_error(status);	
	}
	for (i = 0; i < g_total_connections; i++) {
		pthread_join(g_tid[i], NULL);
	}

	return 0;
}
