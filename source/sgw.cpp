#include "sgw.h"

// string g_sgw_s11_ip_addr = "10.14.13.29";
// string g_sgw_s1_ip_addr = "10.14.13.29";
// string g_sgw_s5_ip_addr = "10.14.13.29";
// int g_sgw_s11_port = 7000;
// int g_sgw_s1_port = 7100;
// int g_sgw_s5_port = 7200;

string g_sgw_s11_ip_addr = "127.0.0.1";
string g_sgw_s1_ip_addr = "127.0.0.1";
string g_sgw_s5_ip_addr = "127.0.0.1";
int g_sgw_s11_port = 7000;
int g_sgw_s1_port = 7100;
int g_sgw_s5_port = 7200;

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
	enodeb_ip_addr = "";
	enodeb_port = 0;
}

void UeContext::init(uint64_t arg_tai, uint64_t arg_apn_in_use,
		uint8_t arg_eps_bearer_id, uint32_t arg_s1_uteid_ul,
		uint32_t arg_s5_uteid_dl, uint32_t arg_s11_cteid_mme,
		uint32_t arg_s11_cteid_sgw, uint32_t arg_s5_cteid_dl,
		string arg_pgw_s5_ip_addr, int arg_pgw_s5_port) {
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
	clrstl();
	g_sync.mux_init(s11id_mux);
	g_sync.mux_init(s1id_mux);
	g_sync.mux_init(s5id_mux);
	g_sync.mux_init(uectx_mux);
}

