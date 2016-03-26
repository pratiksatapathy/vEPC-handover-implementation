#include "hss.h"

Hss::Hss() {
	mux_init(mysql_client_mux);

	/* Lock not necessary since this is called only once per object. Added for uniformity in locking */
	mlock(mysql_client_mux);
	mysql_client.conn();
	munlock(mysql_client_mux);
}

void Hss::get_ue_info(uint64_t imsi, uint64_t &key, uint64_t &rand_num) {
	MYSQL_RES *query_res;
	MYSQL_ROW query_res_row;
	int i;
	int num_fields;
	string query;

	query_res = NULL;
	query = "select key_id, rand_num from ue_info where imsi = " + to_string(imsi);
	cout << "hss_getueinfo:" << query << endl;
	mlock(mysql_client_mux);
	mysql_client.handle_query(query.c_str(), &query_res);
	munlock(mysql_client_mux);
	num_fields = mysql_num_fields(query_res);
	cout << "hss_getueinfo:" << " fetched" << endl;
	query_res_row = mysql_fetch_row(query_res);
	cout << "hss_getueinfo:" << endl;
	if (query_res_row == 0) {
		handle_type1_error(-1, "mysql_fetch_row error: hss_getueinfo");
	}
	for (i = 0; i < num_fields; i++) {
		string query_res_field;

		query_res_field = query_res_row[i];
		if (i == 0) {
			key = stoull(query_res_field);
		}
		else {
			rand_num = stoull(query_res_field);
		}
	}
	mysql_free_result(query_res);
}

void Hss::handle_autninfo_req(int conn_fd, Packet &pkt) {
	uint64_t imsi;
	uint64_t key;
	uint64_t rand_num;
	uint64_t autn_num;
	uint64_t sqn;
	uint64_t xres;
	uint64_t ck;
	uint64_t ik;
	uint64_t k_asme;
	uint64_t num_autn_vectors;
	uint16_t plmn_id;
	uint16_t nw_type;

	pkt.extract_item(imsi);
	pkt.extract_item(plmn_id);
	pkt.extract_item(num_autn_vectors);
	pkt.extract_item(nw_type);
	get_ue_info(imsi, key, rand_num);
	cout << "hss_handleautoinforeq:" << " retrieved from database" << endl;
	sqn = rand_num + 1;
	xres = key + sqn + rand_num;
	autn_num = xres + 1;
	ck = xres + 2;
	ik = xres + 3;
	k_asme = ck + ik + sqn + plmn_id;
	cout << "hss_handleautoinforeq:" << " autn:" << autn_num << " rand:" << rand_num << " xres:" << xres << " k_asme:" << k_asme << endl;
	pkt.clear_pkt();
	pkt.append_item(autn_num);
	pkt.append_item(rand_num);
	pkt.append_item(xres);
	pkt.append_item(k_asme);
	pkt.prepend_diameter_hdr(1, pkt.len);
	server.snd(conn_fd, pkt);
	cout << "hss_handleautoinforeq:" << " response sent to mme" << endl;
}

Hss::~Hss() {

}
