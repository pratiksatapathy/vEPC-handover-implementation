#include "sgw_server.h"

void* process_traffic(void *arg) {
	SGW sgw;

	sgw.setup_topgw();
	sgw.setup_toenodeb();
	while(1){
		sgw.read_data();
		if(sgw.status == 0)
			continue;
		sgw.set_metadata();

		if(sgw.type == 1){	
			attach_process(sgw);
		}
		else if(sgw.type == 2){
			data_transfer(sgw);
		}
		else if(sgw.type == 3){
			detach_process(sgw);
		}
		else{
			cout << "Incorrect type - " << sgw.type << endl;
		}
	}
	return NULL;
}

void attach_process(SGW &sgw){

	if(sgw.subtype == 3){
		create_session(sgw);
	}
	else if(sgw.subtype == 4){
		modify_session(sgw);
	}
	else{
		cout << "Incorrect subtype for type - 1 -> " << sgw.subtype << endl;
	}
}

void create_session(SGW &sgw){

	sgw.store_create_session_data();
	sgw.set_pgw_details(sgw.ue_num);
	sgw.create_session_req_to_pgw();
	sgw.create_session_res_from_pgw();
	if(!sgw.success){
		return;
	}
	sgw.create_session_res_to_mme();
}

void modify_session(SGW &sgw){

	sgw.set_enodeb_details(sgw.ue_num);
	sgw.store_modify_session_data();
	sgw.modify_session_res_to_mme();
}

void data_transfer(SGW &sgw){

	if(sgw.subtype == 1){
		uplink_data_transfer(sgw);
	}
	else if(sgw.subtype == 2){
		downlink_data_transfer(sgw);
	}
	else{
		cout << "Incorrect subtype for type - 2 -> " << sgw.subtype << endl;
	}
}

void uplink_data_transfer(SGW &sgw){

	// if(g_sgw_data[sgw.ue_num].valid == true){
		sgw.make_uplink_data();
		sgw.send_pgw();
	// }
}

void downlink_data_transfer(SGW &sgw){

	// if(g_sgw_data[sgw.ue_num].valid == true){
		sgw.make_downlink_data();
		sgw.send_enodeb();
	// }
}

void detach_process(SGW &sgw){

	if(sgw.subtype == 1){
		delete_session(sgw);
	}
	else{
		cout << "Incorrect subtype for type - 3 -> " << sgw.subtype << endl;
	}
}

void delete_session(SGW &sgw){

	sgw.delete_session_req_to_pgw();
	sgw.delete_session_res_from_pgw();
	sgw.delete_session_res_to_mme();
	if(sgw.success == 1){
		sgw.delete_session_data();
	}
}

void startup_sgw(char *argv[]){

	g_tcount = atoi(argv[1]);
	g_tid.resize(g_tcount);
	g_sgw_server.bind_server(g_sgw1_port, g_sgw1_addr.c_str());
	g_sgw_server.print_status("SGW");	
}

int main(int argc, char *argv[]) {
	int status;
	int i;
		
	check_server_usage(argc, argv);
	startup_sgw(argv);
	setup_sgw_data();	

	for (i = 0; i < g_tcount; i++) {
		status = pthread_create(&g_tid[i], NULL, process_traffic, NULL);
		report_error(status);
	}
	for (i = 0; i < g_tcount; i++) {
		pthread_join(g_tid[i], NULL);
	}		

	free_sgw_data();
	return 0;
}