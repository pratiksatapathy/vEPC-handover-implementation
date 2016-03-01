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
string g_enodeb_addr = "192.168.1.82";
string g_mme_addr = "192.168.1.72";
string g_hss_addr = "192.168.1.80";
string g_sgw1_addr = "192.168.1.74";
string g_pgw_addr = "192.168.1.76";
string g_pgw_dlink_addr = "192.168.1.76";
string g_public_sink_addr = "192.168.1.78";
string g_private_sink_addr = "172.16.0.2";
socklen_t g_sock_addr_len = sizeof(sockaddr_in);
struct timeval g_timeout = {1, 0};

string ulltos(unsigned long long arg) {
	stringstream out;
	string ans;

	out << arg;
	ans = out.str();
	return ans;
}

char* allocate_str_mem(int len) {
	char *tem;

	if (len <= 0) {
		cout << "Memory length error: allocate_str_mem" << endl;
		exit(EXIT_FAILURE);
	}
	tem = (char*)malloc(len * sizeof (char));
	if (tem != NULL) {
		memset(tem, 0, len * sizeof (char));
		return tem;
	}
	else {
		cout << "Memory allocation error: allocate_str_mem" << endl;
		exit(EXIT_FAILURE);
	}
}

uint8_t* allocate_uint8_mem(int len) {
	uint8_t *tem;

	if (len <= 0) {
		cout << "Memory length error: allocate_uint8_mem" << endl;
		exit(EXIT_FAILURE);
	}
	tem = (uint8_t*)malloc(len * sizeof (uint8_t));
	if (tem != NULL) {
		memset(tem, 0, len * sizeof (uint8_t));
		return tem;
	} 
	else {
		cout << "Memory allocation error: allocate_uint8_mem" << endl;
		exit (EXIT_FAILURE);
	}
}

void time_check(time_t start_time, double duration_time, bool time_exceeded) {
	double elapsed_time;

	if ((elapsed_time = difftime(time(0), start_time)) > duration_time) {
		time_exceeded = true;
	}
}