#include "hss.h"

UDPServer g_hss_server;
vector<pthread_t> g_tid;
int g_tcount;

HSS::HSS() {

}

void HSS::handle_query_error() {

	if (query_res == 0) {
		cout << "ERROR: No rows fetched for this query - ";
		cout << query << endl;
		report_error(-1);
	}
}

void HSS::read_data(){

	pkt.clear_data();
	status = recvfrom(g_hss_server.server_socket, pkt.data, BUF_SIZE, 0, (sockaddr*)&client_sock_addr, &g_addr_len);
	report_error(status);
	pkt.data_len = status;
	pkt.curr_pos = 0;	
}

void HSS::set_metadata(){

	pkt.copy_metadata(type, subtype, ue_num);
}

void HSS::process_req(){

	pkt.copy_data(ue_data.imsi);
	pkt.copy_data(ue_data.msisdn);
	set_key_id();
	set_autn_tokens();
	set_autn_xres();
	send_res();
}

void HSS::set_key_id() {

	query = "select key_id from ue_data where imsi = " + to_string(ue_data.imsi) + " and msisdn = " + to_string(ue_data.msisdn);
	db_client.perform_query(query.c_str());
	query_res = mysql_fetch_row(db_client.result);
	handle_query_error();
	res_row = query_res[0];	
	ue_data.key_id = stoull(res_row);
	mysql_free_result(db_client.result);
}

void HSS::set_autn_tokens() {
	int i;

	curr_time = time(0);
	local_time = localtime(&curr_time);
	curr_sec = local_time->tm_sec;
	query = "select * from ue_autn where autn like '%" + to_string(curr_sec) + "'";
	db_client.perform_query(query.c_str());
	num_fields = mysql_num_fields(db_client.result);
	query_res = mysql_fetch_row(db_client.result);
	handle_query_error();
	for (i = 0; i < num_fields; i++) {
		res_row = query_res[i];
		if (i == 0) {
			ue_data.autn_num = stoull(res_row);
		}
		else {
			ue_data.rand_num = stoull(res_row);
		}
	}
	mysql_free_result(db_client.result);
}

void HSS::set_autn_xres() {

	ue_data.autn_xres = (ue_data.autn_num * ue_data.key_id) + (ue_data.rand_num * (ue_data.key_id + 1));
}

void HSS::send_res() {

	pkt.add_metadata(type, subtype, ue_num);
	pkt.add_data(ue_data.autn_num);
	pkt.add_data(ue_data.rand_num);
	pkt.add_data(ue_data.autn_xres);

	g_hss_server.write_data(client_sock_addr, pkt);
}

HSS::~HSS() {


}
