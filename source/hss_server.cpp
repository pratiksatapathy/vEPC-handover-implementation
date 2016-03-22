#include "hss_server.h"

SctpServer g_hss_server;
Hss g_hss;
int g_workers_count;

// void* process_reqs(void *arg) {
// 	HSS hss;
// 	hss.db_client.setup_conn();

// 	while(1){
// 		hss.read_data();
// 		if(hss.status == 0)
// 			continue;
// 		hss.set_metadata();
// 		cout<<"Type is "<<hss.type<<". Subtype is "<<hss.subtype<<endl;
		
// 		if(hss.type == 1){
// 			switch(hss.subtype){
// 				case 1: 
// 					hss.process_req();
// 					break;
// 				default:
// 					cout << "Incorrect subtype for type - 1  -> " << hss.subtype << " . UE num is " << hss.ue_num << endl;
// 					cout << hss.pkt.data_len << endl;
// 			}
// 		}
// 		else{
// 			cout << "Incorrect type - " << hss.type << endl;
// 		}
// 	}
// 	mysql_thread_end();
// 	return NULL;
// }

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<hss_server_exec> THREAD_COUNT" << endl;
		handle_type1_error(-1, "Invalid usage error: hssserver_checkusage");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
	if (mysql_library_init(0, NULL, NULL)) {
		handle_type1_error(-1, "mysql_library_init error: hssserver_init")
	}
}

void run() {
	g_hss_server.run(g_hss_ip_addr.c_str(), g_hss_port, g_workers_count, handle_mme);
}

void handle_mme(int conn_fd) {


	// Packet pkt;

	// g_mme_server.rcv(conn_fd, pkt);
	// pkt.extract_s1ap_hdr();
	// if (pkt.s1ap_hdr.mme_ue_id == -1) {
	// 	switch (pkt.s1ap_hdr.msg_type) {
	// 		case 1: /* Attach request - Initial UE message */
	// 			g_mme.handle_type1_attach(pkt);
	// 			g_mme_server.snd(conn_fd, pkt);
	// 			break;
	// 		case 2:
	// 			break;
	// 		default:
	// 			break;
	// 	}		
	// }
	// else if (pkt.s1ap_hdr.mme_ue_id > 0) {
	// 	switch (pkt.s1ap_hdr.msg_type) {
	// 		case 2: /* Authentication response from UE */
	// 			g_mme.handle_authentication(pkt);
	// 			g_mme_server.snd(conn_fd, pkt);
	// 			break;
	// 		case 3:
	// 			break;
	// 		default:
	// 			break;
	// 	}				
	// }
}

void finish() {
	mysql_library_end();		
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	finish();
	return 0;
}