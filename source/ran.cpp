#include "ran.h"

RanContext::RanContext() {
	emm_state = 0; 
	ecm_state = 0; 
	imsi = 0; 
	guti = 0; 
	ip_addr = "";
	enodeb_s1ap_ue_id = 0; 
	mme_s1ap_ue_id = 0; 
	tai = 1; 
	tau_timer = 0;
	key = 0; 
	k_asme = 0; 
	ksi_asme = 7; 
	k_nas_enc = 0; 
	k_nas_int = 0; 
	nas_enc_algo = 0; 
	nas_int_algo = 0; 
	count = 1;
	bearer = 0;
	dir = 0;
	apn_in_use = 0; 
	eps_bearer_id = 0; 
	e_rab_id = 0; 
	s1_uteid_ul = 0; 
	s1_uteid_dl = 0; 
	mcc = 1; 
	mnc = 1; 
	plmn_id = g_telecom.get_plmn_id(mcc, mnc);
	msisdn = 0; 
	nw_capability = 1;
}

void RanContext::init(uint32_t arg) {
	enodeb_s1ap_ue_id = arg;
	key = arg;
	msisdn = 9000000000 + arg;
	imsi = g_telecom.get_imsi(plmn_id, msisdn);
}

RanContext::~RanContext() {

}

EpcAddrs::EpcAddrs() {
	mme_port = g_mme_port;
	sgw_s1_port = 0;
	mme_ip_addr = g_mme_ip_addr;	
	sgw_s1_ip_addr = "";
}

EpcAddrs::~EpcAddrs() {

}

void UplinkInfo::init(uint32_t arg_s1_uteid_ul, string arg_sgw_s1_ip_addr, uint64_t arg_sgw_s1_port) {
	s1_uteid_ul = arg_s1_uteid_ul;
	sgw_s1_ip_addr = arg_sgw_s1_ip_addr;
	sgw_s1_port = arg_sgw_s1_port;
}

TrafficMonitor::TrafficMonitor() {
	g_sync.mux_init(uplinkinfo_mux);
}

void TrafficMonitor::handle_uplink_udata() {	
	Packet pkt;
	string ip_addr;
	uint32_t s1_uteid_ul;
	string sgw_s1_ip_addr;
	uint64_t sgw_s1_port;
	bool res;

	tun.rcv(pkt);
	ip_addr = g_nw.get_src_ip_addr(pkt);
	res = get_uplink_info(ip_addr, s1_uteid_ul, sgw_s1_ip_addr, sgw_s1_port);
	if (res == true) {
		UdpClient sgw_s1_client;

		sgw_s1_client.conn(sgw_s1_ip_addr.c_str(), sgw_s1_port);
		pkt.prepend_s1ap_hdr(1, 1, pkt.len, s1_uteid_ul);
		sgw_s1_client.snd(pkt);
	}
}

void TrafficMonitor::handle_downlink_udata() {
	Packet pkt;
	struct sockaddr_in src_sock_addr;

	server.rcv(src_sock_addr, pkt);
	pkt.extract_gtp_hdr();
	pkt.truncate();
	tun.snd(pkt);
}

void TrafficMonitor::update_uplink_info(string ip_addr, uint32_t s1_uteid_ul, string sgw_s1_ip_addr, uint64_t sgw_s1_port) {
	g_sync.mlock(uplinkinfo_mux);
	uplink_info[ip_addr].init(s1_uteid_ul, sgw_s1_ip_addr, sgw_s1_port);
	g_sync.munlock(uplinkinfo_mux);
}

bool TrafficMonitor::get_uplink_info(string ip_addr, uint32_t &s1_uteid_ul, string &sgw_s1_ip_addr, uint64_t &sgw_s1_port) {
	bool res = false;

	g_sync.mlock(uplinkinfo_mux);
	if (uplink_info.find(ip_addr) != uplink_info.end()) {
		res = true;
		s1_uteid_ul = uplink_info[ip_addr].s1_uteid_ul;
		sgw_s1_ip_addr = uplink_info[ip_addr].sgw_s1_ip_addr;
		sgw_s1_port = uplink_info[ip_addr].sgw_s1_port;
	}
	g_sync.munlock(uplinkinfo_mux);
	return res;
}

TrafficMonitor::~TrafficMonitor() {

}

Ran::Ran(){
	mme_client.conn(epc_addrs.mme_ip_addr.c_str(), epc_addrs.mme_port);
	crypt.load();
}

