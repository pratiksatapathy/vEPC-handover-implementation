#include "ran.h"

RanContext::RanContext() {
	tai = 1;
	ksi_asme = 7;
	mcc = 1;
	mnc = 1;
	plmn_id = get_plmn_id(mcc, mnc);
	nw_capability = 1;
	count = 1;
	bearer = 0;
	dir = 0;	
}

void RanContext::init(uint32_t arg) {
	enodeb_s1ap_ue_id = arg;
	key = arg;
	msisdn = 9000000000 + arg;
	imsi = get_imsi(plmn_id, msisdn);
}

RanContext::~RanContext() {

}

EpcAddrs::EpcAddrs() {
	mme_port = g_mme_port;
	mme_ip_addr = g_mme_ip_addr;	
}

EpcAddrs::~EpcAddrs() {

}


Ran::Ran(){
	mme_client.conn(epc_addrs.mme_ip_addr.c_str(), epc_addrs.mme_port);
	crypt.load();
}

void Ran::init(int arg) {
	ran_context.init(arg);
}

void Ran::initial_attach() {
	pkt.clear_pkt();
	pkt.append_item(ran_context.imsi);
	pkt.append_item(ran_context.tai);
	pkt.append_item(ran_context.ksi_asme);
	pkt.append_item(ran_context.nw_capability);
	pkt.prepend_s1ap_hdr(1, pkt.len, ran_context.enodeb_s1ap_ue_id, 0);
	mme_client.snd(pkt);
	cout << "ran_initialattach:" << " request sent for ran - " << ran_context.key << endl;
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
	cout << "ran_authenticate: " << " received request for ran - " << ran_context.key << endl;
	pkt.extract_s1ap_hdr();
	ran_context.mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;
	pkt.extract_item(xautn_num);
	pkt.extract_item(rand_num);
	pkt.extract_item(ran_context.ksi_asme);
	
	cout << "ran_authenticate: " << ran_context.key << " autn: " << xautn_num << " rand: " << rand_num << " ksiasme: " << ran_context.ksi_asme << endl;
	sqn = rand_num + 1;
	res = ran_context.key + sqn + rand_num;
	autn_num = res + 1;
	if (autn_num != xautn_num) {
		handle_type1_error(-1, "Authentication of MME failed error: ran_authenticate");
	}
	ck = res + 2;
	ik = res + 3;
	ran_context.k_asme = ck + ik + sqn + ran_context.plmn_id;
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_s1ap_hdr(2, pkt.len, ran_context.enodeb_s1ap_ue_id, ran_context.mme_s1ap_ue_id);
	mme_client.snd(pkt);
	cout << "ran_authenticate:" << " Authentication sucessful for RAN - " << ran_context.key << endl;
}

void Ran::setup_security() {
	uint8_t *hmac_res;
	uint8_t *hmac_xres;
	bool res;

	hmac_res = allocate_uint8_mem(integrity.hmac_len);
	hmac_xres = allocate_uint8_mem(integrity.hmac_len);
	mme_client.rcv(pkt);
	cout << "ran_setupsecurity: " << " received request for ran - " << ran_context.key << endl;
	pkt.extract_s1ap_hdr();
	integrity.rem_hmac(pkt, hmac_xres);
	pkt.extract_item(ran_context.ksi_asme);
	pkt.extract_item(ran_context.nw_capability);
	pkt.extract_item(ran_context.nas_enc_algo);
	pkt.extract_item(ran_context.nas_int_algo);
	setup_crypt_context();
	setup_integrity_context();
	integrity.get_hmac(pkt.data, pkt.len, hmac_res, ran_context.k_nas_int);
	res = integrity.cmp_hmacs(hmac_res, hmac_xres);
	if (res == false) {
		handle_type1_error(-1, "HMAC of initial security msg failed: ran_setupsecurity");
	}
	cout << "ran_setupsecurity:" << " security mode command success" << endl;

	pkt.clear_pkt();
	pkt.append_item(res);
	crypt.enc(pkt, ran_context.k_nas_enc);
	integrity.add_hmac(pkt, ran_context.k_nas_int);
	pkt.prepend_s1ap_hdr(4, pkt.len, pkt.s1ap_hdr.enodeb_s1ap_ue_id, pkt.s1ap_hdr.mme_s1ap_ue_id);
	mme_client.snd(pkt);
	cout << "ran_setupsecurity:" << " security mode complete success" << endl;
	free(hmac_res);
	free(hmac_xres);
}

void Ran::setup_crypt_context() {
	ran_context.k_nas_enc = ran_context.k_asme + ran_context.nas_enc_algo + ran_context.count + ran_context.bearer + ran_context.dir + 1;
}

void Ran::setup_integrity_context() {
	ran_context.k_nas_int = ran_context.k_asme + ran_context.nas_int_algo + ran_context.count + ran_context.bearer + ran_context.dir + 1;
}

void Ran::setup_eps_session() {

}

void Ran::transfer_data() {

}

void Ran::detach() {

}

Ran::~Ran(){

}
