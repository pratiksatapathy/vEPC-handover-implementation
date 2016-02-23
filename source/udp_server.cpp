#include "udp_server.h"

UDPServer::UDPServer() {
	
	server_socket = -1;
	signal(SIGPIPE, SIG_IGN);	  
}

void UDPServer::bind_server(int arg_server_port, const char *arg_server_addr) {

	server_socket = create_udp_socket();
	status = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &g_reuse, sizeof(int));
	report_error(status, "At UDPClient side - Error in setting socket options");	
	server_addr.assign(arg_server_addr);
	server_port = arg_server_port;
	bzero((char *) &server_sock_addr, sizeof(server_sock_addr));
	server_sock_addr.sin_family = AF_INET;  	
	server_sock_addr.sin_port = htons(server_port);
	status = inet_aton(arg_server_addr, &server_sock_addr.sin_addr);	
	if (status == 0) {
		cout << "ERROR: Invalid IP address" << endl;
		exit(EXIT_FAILURE);
	}
	status = bind(server_socket, (struct sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
	report_error(status);	
}

void UDPServer::write_data(struct sockaddr_in &client_sock_addr, Packet &pkt){
	string text;

	while(1){
		status = sendto(server_socket, pkt.data, pkt.data_len, 0, (sockaddr*)&client_sock_addr, g_addr_len);
		if(errno == EPERM){
			errno = 0;
			usleep(1000);
			continue;
		}
		else{
			break;
		}
	}
	text = "At server side: Error in sending data - " + to_string(server_socket); 
	report_error(status, text.c_str());
}

void UDPServer::print_status(const char *server_name){
	string arg;

	arg.assign(server_name);
	arg = arg + " SERVER STARTED on port " + to_string(server_port);		
	cout<<"-------------------------------------------"<<endl;
	cout<<arg<<endl;
	cout<<"-------------------------------------------"<<endl;	
}

UDPServer::~UDPServer() {
	
	if (server_socket >= 0)
		close(server_socket);
}
