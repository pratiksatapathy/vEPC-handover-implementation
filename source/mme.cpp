#include "mme.h"

UeContext::UeContext() {
	emm_state = 0;
	ecm_state = 0;
	imsi = 0;
	string ip_addr = "";
	enodeb_s1ap_ue_id = 0;
	mme_s1ap_ue_id = 0;
	tai = 0;
	ksi_asme = 0;
	k_asme = 0; 
	k_nas_enc = 0; 
	k_nas_int = 0; 
	nas_enc_algo = 0; 
	nas_int_algo = 0; 
	count = 1;
	bearer = 0;
	dir = 1;
	default_apn = 0; 
	apn_in_use = 0; 
	eps_bearer_id = 0; 
	s1_teid_ul = 0; 
	s1_teid_dl = 0; 
	s5_teid_ul = 0; 
	s5_teid_dl = 0; 
	xres = 0;
	nw_type = 0;
	nw_capability = 0;	
}

void UeContext::init(uint64_t arg_imsi, uint32_t arg_enodeb_s1ap_ue_id, uint32_t arg_mme_s1ap_ue_id, uint64_t arg_tai, uint16_t arg_nw_capability) {
	imsi = arg_imsi;
	enodeb_s1ap_ue_id = arg_enodeb_s1ap_ue_id;
	mme_s1ap_ue_id = arg_mme_s1ap_ue_id;
	tai = arg_tai;
	nw_capability = arg_nw_capability;
}

UeContext::~UeContext() {

}

MmeIds::MmeIds() {
	mcc = 1;
	mnc = 1;
	plmn_id = g_telecom.get_plmn_id(mcc, mnc);
	mmegi = 1;
	mmec = 1;
	mmei = g_telecom.get_mmei(mmegi, mmec);
	gummei = g_telecom.get_gummei(plmn_id, mmei);
}

MmeIds::~MmeIds() {
	
}

Mme::Mme() {
	crypt.load();
	ue_count = 0;
	g_sync.mux_init(table1_mux);
	g_sync.mux_init(table2_mux);
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
	

	guti = g_telecom.get_guti(mme_ids.gummei, imsi);
	g_sync.mlock(table1_mux);
	ue_count++;
	mme_s1ap_ue_id = ue_count;
	table1[mme_s1ap_ue_id] = guti;
	g_sync.munlock(table1_mux);

	g_sync.mlock(table2_mux);
	table2[guti].init(imsi, enodeb_s1ap_ue_id, mme_s1ap_ue_id, tai, nw_capability);
	nw_type = table2[guti].nw_type;
	g_sync.munlock(table2_mux);
	

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

	g_sync.mlock(table2_mux);
	table2[guti].xres = xres;
	table2[guti].k_asme = k_asme;
	table2[guti].ksi_asme = 1;
	ksi_asme = table2[guti].ksi_asme;
	g_sync.munlock(table2_mux);

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
	uint64_t guti;
	uint64_t res;
	uint64_t xres;

	guti = get_guti(pkt);
	pkt.extract_item(res);
	g_sync.mlock(table2_mux);
	xres = table2[guti].xres;
	g_sync.munlock(table2_mux);
	if (res == xres) {
		/* Success */
		cout << "mme_handleautn:" << " Authentication successful" << endl;
		return true;
	}
	else {
		rem_table1_entry(pkt.s1ap_hdr.mme_s1ap_ue_id);
		rem_table2_entry(guti);		
		return false;
	}
}

void Mme::handle_security_mode_cmd(int conn_fd, Packet &pkt) {
	uint64_t guti;
	uint64_t ksi_asme;
	uint16_t nw_capability;
	uint64_t nas_enc_algo;
	uint64_t nas_int_algo;
	uint64_t k_nas_enc;
	uint64_t k_nas_int;

	guti = get_guti(pkt);
	setup_crypt_context(guti);
	setup_integrity_context(guti);
	g_sync.mlock(table2_mux);
	ksi_asme = table2[guti].ksi_asme;
	nw_capability = table2[guti].nw_capability;
	nas_enc_algo = table2[guti].nas_enc_algo;
	nas_int_algo = table2[guti].nas_int_algo;
	k_nas_enc = table2[guti].k_nas_enc;
	k_nas_int = table2[guti].k_nas_int;
	g_sync.munlock(table2_mux);

	pkt.clear_pkt();
	pkt.append_item(ksi_asme);
	pkt.append_item(nw_capability);
	pkt.append_item(nas_enc_algo);
	pkt.append_item(nas_int_algo);
	integrity.add_hmac(pkt, k_nas_int);
	pkt.prepend_s1ap_hdr(3, pkt.len, pkt.s1ap_hdr.enodeb_s1ap_ue_id, pkt.s1ap_hdr.mme_s1ap_ue_id);
	server.snd(conn_fd, pkt);
	cout << "mme_handlesecuritymodecmd:" << " security mode command sent" << endl;
}

