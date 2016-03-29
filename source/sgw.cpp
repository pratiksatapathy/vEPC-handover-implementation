#include "sgw.h"

UeContext::UeContext() {
	tai = 0; 
	apn_in_use = 0; 
	eps_bearer_id = 0;
	s1_uteid_ul = 0; 
	s1_uteid_dl = 0; 
	s5_uteid_ul = 0; 
	s5_uteid_dl = 0; 
	s11_cteid_mme = 0;
	s11_cteid_sgw = 0;
	s5_cteid_ul = 0;
	s5_cteid_dl = 0;
	pgw_s5_ip_addr = "";
	pgw_s5_port = 0;
}

void UeContext::init(uint64_t arg_tai, uint64_t arg_apn_in_use, uint8_t arg_eps_bearer_id, uint32_t arg_s1_uteid_ul, uint32_t arg_s5_uteid_dl, uint32_t arg_s11_cteid_mme, uint32_t arg_s11_cteid_sgw, uint32_t arg_s5_cteid_dl, string arg_pgw_s5_ip_addr, int arg_pgw_s5_port) {
	tai = arg_tai; 
	apn_in_use = arg_apn_in_use;
	eps_bearer_id = arg_eps_bearer_id;
	s1_uteid_ul = arg_s1_uteid_ul;
	s5_uteid_dl = arg_s5_uteid_dl;
	s11_cteid_mme = arg_s11_cteid_mme;
	s11_cteid_sgw = arg_s11_cteid_sgw;
	s5_cteid_dl = arg_s5_cteid_dl;
	pgw_s5_ip_addr = arg_pgw_s5_ip_addr;
	pgw_s5_port = arg_pgw_s5_port;
}

UeContext::~UeContext() {

}

Sgw::Sgw() {
	g_sync.mux_init(table1_mux);	
	g_sync.mux_init(table234_mux);	
}

void Sgw::handle_create_session(struct sockaddr_in src_sock_addr, Packet pkt) {
	UdpClient pgw_client;
	uint32_t s1_uteid_ul;
	uint32_t s5_uteid_ul;
	uint32_t s5_uteid_dl;
	uint32_t s11_cteid_mme;
	uint32_t s11_cteid_sgw;
	uint32_t s5_cteid_ul;
	uint32_t s5_cteid_dl;
	uint64_t imsi;
	uint8_t eps_bearer_id;
	string pgw_s5_ip_addr;
	string ue_ip_addr;
	int pgw_s5_port;
	uint64_t apn_in_use;
	uint64_t tai;
	int ip_addr_len;

	pkt.append_item(s11_cteid_mme);
	pkt.append_item(imsi);
	pkt.append_item(eps_bearer_id);
	pkt.append_item(pgw_s5_ip_addr);
	pkt.append_item(pgw_s5_port);
	pkt.append_item(apn_in_use);
	pkt.append_item(tai);
	s1_uteid_ul = s11_cteid_mme;
	s5_uteid_dl = s11_cteid_mme;
	s11_cteid_sgw = s11_cteid_mme;
	s5_cteid_dl = s11_cteid_mme;
	
	g_sync.mlock(table1_mux);
	table1[s11_cteid_sgw] = imsi;
	g_sync.munlock(table1_mux);
	
	g_sync.mlock(table234_mux);
	table2[imsi].init(tai, apn_in_use, eps_bearer_id, s1_uteid_ul, s5_uteid_dl, s11_cteid_mme, s11_cteid_sgw, s5_cteid_dl, pgw_s5_ip_addr, pgw_s5_port);
	g_sync.munlock(table234_mux);

	pgw_client.conn(pgw_s5_ip_addr.c_str(), pgw_s5_port);
	ip_addr_len = g_mme_ip_addr.size();
	pkt.clear_pkt();
	pkt.append_item(s5_cteid_dl);
	pkt.append_item(imsi);
	pkt.append_item(eps_bearer_id);
	pkt.append_item(s5_uteid_dl);
	pkt.append_item(apn_in_use);
	pkt.append_item(tai);
	pkt.prepend_gtpc_hdr(1, pkt.len, 0);
	pgw_client.snd(pkt);
	pgw_client.rcv(pkt);
	pkt.extract_gtpc_hdr();
	pkt.extract_item(s5_cteid_ul);
	pkt.extract_item(ue_ip_addr, ip_addr_len);
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(s5_uteid_ul);

	g_sync.mlock(table234_mux);
	table2[imsi].s5_uteid_ul = s5_uteid_ul;
	table2[imsi].s5_cteid_ul = s5_cteid_ul;
	g_sync.munlock(table234_mux);	

	pkt.clear_pkt();
	pkt.append_item(s11_cteid_sgw);
	pkt.append_item(ue_ip_addr);
	pkt.append_item(s1_uteid_ul);
	pkt.append_item(s5_uteid_ul);
	pkt.append_item(s5_uteid_dl);
	pkt.prepend_gtpc_hdr(1, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
}

void Sgw::handle_modify_bearer(struct sockaddr_in src_sock_addr, Packet pkt) {
	uint64_t imsi;
	uint32_t s1_uteid_dl;
	uint32_t s11_cteid_mme;
	uint8_t eps_bearer_id;
	bool res;

	imsi = get_imsi(pkt);
	pkt.append_item(eps_bearer_id);
	pkt.append_item(s1_uteid_dl);
	g_sync.mlock(table234_mux);
	table2[imsi].s1_uteid_dl = s1_uteid_dl;
	s11_cteid_mme = table2[imsi].s11_cteid_mme;
	g_sync.munlock(table234_mux);
	res = true;
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_gtpc_hdr(2, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
}

uint64_t Sgw::get_imsi(Packet pkt) {
	uint64_t imsi;

	g_sync.mlock(table1_mux);
	imsi = table1[pkt.gtpc_hdr.teid];
	g_sync.munlock(table1_mux);
	return imsi;
}

Sgw::~Sgw() {

}