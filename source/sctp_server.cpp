#include "sctp_server.h"

SCTPServer::SCTPServer() {
	clear_queue();	
	max_qsize = INT_MAX;
	pthread_mutex_init(&mux, NULL);
	pthread_cond_init(&qempty, NULL);
	pthread_cond_init(&qfull, NULL);
	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	handle_failure(listen_fd, "Socket error");
}

void SCTPServer::run(const char *arg_ip_addr, int arg_port, int arg_workers_count, void serve_client(int)) {
	init(arg_port, arg_ip_addr, arg_workers_count, serve_client);
	create_workers();
	bind_server();
	accept_clients();
}

void SCTPServer::init(const char *arg_ip_addr, int arg_port, int arg_workers_count, void arg_serve_client(int)) {
	int status;

	port = arg_port;
	ip_addr.assign(arg_ip_addr);
	bzero((void*)&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	status = inet_aton(ip_addr.c_str(), &sock_addr.sin_addr);	
	if (status == 0) {
		cout << "inet_aton error" << endl;
		exit(EXIT_FAILURE);
	}	
	workers_count = arg_workers_count;	
	workers.resize(workers_count);
	serve_client = arg_serve_client;
}

void SCTPServer::create_workers() {
	int i;

	for (i = 0; i < workers_count; i++) {
		workers[i] = thread(&SCTPServer::worker_func, this);
	}	
}

void SCTPServer::worker_func() {
	int status;
	int conn_fd;

	while (1) {
		status = pthread_mutex_lock(&mux);
		handle_failure(status, "Lock error");
		if(conn_q.empty()) {
			status = pthread_cond_wait(&qempty, &mux);
			handle_failure(status, "Condition wait error - Queue empty");
			status = pthread_mutex_unlock(&mux);
			handle_failure(status, "Unlock error");	
		}
		else {
			conn_fd = conn_q.front();
			conn_q.pop();
			status = pthread_cond_signal(&qfull);
			handle_failure(status, "Condition signal error - Queue full");
			status = pthread_mutex_unlock(&mux);
			handle_failure(status, "Unlock error");	
			serve_client(conn_fd);
			close(conn_fd);
		}
	}
}

void SCTPServer::bind_server() {
	int status;
	
	status = bind(listen_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
	handle_failure(status, "Bind error");
}

void SCTPServer::accept_clients() {
	int conn_fd;
	int status;
	socklen_t sock_addr_len;
	struct sockaddr_in client_sock_addr;

	listen(listen_fd, 500);
	cout << "Server started!" << endl;
	while (1) {
		conn_fd = accept(listen_fd, (struct sockaddr *)&client_sock_addr, &sock_addr_len);
		handle_failure(conn_fd, "Accept error");
		status = pthread_mutex_lock(&mux);
		handle_failure(status, "Lock error");
		if(conn_q.size() >= max_qsize) {
			status = pthread_cond_wait(&qfull, &mux);
			handle_failure(status, "Condition wait error - Queue full");
		}
		conn_q.push(conn_fd);
		status = pthread_cond_signal(&qempty);
		handle_failure(status, "Condition signal error - Queue empty");
		status = pthread_mutex_unlock(&mux);
		handle_failure(status, "Unlock error");		
	}	
}

void SCTPServer::clear_queue() {
	while (!conn_q.empty()) {
		conn_q.pop();
	}
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
	string msg(c_msg);
	msg = msg + ": SCTP server";
	if (arg < 0) {
		perror(msg.c_str());
	}
}

SCTPServer::~SCTPServer() {
	int i;

	for (i = 0; i < workers_count; i++) {
		if (workers[i].joinable()) {
			workers[i].join();
		}
	}
	close(listen_fd);
}