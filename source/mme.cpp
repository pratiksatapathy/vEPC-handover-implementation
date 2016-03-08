#include "mme.h"

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
	int num_autn_vectors;

	num_autn_vectors = 1;
	pkt.extract_item(imsi);
	pkt.extract_item(ksi_asme);
	pkt.extract_item(network_capability);
	add_ue_context(pkt);
	pkt.clear_pkt();
	pkt.append_item(imsi);
	pkt.append_item(plmn_id);
	pkt.append_iten(num_autn_vectors);
	pkt.append_item("nil");
	pkt.prepend_diameter_hdr(1, pkt.len);
	to_hss.conn(g_hss_ip_addr.c_str(), g_hss_port);
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

Mme::~Mme() {

}
