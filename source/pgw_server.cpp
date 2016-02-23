#include "pgw_server.h"

void* process_downlink_traffic(void *arg) {
	PGWDlink pgw_dlink;

	pgw_dlink.setup_tosgw();
	while(1){
		downlink_data_transfer(pgw_dlink);
	}
	return NULL;
}

void downlink_data_transfer(PGWDlink &pgw_dlink){

	pgw_dlink.read_data();
	pgw_dlink.set_ue_num();
	if(!pgw_dlink.success){
		return;
	}
	// if(g_pgw_data[pgw_dlink.ue_num].valid == true){
		pgw_dlink.make_downlink_data();
		pgw_dlink.send_sgw();			
	// }
}

void* process_traffic(void *arg) {
	PGW pgw;

	pgw.setup_topublicsink();
	while(1){
		pgw.read_data();
		if(pgw.status == 0)
			continue;
		pgw.set_metadata();

		if(pgw.type == 1){	
			attach_process(pgw);
		}
		else if(pgw.type == 2){
			data_transfer(pgw);
		}
		else if(pgw.type == 3){
			detach_process(pgw);
		}
		else{
			cout << "Incorrect type - " << pgw.type << endl;
		}
	}
	return NULL;
}

void attach_process(PGW &pgw){
	
	if(pgw.subtype == 3){
		create_session(pgw);
	}
	else{
		cout << "Incorrect subtype for type 1 -> " << pgw.subtype << endl;
	}
}

void create_session(PGW &pgw){

	pgw.set_sgw_details(pgw.ue_num);
	pgw.store_create_session_data();
	pgw.create_session_res_to_sgw();
	pgw.add_map_entry();
}

void data_transfer(PGW &pgw){

	if(pgw.subtype == 1){
		uplink_data_transfer(pgw);
	}
	else{
		cout << "Incorrect subtype for type 2 -> " << pgw.subtype << endl;	
	}
}

void uplink_data_transfer(PGW &pgw){

	// if(g_pgw_data[pgw.ue_num].valid == true){
		pgw.make_uplink_data();
		pgw.send_public_sink();
	// }	
}

void detach_process(PGW &pgw){

	if(pgw.subtype == 1){
		delete_session(pgw);
	}
	else{
		cout << "Incorrect subtype for type 3 -> " << pgw.subtype << endl;	
	}
}

void delete_session(PGW &pgw){

	pgw.delete_session_res_to_sgw();
	if(pgw.success == 1){
		pgw.delete_session_data();
	}
}

void startup_pgw(char *argv[]){

	g_tcount = atoi(argv[1]);
	g_tid.resize(g_tcount);
	g_pgw_server.bind_server(g_pgw_port, g_pgw_addr.c_str());
	g_pgw_server.print_status("PGW");		
	g_pgw_dlink_server.bind_server(g_pgw_dlink_port, g_pgw_dlink_addr.c_str());
	g_pgw_dlink_server.print_status("PGW Downlink Server");
}

int main(int argc, char *argv[]) {
	int status;
	int i;

	check_server_usage(argc, argv);
	startup_pgw(argv);
	setup_pgw_data();
	generate_ip_table();

	status = pthread_create(&g_downlink_tid, NULL, process_downlink_traffic, NULL);
	report_error(status);	

	for (i = 0; i < g_tcount; i++) {
		status = pthread_create(&g_tid[i], NULL, process_traffic, NULL);
		report_error(status);
	}
	for (i = 0; i < g_tcount; i++) {
		pthread_join(g_tid[i], NULL);
	}		

	free_pgw_data();
	return 0;
}