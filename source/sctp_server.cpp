#include "sctp_server.h"

SCTPServer::SCTPServer(int arg_port, arg_ip_addr, int arg_workers_count) {
	port = arg_port;
	ip_addr = arg_ip_addr;	
	workers_count = arg_workers_count;
}

void SCTPServer::init() {
	clear_queue();	
	max_queue_size= INT_MAX;
	workers.resize(workers_count);
	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	handle_failure(listen_fd, "Socket error");
}

void SCTPServer::bind() {
	bzero((void*)&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	status = inet_aton(ip_addr, &sock_addr.sin_addr);	
	if (status == 0) {
		cout << "inet_aton error: SCTP server" << endl;
		exit(EXIT_FAILURE);
	}	
	bind(listen_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
}

void SCTPServer::create_workers(void (*serve_client)(int)) {
	for(i = 0; i < workers_count; i++) {
		workers[i] = thread(worker_func, serve_client);
	}
}

void SCTPServer::accept_clients() {
	int conn_fd;

	listen(listen_fd, 500);
	for(i = 1;; i++) {
		conn_fd = accept(listen_fd, (struct sockaddr *)&client_sock_addr, &g_sock_addr_len);
		handle_failure(conn_fd, "Accept error");
		status = pthread_mutex_lock(&mux);
		handle_failure(status, "Lock error");
		if(conn_queue.size() >= max_queue_size) {
			cout<<"Waiting"<<endl;
			status = pthread_cond_wait(&queue_full, &mux);
			handle_failure(status, "Condition wait error - Queue full");
		}
		conn_queue.push(conn_fd);
		status = pthread_cond_signal(&queue_empty);
		handle_failure(status, "Condition signal error - Queue empty");
		status = pthread_mutex_unlock(&mux);
		handle_failure(status, "Unlock error");		
	}
	for(i = 0; i < workers_count; i++) {
		workers[i].join();
	}
	close(listen_fd);	
}

void SCTPServer::worker_func(void (*serve_client)(int)) {

}

void SCTPServer::handle_failure(int arg, const char *c_msg) {
	string msg(c_msg);
	msg = msg + ": SCTP server";
	if (arg < 0) {
		perror(msg.c_str());
		exit(-1);
	}
}

void SCTPServer::handle_error(int arg, const char *c_msg) {
	string msg(cmsg);
	msg = msg + ": SCTP server";
	if (arg < 0) {
		perror(msg.c_str());
	}
}

void SCTPServer::clear_queue() {
	while (!conn_q.empty()) {
		conn_q.pop();
	}
}

SCTPServer::~SCTPServer() {

}