void Ran::init(int arg) {
	ran_ctx.init(arg);
}

void Ran::initial_attach() {
	pkt.clear_pkt();
	pkt.append_item(ran_ctx.imsi);
	pkt.append_item(ran_ctx.tai);
	pkt.append_item(ran_ctx.ksi_asme);
	pkt.append_item(ran_ctx.nw_capability);
	pkt.prepend_s1ap_hdr(1, pkt.len, ran_ctx.enodeb_s1ap_ue_id, 0);
	mme_client.snd(pkt);
	cout << "ran_initialattach:" << " request sent for ran - " << ran_ctx.key << endl;
}

void Ran::authenticate() {
	uint64_t autn_num;
	uint64_t xautn_num;
	uint64_t rand_num;
	uint64_t sqn;
	uint64_t res;
	uint64_t ck;
	uint64_t ik;

	mme_client.rcv(pkt);
	cout << "ran_authenticate: " << " received request for ran - " << ran_ctx.key << endl;
	pkt.extract_s1ap_hdr();
	ran_ctx.mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;
	pkt.extract_item(xautn_num);
	pkt.extract_item(rand_num);
	pkt.extract_item(ran_ctx.ksi_asme);
	
	cout << "ran_authenticate: " << ran_ctx.key << " autn: " << xautn_num << " rand: " << rand_num << " ksiasme: " << ran_ctx.ksi_asme << endl;
	sqn = rand_num + 1;
	res = ran_ctx.key + sqn + rand_num;
	autn_num = res + 1;
	if (autn_num != xautn_num) {
		g_utils.handle_type1_error(-1, "authentication of MME failed error: ran_authenticate");
	}
	ck = res + 2;
	ik = res + 3;
	ran_ctx.k_asme = ck + ik + sqn + ran_ctx.plmn_id;
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_s1ap_hdr(2, pkt.len, ran_ctx.enodeb_s1ap_ue_id, ran_ctx.mme_s1ap_ue_id);
	mme_client.snd(pkt);
	cout << "ran_authenticate:" << " authentication sucessful for RAN - " << ran_ctx.key << endl;
}

void Ran::set_security() {
	uint8_t *hmac_res;
	uint8_t *hmac_xres;
	bool res;

	hmac_res = g_utils.allocate_uint8_mem(integrity.hmac_len);
	hmac_xres = g_utils.allocate_uint8_mem(integrity.hmac_len);
	mme_client.rcv(pkt);
	cout << "ran_setsecurity: " << " received request for ran - " << ran_ctx.key << endl;
	pkt.extract_s1ap_hdr();
	integrity.rem_hmac(pkt, hmac_xres);
	pkt.extract_item(ran_ctx.ksi_asme);
	pkt.extract_item(ran_ctx.nw_capability);
	pkt.extract_item(ran_ctx.nas_enc_algo);
	pkt.extract_item(ran_ctx.nas_int_algo);
	set_crypt_context();
	set_integrity_context();
	integrity.get_hmac(pkt.data, pkt.len, hmac_res, ran_ctx.k_nas_int);
	res = integrity.cmp_hmacs(hmac_res, hmac_xres);
	if (res == false) {
		g_utils.handle_type1_error(-1, "hmac initial security failure error: ran_setsecurity");
	}
	cout << "ran_setsecurity:" << " security mode command success" << endl;

	pkt.clear_pkt();
	pkt.append_item(res);
	crypt.enc(pkt, ran_ctx.k_nas_enc);
	integrity.add_hmac(pkt, ran_ctx.k_nas_int);
	pkt.prepend_s1ap_hdr(4, pkt.len, pkt.s1ap_hdr.enodeb_s1ap_ue_id, pkt.s1ap_hdr.mme_s1ap_ue_id);
	mme_client.snd(pkt);
	cout << "ran_setsecurity:" << " security mode complete success" << endl;
	free(hmac_res);
	free(hmac_xres);
}

void Ran::set_crypt_context() {
	ran_ctx.k_nas_enc = ran_ctx.k_asme + ran_ctx.nas_enc_algo + ran_ctx.count + ran_ctx.bearer + ran_ctx.dir + 1;
}

void Ran::set_integrity_context() {
	ran_ctx.k_nas_int = ran_ctx.k_asme + ran_ctx.nas_int_algo + ran_ctx.count + ran_ctx.bearer + ran_ctx.dir + 1;
}

