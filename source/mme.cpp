#include "mme.h"

UeContext::UeContext() {
	nw_type = 0;
	count = 1;
	bearer = 0;
	dir = 1;
}

void UeContext::init(uint64_t arg_imsi, uint32_t arg_enodeb_s1ap_ue_id, uint32_t arg_mme_s1ap_ue_id, uint64_t arg_tai, uint16_t arg_nw_capability) {
	imsi = arg_imsi;
	enodeb_s1ap_ue_id = arg_enodeb_s1ap_ue_id;
	mme_s1ap_ue_id = arg_mme_s1ap_ue_id;
	tai = arg_tai;
	nw_capability = arg_nw_capability;
	curr_state = 1;
}

UeContext::~UeContext() {

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
	SctpClient hss_client;
	uint64_t imsi;
	uint64_t tai;
	uint64_t ksi_asme;
	uint16_t nw_type;
	uint16_t nw_capability;
	uint64_t autn_num;
	uint64_t rand_num;
	uint64_t xres;
	uint64_t k_asme;
	uint32_t enodeb_s1ap_ue_id;
	uint32_t mme_s1ap_ue_id;
	uint64_t guti;
	uint64_t num_autn_vectors;

	hss_client.conn(g_hss_ip_addr.c_str(), g_hss_port);
	num_autn_vectors = 1;
	pkt.extract_item(imsi);
	pkt.extract_item(tai);
	pkt.extract_item(ksi_asme); /* No use in this case */
	pkt.extract_item(nw_capability); /* No use in this case */

	cout << "mme_handletype1attach:" << " imsi:" << imsi << " tai:" << tai << " ksi_asme:" << ksi_asme << " nw_capability:" << nw_capability << endl;
	enodeb_s1ap_ue_id = pkt.s1ap_hdr.enodeb_s1ap_ue_id;
	

	guti = get_guti(mme_ids.gummei, imsi);
	mlock(table1_mux);
	ue_count++;
	mme_s1ap_ue_id = ue_count;
	table1[mme_s1ap_ue_id] = guti;
	munlock(table1_mux);

	mlock(table2_mux);
	table2[guti].init(imsi, enodeb_s1ap_ue_id, mme_s1ap_ue_id, tai, nw_capability);
	nw_type = table2[guti].nw_type;
	munlock(table2_mux);
	

	pkt.clear_pkt();
	pkt.append_item(imsi);
	pkt.append_item(mme_ids.plmn_id);
	pkt.append_item(num_autn_vectors);
	pkt.append_item(nw_type);
	pkt.prepend_diameter_hdr(1, pkt.len);
	hss_client.snd(pkt);
	
	cout << "mme_handletype1attach:" << " request sent to hss" << endl;

	hss_client.rcv(pkt);

	pkt.extract_diameter_hdr();
	pkt.extract_item(autn_num);
	pkt.extract_item(rand_num);
	pkt.extract_item(xres);
	pkt.extract_item(k_asme);

	mlock(table2_mux);
	table2[guti].xres = xres;
	table2[guti].k_asme = k_asme;
	table2[guti].ksi_asme = 1;
	ksi_asme = table2[guti].ksi_asme;
	munlock(table2_mux);

	cout << "mme_handletype1attach:" << " autn:" << autn_num <<" rand:" << rand_num << " xres:" << xres << " k_asme:" << k_asme << endl;

	pkt.clear_pkt();
	pkt.append_item(autn_num);
	pkt.append_item(rand_num);
	pkt.append_item(ksi_asme);
	pkt.prepend_s1ap_hdr(1, pkt.len, enodeb_s1ap_ue_id, mme_s1ap_ue_id);
	server.snd(conn_fd, pkt);
	
	cout << "mme_handletype1attach:" << " request sent to ran" << endl;	
}

bool Mme::handle_autn(int conn_fd, Packet &pkt) {
	uint32_t mme_s1ap_ue_id;
	uint64_t guti;
	uint64_t res;
	uint64_t xres;

	mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;

	mlock(table1_mux);
	guti = table1[mme_s1ap_ue_id];
	munlock(table1_mux);

	pkt.extract_item(res);

	mlock(table2_mux);
	xres = table2[guti].xres;
	munlock(table2_mux);

	if (res == xres) {
		/* Success */
		return true;
	}
	else {
		rem_table1_entry(mme_s1ap_ue_id);
		rem_table2_entry(guti);		
		return false;
	}
}

void Mme::setup_security_context(int conn_fd, Packet &pkt) {
	uint32_t mme_s1ap_ue_id;
	uint64_t guti;

	mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;
	mlock(table1_mux);
	guti = table1[mme_s1ap_ue_id];
	munlock(table1_mux);
	setup_crypt_context(guti);
	setup_integrity_context(guti);

	
	
	
		
}

void Mme::setup_crypt_context(uint64_t guti) {
	mlock(table2_mux);
	table2[guti].nas_enc_algo = 1;
	table2[guti].k_nas_enc = table2[guti].k_asme + table2[guti].nas_enc_algo + table2[guti].count + table2[guti].bearer + table2[guti].dir;
	munlock(table2_mux);
}

void Mme::setup_integrity_context(uint64_t guti) {
	mlock(table2_mux);
	table2[guti].nas_int_algo = 1;
	table2[guti].k_nas_int = table2[guti].k_asme + table2[guti].nas_int_algo + table2[guti].count + table2[guti].bearer + table2[guti].dir;
	munlock(table2_mux);
}

void Mme::update_ue_location() {

}

bool Mme::check_table1_entry(uint32_t arg_mme_s1ap_ue_id) {
	mlock(table1_mux);
	if (table1.find(arg_mme_s1ap_ue_id) != table1.end()) {
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

void Mme::rem_table1_entry(uint32_t arg_mme_s1ap_ue_id) {
	mlock(table1_mux);
	table1.erase(arg_mme_s1ap_ue_id);
	munlock(table1_mux);
}

void Mme::rem_table2_entry(uint64_t arg_guti) {
	mlock(table2_mux);
	table2.erase(arg_guti);
	munlock(table2_mux);
}

Mme::~Mme() {

}
