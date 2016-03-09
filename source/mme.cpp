#include "mme.h"

void UeContext::init() {
	emm_state = arg_emm_state;	 
	ecm_state = arg_ecm_state;
	imsi = arg_imsi;
	ip_addr = arg_ip_addr;
	enodeb_s1ap_id = arg_enodeb_s1ap_id;
	mme_s1ap_id = arg_mme_s1ap_id;
	tai = arg_tai;
	tai_list = arg_tai_list;
	k_asme = arg_k_asme;
	default_apn = arg_default_apn;
	apn_in_use = arg_apn_in_use;
	eps_bearer_id = arg_eps_bearer_id;
	s1_teid_ul = arg_s1_teid_ul;
	s1_teid_dl = arg_s1_teid_dl;
	s5_teid_ul = arg_s5_teid_ul;
	s5_teid_dl = arg_s5_teid_dl;
	xres = arg_xres;	
}

MmeIds::MmeIds() {
	mcc = 1;
	mnc = 1;
	plmn_id = get_plmn_id(mcc, mnc);
	mmegi = 1;
	mmec = 1;
	mmei = get_mmei(mmegi, mmec);
	gummei = get_gummei(plmn_id, mmei);
}

MmeIds::~MmeIds() {
	
}

Mme::Mme() {
	ue_count = 0;
	pthread_mutex_init(&ue_identification_table_mux, NULL);
	pthread_mutex_init(&ue_context_table_mux, NULL);	
}

void Mme::handle_type1_attach(Packet &pkt) {
	SctpClient to_hss;
	uint64_t imsi;
	uint64_t ksi_asme;
	uint16_t network_capability;
	uint64_t rand;
	uint64_t autn;
	uint64_t xres;
	uint64_t k_asme;
	uint32_t enodeb_s1ap_id;
	uint32_t mme_s1ap_id;
	int num_autn_vectors;

	num_autn_vectors = 1;
	pkt.extract_item(imsi);
	pkt.extract_item(ksi_asme); /* No use in this case */
	pkt.extract_item(network_capability); /* No use in this case */

	enodeb_s1ap_id = pkt.s1ap_hdr.enodeb_ue_id;
	

	guti = get_guti(gummei, imsi);
	mlock(1);
	ue_count++;
	mme_s1ap_id = ue_count;
	table1[mme_s1ap_id] = guti;
	munlock(1);
	mlock(2);
	table2[guti].
	munlock(2);
	

	pkt.clear_pkt();
	pkt.append_item(imsi);
	pkt.append_item(plmn_id);
	pkt.append_iten(num_autn_vectors);
	pkt.append_item("nil");
	pkt.prepend_diameter_hdr(1, pkt.len);
	to_hss.conn(g_hss_ip_addr.c_str(), g_hss_port);
	to_hss.snd(pkt);
	to_hss.rcv(pkt);
}

void Mme::add_ue_context(uint64_t guti, ) {
	uint32_t mme_s1ap_id;
	uint64_t guti;
	int status;

	ue_count++;
	mme_s1ap_id = ue_count;
	guti = get_guti()
	status = pthread_mutex_lock(&ue_identification_table_mux);
	handle_type1_error(status, "Lock error: mme_add_ue_context")

	status = pthread_mutex_unlock(&ue_identification_table_mux);
	handle_type1_error(status, "Unlock error: mme_add_ue_context")

}

void Mme::handle_authentication(Packet &pkt) {

}

void Mme::mlock(int arg) {
	int status;

	switch (arg) {
		case 1:
			status = pthread_mutex_lock(&table1_mux);
			handle_type1_error(status, "Lock error: mme_mlock");
			break;
		case 2:
			status = pthread_mutex_lock(&table2_mux);
			handle_type1_error(status, "Lock error: mme_mlock");
			break;			
	}
}

void Mme::munlock(int arg) {
	int status;

	switch (arg) {
		case 1:
			status = pthread_mutex_unlock(&table1_mux);
			handle_type1_error(status, "Unlock error: mme_munlock");
			break;
		case 2:
			status = pthread_mutex_unlock(&table2_mux);
			handle_type1_error(status, "Unlock error: mme_munlock");
			break;		
	}
}

Mme::~Mme() {

}
