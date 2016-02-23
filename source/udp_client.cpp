#include "udp_client.h"

UDPClient::UDPClient() {
	
	client_socket = -1;
	signal(SIGPIPE, SIG_IGN);			
}

void UDPClient::bind_client() {
	
	client_socket = create_udp_socket();
	status = setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&g_timeout, sizeof(struct timeval));
	report_error(status, "Error in making RECEIVE TIMEOUT");	
	status = setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &g_reuse, sizeof(int));	
	report_error(status, "At UDPClient side - Error in setting socket options");

	bzero((char *) &client_sock_addr, sizeof(client_sock_addr));
	client_sock_addr.sin_family = AF_INET;  
	client_sock_addr.sin_addr.s_addr = INADDR_ANY;	
	client_sock_addr.sin_port = g_freeport;

	status = bind(client_socket, (struct sockaddr*)&client_sock_addr, sizeof(client_sock_addr));
	report_error(status, "Error in binding client socket");	

	status = getsockname(client_socket, (struct sockaddr*)&client_sock_addr, &g_addr_len);
	report_error(status, "At UDPClient side - Error in getting socket name");
	client_port = ntohs(client_sock_addr.sin_port);	
	client_addr = "127.0.0.1";
	//cout << "client binded with port " << client_port << endl;
}

void UDPClient::set_server_details(int arg_server_port, const char *arg_server_addr) {
	
	server_port = arg_server_port;
	server_addr.assign(arg_server_addr);
	bzero((char*)&server_sock_addr, sizeof(server_sock_addr));
	server_sock_addr.sin_family = AF_INET;
	server_sock_addr.sin_port = htons(server_port);
	status = inet_aton(arg_server_addr, &server_sock_addr.sin_addr);
	if (status == 0) {
		cout << "ERROR: In setting server details - Invalid Server IP address " << "***" << server_addr << "*** " << "Size is " << server_addr.size() << endl;
		exit(EXIT_FAILURE);
	}
}

void UDPClient::read_data(bool &success) {
	
	pkt.clear_data();
	status = recvfrom(client_socket, pkt.data, BUF_SIZE, 0, (sockaddr*)&server_sock_addr, &g_addr_len);
	report_error(status, success);
	pkt.data_len = status;
	pkt.curr_pos = 0;
}

void UDPClient::write_data() {
	
	while(1){
		status = sendto(client_socket, pkt.data, pkt.data_len, 0, (sockaddr*)&server_sock_addr, g_addr_len);
		if(errno == EPERM){
			errno = 0;
			usleep(1000);
			continue;
		}
		else{
			break;
		}
	}
	report_error(status, "UDPClient side: Error in writing data");
}

void UDPClient::close_client(){

	if (client_socket >= 0){
		close(client_socket);	
		client_socket = -1;
	}
}

UDPClient::~UDPClient() {
	
	if (client_socket >= 0){
		close(client_socket);
	}
}
