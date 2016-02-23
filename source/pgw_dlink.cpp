#include "pgw_dlink.h"

UDPServer g_pgw_dlink_server;

PGWDlink::PGWDlink(){


}

void PGWDlink::setup_tosgw(){

	to_sgw.bind_client();
}

void PGWDlink::read_data(){

	pkt.clear_data();
	status = recvfrom(g_pgw_dlink_server.server_socket, pkt.data, BUF_SIZE, 0, (sockaddr*)&client_sock_addr, &g_addr_len);
	report_error(status);
	pkt.data_len = status;
	pkt.curr_pos = 0;
}

void PGWDlink::set_ue_num(){
	struct ip *iphdr = allocate_ip_mem(IP_LEN);
	char *ip_addr = allocate_str_mem(INET_ADDRSTRLEN);

	success = 1;
	memmove(iphdr, pkt.data, IP_LEN * sizeof(uint8_t));
	inet_ntop(AF_INET, &(iphdr->ip_dst), ip_addr, INET_ADDRSTRLEN);
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

void PGWDlink::make_downlink_data(){

	int type = 2;
	int subtype = 2;

	to_sgw.pkt.add_metadata(type, subtype, ue_num);

	status = pthread_mutex_lock(&g_arr_lock);
	report_error(status, "Error in thread locking");

	to_sgw.pkt.add_gtpu_hdr(g_pgw_data[ue_num].sgw_uteid);

	status = pthread_mutex_unlock(&g_arr_lock);
	report_error(status, "Error in thread unlocking");

	to_sgw.pkt.add_data(pkt.data, pkt.data_len);
}

void PGWDlink::send_sgw(){

	status = pthread_mutex_lock(&g_arr_lock);
	report_error(status, "Error in thread locking");

	to_sgw.set_server_details(g_pgw_data[ue_num].sgw_port, g_pgw_data[ue_num].sgw_addr.c_str());

	status = pthread_mutex_unlock(&g_arr_lock);
	report_error(status, "Error in thread unlocking");	
	
	to_sgw.write_data();
}

PGWDlink::~PGWDlink(){


}