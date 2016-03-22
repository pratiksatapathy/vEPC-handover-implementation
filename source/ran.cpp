#include "ran.h"

RanContext::RanContext() {
	tai = 1;
	ksi_asme = 7;
	mcc = 1;
	mnc = 1;
	plmn_id = get_plmn_id(mcc, mnc);
	network_capability = 1;
}

void RanContext::init(uint32_t arg) {
	msisdn = arg;
	imsi = get_imsi(plmn_id, msisdn);
	enodeb_s1ap_id = arg;
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
	to_mme.conn(g_mme_ip_addr.c_str(), g_mme_port);
}

void Ran::init(int arg) {
	ran_context.init(arg);
}

void Ran::initial_attach() {
	pkt.clear_pkt();
	pkt.append_item(ran_context.imsi);
	pkt.append_item(ran_context.tai);
	pkt.append_item(ran_context.ksi_asme);
	pkt.append_item(ran_context.network_capability);
	to_mme.snd(pkt);
}

void Ran::authenticate() {
	uint64_t autn_num;
	uint64_t rand_num;

	to_mme.rcv(pkt);
	pkt.extract_item(autn_num);
	pkt.extract_item(rand_num);
	pkt.extract_item(ran_context.ksi_asme);
	
}

void Ran::setup_security_context() {

}

void Ran::setup_eps_session() {

}

void Ran::transfer_data() {

}

void Ran::detach() {

}

Ran::~Ran(){

}
