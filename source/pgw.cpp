#include "pgw.h"

UeContext::UeContext() {
	imsi = 0; 
	ip_addr = "";
	tai = 0; 
	apn_in_use = 0; 
	s5_uteid_ul = 0; 
	s5_uteid_dl = 0; 
	s5_cteid_ul = 0;
	s5_cteid_dl = 0;
}

UeContext::~UeContext() {

}

Pgw::Pgw() {
	g_sync.mux_init(table1_mux);	
}

bool Pgw::check_table1_entry(uint8_t arg_eps_bearer_id) {
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

void Pgw::rem_table1_entry(uint8_t arg_eps_bearer_id) {
	g_sync.mlock(table1_mux);
	table1.erase(arg_eps_bearer_id);
	g_sync.munlock(table1_mux);
}

Pgw::~Pgw() {

}