void Ran::set_eps_session(TrafficMonitor &traf_mon) {
	bool res;
	int tai_list_size;
	uint64_t k_enodeb;

	mme_client.rcv(pkt);
	cout << "ran_setepssession:" << " attach accept received from mme" << endl;
	pkt.extract_s1ap_hdr();
	res = integrity.hmac_check(pkt, ran_ctx.k_nas_int);
	if (res == false) {
		g_utils.handle_type1_error(-1, "hmac attach accept failure error: ran_setepssession");
	}
	crypt.dec(pkt, ran_ctx.k_nas_enc);
	pkt.extract_item(ran_ctx.eps_bearer_id);
	pkt.extract_item(ran_ctx.e_rab_id);
	pkt.extract_item(ran_ctx.s1_uteid_ul);
	pkt.extract_item(k_enodeb);
	pkt.extract_item(ran_ctx.nw_capability);
	pkt.extract_item(tai_list_size);
	pkt.extract_item(ran_ctx.tai_list, tai_list_size);
	pkt.extract_item(ran_ctx.tau_timer);
	pkt.extract_item(ran_ctx.ip_addr);
	pkt.extract_item(epc_addrs.sgw_s1_ip_addr);
	pkt.extract_item(epc_addrs.sgw_s1_port);
	pkt.extract_item(res);
	if (res == false) {
		g_utils.handle_type1_error(-1, "attach request failure error: ran_setepssession");	
	}	
	traf_mon.update_uplink_info(ran_ctx.ip_addr, ran_ctx.s1_uteid_ul, epc_addrs.sgw_s1_ip_addr, epc_addrs.sgw_s1_port);
	ran_ctx.s1_uteid_dl = ran_ctx.s1_uteid_ul;
	pkt.clear_pkt();
	pkt.append_item(ran_ctx.eps_bearer_id);
	pkt.append_item(ran_ctx.s1_uteid_dl);
	crypt.enc(pkt, ran_ctx.k_nas_enc);
	integrity.add_hmac(pkt, ran_ctx.k_nas_int);
	pkt.prepend_s1ap_hdr(6, pkt.len, pkt.s1ap_hdr.enodeb_s1ap_ue_id, pkt.s1ap_hdr.mme_s1ap_ue_id);
	mme_client.snd(pkt);
	cout << "ran_setepssession:" << " attach complete sent to mme" << endl;
	ran_ctx.emm_state = 1;
	ran_ctx.ecm_state = 1;
}

void Ran::transfer_data() {
	string cmd;
	string rate;	
	string mtu;
	string dur;
	string server_ip_addr;
	uint64_t server_port;

	rate = " -b 1M";
	mtu = " -M 500";
	dur = " -t 1";
	server_ip_addr = "172.16.0.2";
	server_port = ran_ctx.key + 55000;
	g_nw.add_itf(ran_ctx.key, ran_ctx.ip_addr + "/8");
	cmd = "iperf3 -B " + ran_ctx.ip_addr + " -c " + server_ip_addr + " -p " + to_string(server_port) + rate + mtu + dur;	
	system(cmd.c_str());
	g_nw.rem_itf(ran_ctx.key);
	cout << "ran_transferdata:" << " transfer done for ran:" << ran_ctx.key << endl;
}

void Ran::detach() {
	uint64_t detach_type;
	bool res;

	detach_type = 1;
	pkt.clear_pkt();
	pkt.append_item(ran_ctx.guti);
	pkt.append_item(ran_ctx.ksi_asme);
	pkt.append_item(detach_type);
	crypt.enc(pkt, ran_ctx.k_nas_enc);
	integrity.add_hmac(pkt, ran_ctx.k_nas_int);
	pkt.prepend_s1ap_hdr(7, pkt.len, ran_ctx.enodeb_s1ap_ue_id, ran_ctx.mme_s1ap_ue_id);
	mme_client.snd(pkt);
	cout << "ran_detach:" << " detach request sent to mme" << endl;
	mme_client.rcv(pkt);
	cout << "ran_detach:" << " detach complete received from mme" << endl;
	pkt.extract_s1ap_hdr();
	res = integrity.hmac_check(pkt, ran_ctx.k_nas_int);
	if (res == false) {
		g_utils.handle_type1_error(-1, "HMAC detach failure error: ran_detach");
	}
	crypt.dec(pkt, ran_ctx.k_nas_enc);
	pkt.extract_item(res);
	if (res == false) {
		g_utils.handle_type1_error(-1, "detach failure error: ran_detach");	
	}
	cout << "ran_detach:" << " detach successful" << endl;
}

Ran::~Ran(){

}

