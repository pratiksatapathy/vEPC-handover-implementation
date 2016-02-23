#include "mme_server.h"

void* process_reqs(void *arg) {
	MME mme;

	mme.setup_tohss();
	mme.setup_tosgw();
	while(1){
		mme.read_data();
		cout<< "read data" <<endl;
		if(mme.status == 0)
			continue;
		mme.set_metadata();

		if(mme.type == 1){
			attach_process(mme);
		}
		else if(mme.type == 3){
			detach_process(mme);
		}
		else{
			cout << "Incorrect type - " << mme.type << endl;
		}
	}
	return NULL;
}

void attach_process(MME &mme){
	
	if(mme.subtype == 1){
		init_autn(mme);
	}
	else if(mme.subtype == 2){
		create_session(mme);
	}	
	else if(mme.subtype == 3){
		modify_session(mme);
	}
	else{
		cout << "Incorrect subtype for type - 1" << mme.subtype << endl;
	}
}

void init_autn(MME &mme){

	mme.fetch_ue_data();
	if(!mme.success){
		return;
	}

	mme.store_ue_data();
	mme.send_autn_req();
}

void create_session(MME &mme){

	mme.process_autn();
	if(!mme.success){
		return;
	}
	mme.set_sgw_details(mme.ue_num);
	mme.set_pgw_details(mme.ue_num);
	mme.create_session_req_to_sgw();
	mme.create_session_res_from_sgw();
	if(!mme.success){
		return; // Added for uniformity
	}
}

void modify_session(MME &mme){

	mme.store_enodeb_data();
	mme.modify_session_req_to_sgw();
	mme.modify_session_res_from_sgw();
	if(!mme.success){
		return;
	}
	mme.send_attach_res();
}

void detach_process(MME &mme){
	
	if(mme.subtype == 1){
		delete_session(mme);
	}
	else{
		cout << "Incorrect type - " << mme.type << endl;
	}
}

void delete_session(MME &mme){

	mme.delete_session_req_to_sgw();
	mme.delete_session_res_from_sgw();
	mme.send_detach_res();
	if(mme.success == 1){
		mme.delete_session_data();
	}
}

void startup_mme(char *argv[]){

	g_tcount = atoi(argv[1]);
	g_tid.resize(g_tcount);
	g_mme_server.bind_server(g_mme_port, g_mme_addr.c_str());
	g_mme_server.print_status("MME");
}

int main(int argc, char *argv[]) {
	int status;
	int i;
	
	check_server_usage(argc, argv);
	startup_mme(argv);
	setup_mme_data();

	for (i = 0; i < g_tcount; i++) {
		status = pthread_create(&g_tid[i], NULL, process_reqs, NULL);
		report_error(status);
	}
	
	for (i = 0; i < g_tcount; i++) {
		pthread_join(g_tid[i], NULL);
	}		
	
	free_mme_data();
	return 0;
}
