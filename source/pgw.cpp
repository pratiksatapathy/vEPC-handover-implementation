#include "pgw.h"

UeContext::UeContext() {
	ip_addr = "";
	tai = 0; 
	apn_in_use = 0; 
	s5_uteid_ul = 0; 
	s5_uteid_dl = 0; 
	s5_cteid_ul = 0;
	s5_cteid_dl = 0;
}

void UeContext::init(string arg_ip_addr, uint64_t arg_tai, uint64_t arg_apn_in_use, uint8_t arg_eps_bearer_id, uint32_t arg_s5_uteid_ul, uint32_t arg_s5_uteid_dl, uint32_t arg_s5_cteid_ul, uint32_t arg_s5_cteid_dl) {
	ip_addr = arg_ip_addr;
	tai = arg_tai; 
	apn_in_use = arg_apn_in_use; 
	eps_bearer_id = arg_eps_bearer_id; 
	s5_uteid_ul = arg_s5_uteid_ul; 
	s5_uteid_dl = arg_s5_uteid_dl; 
	s5_cteid_ul = arg_s5_cteid_ul;
	s5_cteid_dl = arg_s5_cteid_dl;
}

UeContext::~UeContext() {

}

Pgw::Pgw() {
	set_ip_addrs();
	g_sync.mux_init(s5id_mux);	
	g_sync.mux_init(sgiid_mux);	
	g_sync.mux_init(uectx_mux);	
}

void Pgw::handle_create_session(struct sockaddr_in src_sock_addr, Packet pkt) {
	uint64_t imsi;
	uint8_t eps_bearer_id;
	uint32_t s5_uteid_ul;
	uint32_t s5_uteid_dl;
	uint32_t s5_cteid_ul;
	uint32_t s5_cteid_dl;
	uint64_t apn_in_use;
	uint64_t tai;
	string ue_ip_addr;

	pkt.extract_item(s5_cteid_dl);
	pkt.extract_item(imsi);
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(s5_uteid_dl);
	pkt.extract_item(apn_in_use);
	pkt.extract_item(tai);
	s5_cteid_ul = s5_cteid_dl;
	s5_uteid_ul = s5_cteid_dl;
	ue_ip_addr = ip_addrs[imsi];
	update_itfid(5, s5_uteid_ul, "", imsi);
	update_itfid(0, 0, ue_ip_addr, imsi);
	g_sync.mlock(uectx_mux);
	ue_ctx[imsi].init(ue_ip_addr, tai, apn_in_use, eps_bearer_id, s5_uteid_ul, s5_uteid_dl, s5_cteid_ul, s5_cteid_dl);
	g_sync.munlock(uectx_mux);	
	pkt.clear_pkt();
	pkt.append_item(s5_cteid_ul);
	pkt.append_item(eps_bearer_id);
	pkt.append_item(s5_uteid_ul);
	pkt.append_item(ue_ip_addr);
	pkt.prepend_gtp_hdr(2, 1, pkt.len, s5_cteid_dl);
	s5_server.snd(src_sock_addr, pkt);
	cout << "pgw_handlecreatesession:" << " create session response sent to mme" << endl;
}

void Pgw::handle_uplink_udata(Packet pkt) {
	UdpClient sink_client;

	sink_client.conn(g_sink_ip_addr, g_sink_port);
	sink_client.snd(pkt);
	cout << "pgw_handleuplinkudata:" << " uplink udata forwarded to sink" << endl;
}

void Pgw::handle_downlink_udata(Packet pkt) {
	uint64_t imsi;
	uint32_t s5_uteid_dl;
	string ue_ip_addr;
	bool res;

	ue_ip_addr = g_nw.get_dst_ip_addr(pkt);
	imsi = get_imsi(0, 0, ue_ip_addr);
	res = get_downlink_info(imsi, s5_uteid_dl);
	if (res) {
		UdpClient sgw_s5_client;

		sgw_s5_client.conn(g_sgw_s5_ip_addr, g_sgw_s5_port);
		pkt.prepend_gtp_hdr(1, 3, pkt.len, s5_uteid_dl);
		sgw_s5_client.snd(pkt);
		cout << "pgw_handledownlinkudata:" << " downlink udata forwarded to sgw" << endl;
	}
}