void Sgw::clrstl() {
	s11_id.clear();
	s1_id.clear();
	s5_id.clear();
	ue_ctx.clear();
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
	uint64_t apn_in_use;
	uint64_t tai;
	string pgw_s5_ip_addr;
	string ue_ip_addr;
	int pgw_s5_port;

	pkt.extract_item(s11_cteid_mme);
	pkt.extract_item(imsi);
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(pgw_s5_ip_addr);
	pkt.extract_item(pgw_s5_port);
	pkt.extract_item(apn_in_use);
	pkt.extract_item(tai);
	s1_uteid_ul = s11_cteid_mme;
	s5_uteid_dl = s11_cteid_mme;
	s11_cteid_sgw = s11_cteid_mme;
	s5_cteid_dl = s11_cteid_mme;
	cout << "--" << pgw_s5_ip_addr << "--" << pkt.len << "--: " << imsi << endl;

	update_itfid(11, s11_cteid_sgw, imsi);
	update_itfid(1, s1_uteid_ul, imsi);
	update_itfid(5, s5_uteid_dl, imsi);
	g_sync.mlock(uectx_mux);
	ue_ctx[imsi].init(tai, apn_in_use, eps_bearer_id, s1_uteid_ul, s5_uteid_dl,
			s11_cteid_mme, s11_cteid_sgw, s5_cteid_dl, pgw_s5_ip_addr,
			pgw_s5_port);
	ue_ctx[imsi].tai = tai;
	g_sync.munlock(uectx_mux);
	cout << "Entry added for this imsi: " << imsi << endl;

	pgw_client.conn(g_sgw_s5_ip_addr, pgw_s5_ip_addr, pgw_s5_port);
	pkt.clear_pkt();
	pkt.append_item(s5_cteid_dl);
	pkt.append_item(imsi);
	pkt.append_item(eps_bearer_id);
	pkt.append_item(s5_uteid_dl);
	pkt.append_item(apn_in_use);
	pkt.append_item(tai);
	pkt.prepend_gtp_hdr(2, 1, pkt.len, 0);
	pgw_client.snd(pkt);
	cout << "sgw_handlecreatesession:"
			<< " create session request sent to pgw: " << imsi << endl;
	pgw_client.rcv(pkt);
	cout << "sgw_handlecreatesession:"
			<< " create session response received from pgw: " << imsi << endl;
	pkt.extract_gtp_hdr();
	pkt.extract_item(s5_cteid_ul);
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(s5_uteid_ul);
	pkt.extract_item(ue_ip_addr);

	g_sync.mlock(uectx_mux);
	ue_ctx[imsi].s5_uteid_ul = s5_uteid_ul;
	ue_ctx[imsi].s5_cteid_ul = s5_cteid_ul;
	g_sync.munlock(uectx_mux);

	pkt.clear_pkt();
	pkt.append_item(s11_cteid_sgw);
	pkt.append_item(ue_ip_addr);
	pkt.append_item(s1_uteid_ul);
	pkt.append_item(s5_uteid_ul);
	pkt.append_item(s5_uteid_dl);
	pkt.prepend_gtp_hdr(2, 1, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
	cout << "sgw_handlecreatesession:"
			<< " create session response sent to mme: " << imsi << endl;
}

void Sgw::handle_modify_bearer(struct sockaddr_in src_sock_addr, Packet pkt) {
	uint64_t imsi;
	uint32_t s1_uteid_dl;
	uint32_t s11_cteid_mme;
	uint8_t eps_bearer_id;
	string enodeb_ip_addr;
	int enodeb_port;
	bool res;

	imsi = get_imsi(11, pkt.gtp_hdr.teid);
	if (imsi == 0) {
		cout << "sgw_handlemodifybearer:" << " :zero imsi " << pkt.gtp_hdr.teid
				<< " " << pkt.len << ": " << imsi << endl;
		g_utils.handle_type1_error(-1, "Zero imsi: sgw_handlemodifybearer");
	}
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(s1_uteid_dl);
	pkt.extract_item(enodeb_ip_addr);
	pkt.extract_item(enodeb_port);
	g_sync.mlock(uectx_mux);
	ue_ctx[imsi].s1_uteid_dl = s1_uteid_dl;
	ue_ctx[imsi].enodeb_ip_addr = enodeb_ip_addr;
	ue_ctx[imsi].enodeb_port = enodeb_port;
	s11_cteid_mme = ue_ctx[imsi].s11_cteid_mme;
	g_sync.munlock(uectx_mux);
	res = true;
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_gtp_hdr(2, 2, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
	cout << "sgw_handlemodifybearer:" << " modify bearer response sent to mme: "
			<< imsi << endl;
}
//HO code start

void Sgw::handle_indirect_tunnel_setup(struct sockaddr_in src_sock_addr,
		Packet pkt) {

	uint64_t imsi;
	uint32_t s1_uteid_dl;
	uint32_t s1_uteid_ul;

	uint32_t s11_cteid_mme;

	bool res;

	imsi = get_imsi(11, pkt.gtp_hdr.teid);

	pkt.extract_item(s1_uteid_dl);
	//update to s1 map

	g_sync.mlock(uectx_mux);
	ho_ue_ctx[imsi].s1_uteid_dl = s1_uteid_dl; //lock ??//dl to sent uplink data to tRan
	s11_cteid_mme = ue_ctx[imsi].s11_cteid_mme;
	g_sync.munlock(uectx_mux);
	res = true;

	s1_uteid_ul = s11_cteid_mme + 1; //create an indirect tunnel end point
	update_itfid(1, s1_uteid_ul, imsi);


	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.append_item(s1_uteid_ul);
	pkt.prepend_gtp_hdr(2, 4, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
	cout<<"indirect tunnel set up done at sgw"<<"\n";
}
void Sgw::handle_handover_completion(struct sockaddr_in src_sock_addr,Packet pkt) {

	//change the downlink of indirect tunnel

	uint64_t imsi;
	uint32_t s1_uteid_dl;
	uint32_t s1_uteid_ul;

	uint32_t s11_cteid_mme;

	bool res;

	imsi = get_imsi(11, pkt.gtp_hdr.teid);

	//reassign the indirect tunnel as direct tunnel
	g_sync.mlock(uectx_mux);
	ue_ctx[imsi].s1_uteid_dl =  ho_ue_ctx[imsi].s1_uteid_dl ; //lock ??
	s11_cteid_mme = ue_ctx[imsi].s11_cteid_mme;
	g_sync.munlock(uectx_mux);
	res = true;

	//remove from handover entry
	ho_ue_ctx.erase(imsi);

	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_gtp_hdr(2, 5, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);

	cout<<"switched downlink for the particular UE, removed entry from HO context \n";

}
void Sgw::handle_indirect_tunnel_teardown_(struct sockaddr_in src_sock_addr,Packet pkt) {


	uint64_t imsi;
	uint32_t s1_uteid_ul_indirect;

	uint32_t s11_cteid_mme;

	bool res;

	imsi = get_imsi(11, pkt.gtp_hdr.teid);
	s11_cteid_mme = ue_ctx[imsi].s11_cteid_mme;


	pkt.extract_item(s1_uteid_ul_indirect);
	rem_itfid(1, s1_uteid_ul_indirect);//tearing down the indirect tunnel



	res = true;
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_gtp_hdr(2, 6, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
	cout<<"teardown of indirect uplink teid complete at sgw \n";

}

//HO code end
void Sgw::handle_uplink_udata(Packet pkt) {

	//chk whther indirect tunnel data for dl tenb

	uint64_t imsi;
	uint32_t s5_uteid_ul;
	uint32_t s1_uteid_dl;
	string pgw_s5_ip_addr;
	int pgw_s5_port;
	bool res;

	imsi = get_imsi(1, pkt.gtp_hdr.teid);

	//handover code



	if (ho_ue_ctx.find(imsi) != ho_ue_ctx.end()) {
		string enodeb_ip_addr;
		int enodeb_port;
		s1_uteid_dl = ho_ue_ctx[imsi].s1_uteid_dl;

		//to add
		enodeb_ip_addr = ho_ue_ctx[imsi].enodeb_ip_addr;//ran 2 can send these things
		enodeb_port = ho_ue_ctx[imsi].enodeb_port;

		UdpClient enodeb_client;

		pkt.truncate();
		pkt.prepend_gtp_hdr(1, 2, pkt.len, s1_uteid_dl);
		enodeb_client.conn(g_sgw_s1_ip_addr, enodeb_ip_addr, enodeb_port);
		enodeb_client.snd(pkt);
		cout << "sgw_handledownlinkudata:"
				<< " downlink udata forwarded to enodeb through indirect tunnel: " << imsi << endl;


	} else {

		res = get_uplink_info(imsi, s5_uteid_ul, pgw_s5_ip_addr, pgw_s5_port);
		if (res) {
			UdpClient pgw_s5_client;

			pkt.truncate();
			pkt.prepend_gtp_hdr(1, 2, pkt.len, s5_uteid_ul);
			pgw_s5_client.conn(g_sgw_s5_ip_addr, pgw_s5_ip_addr, pgw_s5_port);
			pgw_s5_client.snd(pkt);
			cout << "sgw_handleuplinkudata:"
					<< " uplink udata forwarded to pgw: " << imsi << endl;
		}
	}
}

void Sgw::handle_downlink_udata(Packet pkt) {
	uint64_t imsi;
	uint32_t s1_uteid_dl;
	string enodeb_ip_addr;
	int enodeb_port;
	bool res;

	imsi = get_imsi(5, pkt.gtp_hdr.teid);
	res = get_downlink_info(imsi, s1_uteid_dl, enodeb_ip_addr, enodeb_port);
	if (res) {
		UdpClient enodeb_client;

		pkt.truncate();
		pkt.prepend_gtp_hdr(1, 2, pkt.len, s1_uteid_dl);
		enodeb_client.conn(g_sgw_s1_ip_addr, enodeb_ip_addr, enodeb_port);
		enodeb_client.snd(pkt);
		cout << "sgw_handledownlinkudata:"
				<< " downlink udata forwarded to enodeb: " << imsi << endl;
	}
}

void Sgw::handle_detach(struct sockaddr_in src_sock_addr, Packet pkt) {
	UdpClient pgw_s5_client;
	uint64_t imsi;
	uint64_t tai;
	uint32_t s1_uteid_ul;
	uint32_t s5_uteid_dl;
	uint32_t s11_cteid_mme;
	uint32_t s11_cteid_sgw;
	uint64_t s5_cteid_ul;
	uint8_t eps_bearer_id;
	string pgw_s5_ip_addr;
	int pgw_s5_port;
	bool res;

	imsi = get_imsi(11, pkt.gtp_hdr.teid);
	if (imsi == 0) {
		cout << "imsi is zero" << " " << pkt.gtp_hdr.teid << " " << pkt.len
				<< ": " << imsi << endl;
		// g_utils.handle_type1_error(-1, "No imsi found for this teid " + to_string(pkt.gtp_hdr.teid));
	}
	pkt.extract_item(eps_bearer_id);
	pkt.extract_item(tai);
	g_sync.mlock(uectx_mux);
	if (ue_ctx.find(imsi) == ue_ctx.end()) {
		cout << "ERR ERR ERR: " << imsi << endl;
		g_utils.handle_type1_error(-1,
				"No values found for this imsi " + to_string(imsi)
				+ ": sgw_handledetach");
	}
	pgw_s5_ip_addr = ue_ctx[imsi].pgw_s5_ip_addr;
	pgw_s5_port = ue_ctx[imsi].pgw_s5_port;
	s5_cteid_ul = ue_ctx[imsi].s5_cteid_ul;
	s11_cteid_mme = ue_ctx[imsi].s11_cteid_mme;
	s11_cteid_sgw = ue_ctx[imsi].s11_cteid_sgw;
	s1_uteid_ul = ue_ctx[imsi].s1_uteid_ul;
	s5_uteid_dl = ue_ctx[imsi].s5_uteid_dl;
	g_sync.munlock(uectx_mux);
	if (imsi == 0) {
		cout << "ERR: " << pgw_s5_ip_addr << " " << pgw_s5_port << " "
				<< s5_cteid_ul << " " << s11_cteid_mme << " " << imsi << endl;
	}

	pgw_s5_client.conn(g_sgw_s5_ip_addr, pgw_s5_ip_addr, pgw_s5_port);
	pkt.clear_pkt();
	pkt.append_item(eps_bearer_id);
	pkt.append_item(tai);
	pkt.prepend_gtp_hdr(2, 4, pkt.len, s5_cteid_ul);
	pgw_s5_client.snd(pkt);
	cout << "sgw_handledetach:" << " detach request sent to pgw: " << imsi
			<< endl;
	pgw_s5_client.rcv(pkt);
	cout << "sgw_handledetach:" << " detach response received from pgw: "
			<< imsi << endl;
	pkt.extract_gtp_hdr();
	pkt.extract_item(res);
	if (res == false) {
		cout << "sgw_handledetach:" << " pgw detach failure: " << imsi << endl;
		return;
	}
	pkt.clear_pkt();
	pkt.append_item(res);
	pkt.prepend_gtp_hdr(2, 3, pkt.len, s11_cteid_mme);
	s11_server.snd(src_sock_addr, pkt);
	cout << "sgw_handledetach:" << " detach response sent to mme: " << imsi
			<< endl;
	rem_itfid(11, s11_cteid_sgw);
	rem_itfid(1, s1_uteid_ul);
	rem_itfid(5, s5_uteid_dl);
	rem_uectx(imsi);
	cout << "Entry removed for this imsi: " << imsi << " " << endl;
	cout << "sgw_handledetach:" << " detach successful: " << imsi << endl;
}

void Sgw::update_itfid(int itf_id_no, uint32_t teid, uint64_t imsi) {
	switch (itf_id_no) {
	case 11:
		g_sync.mlock(s11id_mux);
		s11_id[teid] = imsi;
		g_sync.munlock(s11id_mux);
		break;
	case 1:
		g_sync.mlock(s1id_mux);
		s1_id[teid] = imsi;
		g_sync.munlock(s1id_mux);
		break;
	case 5:
		g_sync.mlock(s5id_mux);
		s5_id[teid] = imsi;
		g_sync.munlock(s5id_mux);
		break;
	default:
		g_utils.handle_type1_error(-1, "incorrect itf_id_no: sgw_updateitfid");
	}
}

uint64_t Sgw::get_imsi(int itf_id_no, uint32_t teid) {
	uint64_t imsi;

	imsi = 0;
	switch (itf_id_no) {
	case 11:
		g_sync.mlock(s11id_mux);
		if (s11_id.find(teid) != s11_id.end()) {
			imsi = s11_id[teid];
		}
		g_sync.munlock(s11id_mux);
		break;
	case 1:
		g_sync.mlock(s1id_mux);
		if (s1_id.find(teid) != s1_id.end()) {
			imsi = s1_id[teid];
		}
		g_sync.munlock(s1id_mux);
		break;
	case 5:
		g_sync.mlock(s5id_mux);
		if (s5_id.find(teid) != s5_id.end()) {
			imsi = s5_id[teid];
		}
		g_sync.munlock(s5id_mux);
		break;
	default:
		g_utils.handle_type1_error(-1, "incorrect itf_id_no: sgw_getimsi");
	}
	return imsi;
}

bool Sgw::get_uplink_info(uint64_t imsi, uint32_t &s5_uteid_ul,
		string &pgw_s5_ip_addr, int &pgw_s5_port) {
	bool res = false;

	g_sync.mlock(uectx_mux);
	if (ue_ctx.find(imsi) != ue_ctx.end()) {
		res = true;
		s5_uteid_ul = ue_ctx[imsi].s5_uteid_ul;
		pgw_s5_ip_addr = ue_ctx[imsi].pgw_s5_ip_addr;
		pgw_s5_port = ue_ctx[imsi].pgw_s5_port;
	}
	g_sync.munlock(uectx_mux);
	return res;
}

bool Sgw::get_downlink_info(uint64_t imsi, uint32_t &s1_uteid_dl,
		string &enodeb_ip_addr, int &enodeb_port) {
	bool res = false;

	g_sync.mlock(uectx_mux);
	if (ue_ctx.find(imsi) != ue_ctx.end()) {
		res = true;
		s1_uteid_dl = ue_ctx[imsi].s1_uteid_dl;
		enodeb_ip_addr = ue_ctx[imsi].enodeb_ip_addr;
		enodeb_port = ue_ctx[imsi].enodeb_port;
	}
	g_sync.munlock(uectx_mux);
	return res;
}

void Sgw::rem_itfid(int itf_id_no, uint32_t teid) {
	switch (itf_id_no) {
	case 11:
		g_sync.mlock(s11id_mux);
		s11_id.erase(teid);
		g_sync.munlock(s11id_mux);
		break;
	case 1:
		g_sync.mlock(s1id_mux);
		s1_id.erase(teid);
		g_sync.munlock(s1id_mux);
		break;
	case 5:
		g_sync.mlock(s5id_mux);
		s5_id.erase(teid);
		g_sync.munlock(s5id_mux);
		break;
	default:
		g_utils.handle_type1_error(-1, "incorrect itf_id_no: sgw_remitfid");
	}
}

void Sgw::rem_uectx(uint64_t imsi) {
	g_sync.mlock(uectx_mux);
	ue_ctx.erase(imsi);
	g_sync.munlock(uectx_mux);
}

Sgw::~Sgw() {

}
