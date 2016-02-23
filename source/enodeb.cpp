#include "enodeb.h"

UDPServer g_enodeb_server;

EnodeB::EnodeB() {
	
}

uint16_t EnodeB::generate_uteid(int ue_num) {
	
	return ue_num;
}

void EnodeB::attach_to_tun() {	
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
	cout << "Enodeb attached to tun device - " << tun_name << endl;
}

void EnodeB::setup_tosgw(){

	to_sgw.bind_client();
}

void EnodeB::read_tun() {

	pkt.clear_data();
	status = read(tun_fd, pkt.data, BUF_SIZE);
	report_error(status, "Error in reading from TUN device");
	pkt.data_len = status;
	pkt.curr_pos = 0;
}

void EnodeB::set_ue_num(){
	struct ip *iphdr = allocate_ip_mem(IP_LEN);
	char *ip_addr = allocate_str_mem(INET_ADDRSTRLEN);

	success = 1;
	memmove(iphdr, pkt.data, IP_LEN * sizeof(uint8_t));
	inet_ntop(AF_INET, &(iphdr->ip_src), ip_addr, INET_ADDRSTRLEN);
	ue_ip.assign(ip_addr);
	
	status = pthread_mutex_lock(&g_map_lock);
	report_error(status, "Error in thread locking");

	if(g_ue_maptable.find(ue_ip) == g_ue_maptable.end()){
		success = 0;
	}
	else{
		ue_num = g_ue_maptable[ue_ip];
	}

	status = pthread_mutex_unlock(&g_map_lock);
	report_error(status, "Error in thread unlocking");

	free(iphdr);
	free(ip_addr);
}

void EnodeB::make_uplink_data(){

	type = 2;
	subtype = 1;
	
	to_sgw.pkt.add_metadata(type, subtype, ue_num);

	status = pthread_mutex_lock(&g_arr_lock);
	report_error(status, "Error in thread locking");

	to_sgw.pkt.add_gtpu_hdr(g_ran_data[ue_num].sgw_uteid);

	status = pthread_mutex_unlock(&g_arr_lock);
	report_error(status, "Error in thread unlocking");

	to_sgw.pkt.add_data(pkt.data, pkt.data_len);
}

void EnodeB::send_sgw(){

	status = pthread_mutex_lock(&g_arr_lock);
	report_error(status, "Error in thread locking");

	to_sgw.set_server_details(g_ran_data[ue_num].sgw_port, g_ran_data[ue_num].sgw_addr.c_str());

	status = pthread_mutex_unlock(&g_arr_lock);
	report_error(status, "Error in thread unlocking");

	to_sgw.write_data();
}

void EnodeB::recv_sgw(){

	pkt.clear_data();
	status = recvfrom(g_enodeb_server.server_socket, pkt.data, BUF_SIZE, 0, (sockaddr*)&client_sock_addr, &g_addr_len);
	report_error(status);
	pkt.data_len = status;
	pkt.curr_pos = 0;
}

void EnodeB::set_metadata(){

	pkt.copy_metadata(type, subtype, ue_num);
}

void EnodeB::make_downlink_data(){
	Packet temp;
	int len;

	pkt.copy_gtpu_hdr();
	
	len = (pkt.data_len - pkt.curr_pos);
	temp.clear_data();
	pkt.copy_data(temp.data, len);
	temp.data_len = len;
	temp.curr_pos = 0;
	temp.copy_topkt(pkt);
}

void EnodeB::write_tun() {

	status = write(tun_fd, pkt.data, pkt.data_len);
	report_error(status, "Error in writing to TUN device");
}

EnodeB::~EnodeB() {

}
