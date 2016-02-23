#include "utils.h"

int g_reuse = 1;

int g_freeport = 0;
int g_enodeb_port = 4000;
int g_mme_port = 5000;
int g_hss_port = 6000;
int g_sgw1_port = 7000;
int g_pgw_port = 8000;
int g_pgw_dlink_port = 9000;
int g_public_sink_port = 10000;

// string g_enodeb_addr = "10.14.13.29";
// string g_mme_addr = "10.14.13.29";
// string g_hss_addr = "10.14.13.29";
// string g_sgw1_addr = "10.14.13.29";
// string g_pgw_addr = "10.14.13.29";
// string g_pgw_dlink_addr = "10.14.13.29";
// string g_public_sink_addr = "10.129.26.169";
// string g_private_sink_addr = "172.16.0.2";

string g_enodeb_addr = "192.168.1.82";
string g_mme_addr = "192.168.1.72";
string g_hss_addr = "192.168.1.80";
string g_sgw1_addr = "192.168.1.74";
string g_pgw_addr = "192.168.1.76";
string g_pgw_dlink_addr = "192.168.1.76";
string g_public_sink_addr = "192.168.1.78";
string g_private_sink_addr = "172.16.0.2";

socklen_t g_addr_len = sizeof(sockaddr_in);

struct timeval g_timeout = {1, 0};

// int g_sgw2_port = 7100;
// int g_sgw3_port = 7200;
// string g_sgw2_addr = "192.168.1.74";
// string g_sgw3_addr = "192.168.1.74";
// int g_fail_count = 0;

void report_error(int arg) {

	if (arg < 0){
		perror("ERROR");
		cout << "Killing the thread." << endl;
		pthread_exit(NULL);			
	}
}

void report_error(int arg, const char *message) {

	if (arg < 0){
		perror(message);
		cout << "Killing the thread." << endl;
		pthread_exit(NULL);	
	}
}

void report_error(int arg, bool &success){

	success = 1;
	if(arg<0){
		if(errno == EAGAIN || errno == EWOULDBLOCK){
			success = 0;
		}
		else{
			perror("ERROR");
			cout<<"Killing the thread"<<endl;
			pthread_exit(NULL);
		}
	}
}

void print(string message) {

	cout << message << endl;
}

void print(int arg) {

	cout << arg << endl;
}

void print(string message, int arg) {

	cout << message << " " << arg << endl;
}

void print_message(string message) {

	cout << "***********************" << endl;
	cout << message << endl;
	cout << "***********************" << endl;
}

void print_message(string message, int arg) {

	cout << "***********************" << endl;
	cout << message << " " << arg << endl;
	cout << "***********************" << endl;
}

void print_message(string message, unsigned long long arg) {

	cout << "***********************" << endl;
	cout << message << " " << arg << endl;
	cout << "***********************" << endl;
}

const char* to_char_array(unsigned long long arg) {
	string tem;
	stringstream out;

	out << arg;
	tem = out.str();
	const char *ans = tem.c_str();
	return ans;
}

char* allocate_str_mem(int len) {
	char *tem;

	if (len <= 0) {
		print("ERROR: Given_memory_length<=0");
		exit (EXIT_FAILURE);
	}
	tem = (char*)malloc(len * sizeof (char));
	if (tem != NULL) {
		memset(tem, 0, len * sizeof (char));
		return tem;
	}
	else {
		print("ERROR: Memory allocation failure");
		exit(EXIT_FAILURE);
	}
}

uint8_t* allocate_uint8_mem(int len) {
	uint8_t *tem;

	if (len <= 0) {
		print("ERROR: Given_memory_length<=0");
		exit(EXIT_FAILURE);
	}
	tem = (uint8_t*)malloc(len * sizeof (uint8_t));
	if (tem != NULL) {
		memset(tem, 0, len * sizeof (uint8_t));
		return tem;
	} 
	else {
		print("ERROR: Memory allocation failure");
		exit (EXIT_FAILURE);
	}
}

struct ip* allocate_ip_mem(int len) {
	struct ip *ip_hdr;

	if (len <= 0) {
		print("ERROR: Given_memory_length<=0");
		exit(EXIT_FAILURE);
	}
	ip_hdr = (ip*)malloc(len * sizeof (uint8_t));
	if (ip_hdr != NULL) {
		memset(ip_hdr, 0, len * sizeof (uint8_t));
		return ip_hdr;
	} 
	else {
		print("ERROR: Memory allocation failure");
		exit (EXIT_FAILURE);
	}
}

struct tcphdr* allocate_tcp_mem(int len) {
	struct tcphdr *tcp_hdr;

	if (len <= 0) {
		print("ERROR: Given_memory_length<=0");
		exit(EXIT_FAILURE);
	}
	tcp_hdr = (tcphdr*)malloc(len * sizeof (uint8_t));
	if (tcp_hdr != NULL) {
		memset(tcp_hdr, 0, len * sizeof (uint8_t));
		return tcp_hdr;
	} 
	else {
		print("ERROR: Memory allocation failure");
		exit (EXIT_FAILURE);
	}
}

void check_server_usage(int argc, char *argv[]) {

	if (argc < 2) {
		cout << "Please try again with the number of threads to be spawn" << endl;
		exit(-1);
	}
	if (atoi(argv[1]) == 0) {
		cout << "Please try again with a valid number of threads to be spawn" << endl;
		exit(-1);
	}
}

void check_client_usage(int argc, char *argv[]) {

	if (argc < 3) {
		cout << "Please enter the correct number of arguments" << endl;
		cout << "Argument-1: Number of threads to be spawn" << endl;
		cout << "Argument-2: Duration of simulation" << endl;
		exit(-1);
	}
	if (atoi(argv[1]) == 0 || atof(argv[2]) == 0) {
		cout << "Please enter valid arguments" << endl;
		cout << "Argument-1: Number of threads to be spawn" << endl;
		cout << "Argument-2: Duration of simulation" << endl;
		exit(-1);
	}
}

void time_check(time_t &start_time, double &duration_time, bool &time_exceeded) {
	double elapsed_time;

	if ((elapsed_time = difftime(time(0), start_time)) > duration_time) {
		time_exceeded = true;
	}
}

void handle_exceptions() {

	cout << "Exception: Possible because of garbage data" << endl;
	pthread_exit(NULL);
}

int create_udp_socket(){
	int sockfd;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	report_error(sockfd, "Error in creating socket");
	return sockfd;	
}
