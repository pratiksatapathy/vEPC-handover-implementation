#include "sgw.h"

UeContext::UeContext() {
	imsi = 0; 
	tai = 0; 
	apn_in_use = 0; 
	s1_uteid_ul = 0; 
	s1_uteid_dl = 0; 
	s5_uteid_ul = 0; 
	s5_uteid_dl = 0; 
	pgw_port = 0;
	string pgw_ip_addr = "";
	s11_cteid_mme = 0;
	s11_cteid_sgw = 0;
	s5_cteid_ul = 0;
	s5_cteid_dl = 0;
}

UeContext::~UeContext() {

}

Sgw::Sgw() {
	g_sync.mux_init(table1_mux);	
}

bool Sgw::check_table1_entry(uint8_t arg_eps_bearer_id) {
	bool res;

	g_sync.mlock(table1_mux);
	if (table1.find(arg_eps_bearer_id) != table1.end()) {
		res = true;
	}
	else {
		res = false;
	}
	g_sync.munlock(table1_mux);
	return res;
}

void Sgw::rem_table1_entry(uint8_t arg_eps_bearer_id) {
	g_sync.mlock(table1_mux);
	table1.erase(arg_eps_bearer_id);
	g_sync.munlock(table1_mux);
}

Sgw::~Sgw() {

}

