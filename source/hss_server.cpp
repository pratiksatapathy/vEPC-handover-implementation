#include "hss_server.h"

void* process_reqs(void *arg) {
	HSS hss;
	hss.db_client.setup_conn();

	while(1){
		hss.read_data();
		if(hss.status == 0)
			continue;
		hss.set_metadata();
		cout<<"Type is "<<hss.type<<". Subtype is "<<hss.subtype<<endl;
		
		if(hss.type == 1){
			switch(hss.subtype){
				case 1: 
					hss.process_req();
					break;
				default:
					cout << "Incorrect subtype for type - 1  -> " << hss.subtype << " . UE num is " << hss.ue_num << endl;
					cout << hss.pkt.data_len << endl;
			}
		}
		else{
			cout << "Incorrect type - " << hss.type << endl;
		}
	}
	mysql_thread_end();
	return NULL;
}

void startup_hss(char *argv[]){

	g_tcount = atoi(argv[1]);
	g_tid.resize(g_tcount);
	g_hss_server.bind_server(g_hss_port, g_hss_addr.c_str());
	g_hss_server.print_status("HSS");
}

int main(int argc, char *argv[]) {
	int status;
	int i;

	check_server_usage(argc, argv);
	if(mysql_library_init(0, NULL, NULL))
		cout << "ERROR: mysql library cannot be opened" << endl;	
	startup_hss(argv);
	setup_conn_details();
	for (i = 0; i < g_tcount; i++) {
		status = pthread_create(&g_tid[i], NULL, process_reqs, NULL);
		report_error(status);
	}
	for (i = 0; i < g_tcount; i++) {
		pthread_join(g_tid[i], NULL);
	}	
	mysql_library_end();	
	return 0;
}