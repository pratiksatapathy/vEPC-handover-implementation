#include "utils.h"

int g_reuse = 1;
int g_freeport = 0;
int g_enodeb_port = 4000;
int g_mme_port = 5000;
int g_hss_port = 6000;
int g_sgw_port = 7000;
int g_pgw_port = 8000;
int g_pgw_dlink_port = 9000;
int g_public_sink_port = 10000;
// string g_enodeb_ip_addr = "192.168.1.82";
// string g_mme_ip_addr = "192.168.1.72";
// string g_hss_ip_addr = "192.168.1.80";
// string g_sgw1_ip_addr = "192.168.1.74";
// string g_pgw_ip_addr = "192.168.1.76";
// string g_pgw_dlink_ip_addr = "192.168.1.76";
// string g_public_sink_ip_addr = "192.168.1.78";
// string g_private_sink_ip_addr = "172.16.0.2";
string g_enodeb_ip_addr = "127.0.0.1";
string g_mme_ip_addr = "127.0.0.1";
string g_hss_ip_addr = "127.0.0.1";
string g_sgw_ip_addr = "127.0.0.1";
string g_pgw_ip_addr = "127.0.0.1";
string g_pgw_dlink_ip_addr = "127.0.0.1";
string g_public_sink_ip_addr = "127.0.0.1";
string g_private_sink_ip_addr = "127.0.0.1";
struct timeval g_timeout = {20, 0};

/* Action - Exit the program */
void handle_type1_error(int arg, const char *msg) {
	if (arg < 0) {
		perror(msg);
		exit(EXIT_FAILURE);
	}	
}

/* Action - Nothing */
void handle_type2_error(int arg, const char *msg) {
	if (arg < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

char* allocate_str_mem(int len) {
	char *tem;

	if (len <= 0) {
		handle_type1_error(-1, "Memory length error: utils_allocatestrmem");
	}
	tem = (char*)malloc(len * sizeof (char));
	if (tem != NULL) {
		memset(tem, 0, len * sizeof (char));
		return tem;
	}
	else {
		handle_type1_error(-1, "Memory allocation error: utils_allocatestrmem");
	}
}

uint8_t* allocate_uint8_mem(int len) {
	uint8_t *tem;

	if (len <= 0) {
		handle_type1_error(-1, "Memory length error: utils_allocateuint8mem");
	}
	tem = (uint8_t*)malloc(len * sizeof (uint8_t));
	if (tem != NULL) {
		memset(tem, 0, len * sizeof (uint8_t));
		return tem;
	} 
	else {
		handle_type1_error(-1, "Memory allocation error: utils_allocateuint8mem");
	}
}

void time_check(time_t start_time, double duration_time, bool &time_exceeded) {
	double elapsed_time;

	if ((elapsed_time = difftime(time(0), start_time)) > duration_time) {
		time_exceeded = true;
	}
}