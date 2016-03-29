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

Pgw::~Pgw() {

}

