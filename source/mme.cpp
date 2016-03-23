#include "mme.h"

void UeContext::init(uint64_t arg_imsi, uint32_t arg_enodeb_s1ap_id, uint32_t arg_mme_s1ap_id, uint64_t arg_tai) {
	imsi = arg_imsi;
	enodeb_s1ap_id = arg_enodeb_s1ap_id;
	mme_s1ap_id = arg_mme_s1ap_id;
	tai = arg_tai;
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
	mux_init(table1_mux);
	mux_init(table2_mux);
}

void Mme::handle_type1_attach(int conn_fd, Packet &pkt) {
	SctpClient to_hss;
	uint64_t imsi;
	uint64_t tai;
	uint64_t ksi_asme;
	uint16_t network_capability;
	uint64_t autn_num;
	uint64_t rand_num;
	uint64_t xres;
	uint64_t k_asme;
	uint32_t enodeb_s1ap_id;
	uint32_t mme_s1ap_id;
	uint64_t guti;
	int num_autn_vectors;

	num_autn_vectors = 1;
	pkt.extract_item(imsi);
	pkt.extract_item(tai);
	pkt.extract_item(ksi_asme); /* No use in this case */
	pkt.extract_item(network_capability); /* No use in this case */

	enodeb_s1ap_id = pkt.s1ap_hdr.enodeb_ue_id;
	

	guti = get_guti(mme_ids.gummei, imsi);
	mlock(table1_mux);
	ue_count++;
	mme_s1ap_id = ue_count;
	table1[mme_s1ap_id] = guti;
	munlock(table1_mux);

	mlock(table2_mux);
	table2[guti].init(imsi, enodeb_s1ap_id, mme_s1ap_id, tai);
	munlock(table2_mux);
	

	pkt.clear_pkt();
	pkt.append_item(imsi);
	pkt.append_item(mme_ids.plmn_id);
	pkt.append_item(num_autn_vectors);
	pkt.append_item("nil");
	pkt.prepend_diameter_hdr(1, pkt.len);
	to_hss.conn(g_hss_ip_addr.c_str(), g_hss_port);
	to_hss.snd(pkt);
	to_hss.rcv(pkt);

	pkt.extract_item(autn_num);
	pkt.extract_item(rand_num);
	pkt.extract_item(xres);
	pkt.extract_item(k_asme);

	mlock(table2_mux);
	table2[guti].xres = xres;
	table2[guti].k_asme = k_asme;
	munlock(table2_mux);

	pkt.clear_pkt();
	pkt.append_item(autn_num);
	pkt.append_item(rand_num);
	pkt.append_item(ksi_asme);
	pkt.prepend_s1ap_hdr(1, pkt.len, enodeb_s1ap_id, mme_s1ap_id);
	server.snd(conn_fd, pkt);
}

void Mme::handle_autn(int conn_fd, Packet &pkt) {
	uint32_t mme_s1ap_id;
	uint64_t guti;
	uint64_t res;
	uint64_t xres;

	mme_s1ap_id = pkt.s1ap_hdr.mme_ue_id;

	mlock(table1_mux);
	guti = table1[mme_s1ap_id];
	munlock(table1_mux);

	pkt.extract_item(res);

	mlock(table2_mux);
	xres = table2[guti].xres;
	munlock(table2_mux);

	if (res == xres) {
		/* Success */
	}
	else {
		rem_table1_entry(mme_s1ap_id);
		rem_table2_entry(guti);		
	}
}

bool Mme::check_table1_entry(uint32_t arg_mme_s1ap_id) {
	mlock(table1_mux);
	if (table1.find(arg_mme_s1ap_id) != table1.end()) {
		return true;
	}
	else {
		return false;
	}
	munlock(table1_mux);
}

bool Mme::check_table2_entry(uint64_t arg_guti) {
	mlock(table2_mux);
	if (table2.find(arg_guti) != table2.end()) {
		return true;
	}
	else {
		return false;
	}
	munlock(table2_mux);
}

void Mme::rem_table1_entry(uint32_t arg_mme_s1ap_id) {
	mlock(table1_mux);
	table1.erase(arg_mme_s1ap_id);
	munlock(table1_mux);
}

void Mme::rem_table2_entry(uint64_t arg_guti) {
	mlock(table2_mux);
	table2.erase(arg_guti);
	munlock(table2_mux);
}

Mme::~Mme() {

}