void Pgw::handle_detach(struct sockaddr_in src_sock_addr, Packet pkt) {
	uint64_t imsi;
	uint64_t tai;
	uint32_t s5_cteid_ul;
	uint32_t s5_cteid_dl;
	uint8_t eps_bearer_id;
	string ue_ip_addr;
	bool res;

	res = true;
	imsi = get_imsi(5, pkt.gtp_hdr.teid, "");
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(tai);
	g_sync.mlock(uectx_mux);
	s5_cteid_ul = ue_ctx[imsi].s5_cteid_ul;
	s5_cteid_dl = ue_ctx[imsi].s5_cteid_dl;
	ue_ip_addr = ue_ctx[imsi].ip_addr;
	g_sync.munlock(uectx_mux);
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_gtp_hdr(2, 4, pkt.len, s5_cteid_dl);
	s5_server.snd(src_sock_addr, pkt);
	cout << "pgw_handledetach:" << " detach complete sent to sgw" << endl;
	rem_itfid(5, s5_cteid_ul, "");
	rem_itfid(0, 0, ue_ip_addr);
	rem_uectx(imsi);
	cout << "pgw_handledetach:" << " detach successful" << endl;
}

void Pgw::set_ip_addrs() {
	int i;
	int imsi;
	int subnet;
	int host;
	string prefix;
	string ip_addr;

	prefix = "172.16.";
	subnet = 1;
	host = 3;
	for (i = 0; i < MAX_UE_COUNT; i++) {
		imsi = 119000000000 + i;
		ip_addr = prefix + to_string(subnet) + "." + to_string(host);
		ip_addrs[imsi] = ip_addr;
		if (host == 254) {
			subnet++;
			host = 3;
		}
		else {
			host++;
		}
	}
}

void Pgw::update_itfid(int itf_id_no, uint32_t teid, string ue_ip_addr, uint64_t imsi) {
	switch (itf_id_no) {
		case 5:
			g_sync.mlock(s5id_mux);
			s5_id[teid] = imsi;
			g_sync.munlock(s5id_mux);
			break;
		case 0:
			g_sync.mlock(sgiid_mux);
			sgi_id[ue_ip_addr] = imsi;
			g_sync.munlock(sgiid_mux);		
			break;
		default:
			g_utils.handle_type1_error(-1, "incorrect itf_id_no: pgw_updateitfid");
	}
}

uint64_t Pgw::get_imsi(int itf_id_no, uint32_t teid, string ue_ip_addr) {
	uint64_t imsi;

	imsi = 0;
	switch (itf_id_no) {
		case 5:
			g_sync.mlock(s5id_mux);
			if (s5_id.find(teid) != s5_id.end()) {
				imsi = s5_id[teid];
			}
			g_sync.munlock(s5id_mux);		
			break;
		case 0:
			g_sync.mlock(sgiid_mux);
			if (sgi_id.find(ue_ip_addr) != sgi_id.end()) {
				imsi = sgi_id[ue_ip_addr];
			}
			g_sync.munlock(sgiid_mux);		
			break;
		default:
			g_utils.handle_type1_error(-1, "incorrect itf_id_no: pgw_getimsi");
	}
	return imsi;
}

bool Pgw::get_downlink_info(uint64_t imsi, uint32_t &s5_uteid_dl) {
	bool res = false;

	g_sync.mlock(uectx_mux);
	if (ue_ctx.find(imsi) != ue_ctx.end()) {
		res = true;
		s5_uteid_dl = ue_ctx[imsi].s5_uteid_dl;
	}
	g_sync.munlock(uectx_mux);
	return res;
}

void Pgw::rem_itfid(int itf_id_no, uint32_t teid, string ue_ip_addr) {
	switch (itf_id_no) {
		case 5:
			g_sync.mlock(s5id_mux);
			s5_id.erase(teid);
			g_sync.munlock(s5id_mux);
			break;
		case 0:
			g_sync.mlock(sgiid_mux);
			sgi_id.erase(ue_ip_addr);
			g_sync.munlock(sgiid_mux);		
			break;
		default:
			g_utils.handle_type1_error(-1, "incorrect itf_id_no: pgw_remitfid");
	}
}

void Pgw::rem_uectx(uint64_t imsi) {
	g_sync.mlock(uectx_mux);	
	ue_ctx.erase(imsi);
	g_sync.munlock(uectx_mux);	
}

Pgw::~Pgw() {

}