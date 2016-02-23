#include "sink_monitor.h"

UDPServer g_public_sink_server;

SinkMonitor::SinkMonitor() {


}

void SinkMonitor::attach_to_tun() {	
	struct ifreq ifr;
	const char *dev = "/dev/net/tun";
	int flags;
	int status;

	tun_name = "tun1";
	flags = (IFF_TUN | IFF_NO_PI);

	tun_fd = open(dev , O_RDWR);
	report_error(tun_fd, "Opening /dev/net/tun");
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_flags = flags;
	if (tun_name.size() != 0) {
		strncpy(ifr.ifr_name, tun_name.c_str(), IFNAMSIZ);
	}
	status = ioctl(tun_fd, TUNSETIFF, (void*)&ifr);
	if (status<0) {
		cout << "ioctl(TUNSETIFF)" << " " << errno << endl;
		close(tun_fd);
		exit(-1);
	}
	tun_name.assign(ifr.ifr_name);
	cout << "Sink Monitor attached to TUN device" << endl;
}

void SinkMonitor::setup_topgwdlink(){

	to_pgw_dlink.bind_client();
}

void SinkMonitor::read_pgw(){

	pkt.clear_data();
	status = recvfrom(g_public_sink_server.server_socket, pkt.data, BUF_SIZE, 0, (sockaddr*)&client_sock_addr, &g_addr_len);
	report_error(status);
	pkt.data_len = status;
	pkt.curr_pos = 0;
}

void SinkMonitor::make_uplink_data(){


	// Nothing to be done on the packet received in order to forward it to the private sink
}

void SinkMonitor::write_tun() {

	status = write(tun_fd, pkt.data, pkt.data_len);
	report_error(status);
}


void SinkMonitor::read_tun() {

	pkt.clear_data();
	status = read(tun_fd, pkt.data, BUF_SIZE);
	report_error(status);
	pkt.data_len = status;
	pkt.curr_pos = 0;
}

void SinkMonitor::make_downlink_data(){


	// Nothing to be done on the packet read in order to forward it to the PGW_Dlink
}

void SinkMonitor::send_pgw_dlink(){

	to_pgw_dlink.set_server_details(g_pgw_dlink_port, g_pgw_dlink_addr.c_str());
	to_pgw_dlink.pkt.copy_frompkt(pkt);
	to_pgw_dlink.write_data();
}

SinkMonitor::~SinkMonitor() {


}