void Mme::setup_crypt_context(uint64_t guti) {
	g_sync.mlock(table2_mux);
	table2[guti].nas_enc_algo = 1;
	table2[guti].k_nas_enc = table2[guti].k_asme + table2[guti].nas_enc_algo + table2[guti].count + table2[guti].bearer + table2[guti].dir;
	g_sync.munlock(table2_mux);
}

void Mme::setup_integrity_context(uint64_t guti) {
	g_sync.mlock(table2_mux);
	table2[guti].nas_int_algo = 1;
	table2[guti].k_nas_int = table2[guti].k_asme + table2[guti].nas_int_algo + table2[guti].count + table2[guti].bearer + table2[guti].dir;
	g_sync.munlock(table2_mux);
}

bool Mme::handle_security_mode_complete(int conn_fd, Packet &pkt) {
	uint64_t guti;
	uint64_t k_nas_enc;
	uint64_t k_nas_int;
	bool res;

	guti = get_guti(pkt);
	g_sync.mlock(table2_mux);
	k_nas_enc = table2[guti].k_nas_enc;
	k_nas_int = table2[guti].k_nas_int;
	g_sync.munlock(table2_mux);
	res = integrity.hmac_check(pkt, k_nas_int);
	if (res == false) {
		cout << "mme_handlesecuritymodecomplete:" << " hmac failure" << endl;
		return false;
	}
	else {
		crypt.dec(pkt, k_nas_enc);
		pkt.extract_item(res);
		if (res == false) {
			cout << "mme_handlesecuritymodecomplete:" << " security mode complete failure" << endl;
			return false;
		}
		else {
			cout << "mme_handlesecuritymodecomplete:" << " security mode complete success" << endl;
			return true;
		}
	}
}

void Mme::handle_location_update(Packet &pkt) {
	SctpClient hss_client;
	uint64_t guti;
	uint64_t imsi;
	uint64_t default_apn;

	hss_client.conn(g_hss_ip_addr.c_str(), g_hss_port);
	guti = get_guti(pkt);
	g_sync.mlock(table2_mux);
	imsi = table2[guti].imsi;
	g_sync.munlock(table2_mux);
	pkt.clear_pkt();
	pkt.append_item(imsi);
	pkt.append_item(mme_ids.mmei);
	pkt.prepend_diameter_hdr(2, pkt.len);
	hss_client.snd(pkt);
	hss_client.rcv(pkt);
	pkt.extract_diameter_hdr();
	pkt.extract_item(default_apn);
	g_sync.mlock(table2_mux);
	table2[guti].default_apn = default_apn;
	table2[guti].apn_in_use = table2[guti].default_apn;
	g_sync.munlock(table2_mux);
}

void Mme::handle_create_session(int conn_fd, Packet &pkt) {
	uint64_t guti;

	guti = get_guti(pkt);


}

void Mme::handle_modify_bearer(Packet &pkt) {

}

uint64_t Mme::get_guti(Packet pkt) {
	uint64_t mme_s1ap_ue_id;
	uint64_t guti;

	mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;
	g_sync.mlock(table1_mux);
	guti = table1[mme_s1ap_ue_id];
	g_sync.munlock(table1_mux);
	return guti;
}

bool Mme::check_table1_entry(uint32_t arg_mme_s1ap_ue_id) {
	g_sync.mlock(table1_mux);
	if (table1.find(arg_mme_s1ap_ue_id) != table1.end()) {
		return true;
	}
	else {
		return false;
	}
	g_sync.munlock(table1_mux);
}

bool Mme::check_table2_entry(uint64_t arg_guti) {
	g_sync.mlock(table2_mux);
	if (table2.find(arg_guti) != table2.end()) {
		return true;
	}
	else {
		return false;
	}
	g_sync.munlock(table2_mux);
}

void Mme::rem_table1_entry(uint32_t arg_mme_s1ap_ue_id) {
	g_sync.mlock(table1_mux);
	table1.erase(arg_mme_s1ap_ue_id);
	g_sync.munlock(table1_mux);
}

void Mme::rem_table2_entry(uint64_t arg_guti) {
	g_sync.mlock(table2_mux);
	table2.erase(arg_guti);
	g_sync.munlock(table2_mux);
}

Mme::~Mme() {

}
