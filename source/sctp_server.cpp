#include "sctp_server.h"

SctpServer::SctpServer() {
	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	handle_type1_error(listen_fd, "Socket error: sctpserver_sctpserver");
	max_qsize = INT_MAX;
	clear_queue();
	pthread_mutex_init(&mux, NULL);
	pthread_cond_init(&qempty, NULL);
	pthread_cond_init(&qfull, NULL);
}

void SctpServer::clear_queue() {
	while (!conn_q.empty()) {
		conn_q.pop();
	}
}

void SctpServer::run(const char *arg_ip_addr, int arg_port, int arg_workers_count, void serve_client(int)) {
	init(arg_ip_addr, arg_port, arg_workers_count, serve_client);
	create_workers();
	set_sock_reuse(listen_fd);
	bind_sock(listen_fd, sock_addr);
	accept_clients();
}

void SctpServer::init(const char *arg_ip_addr, int arg_port, int arg_workers_count, void arg_serve_client(int)) {
	int status;

	port = arg_port;
	ip_addr.assign(arg_ip_addr);
	set_inet_sock_addr(ip_addr.c_str(), port, sock_addr);
	workers_count = arg_workers_count;	
	workers.resize(workers_count);
	serve_client = arg_serve_client;
}

void SctpServer::create_workers() {
	int i;

	for (i = 0; i < workers_count; i++) {
		workers[i] = thread(&SctpServer::worker_func, this);
		workers[i].detach();
	}	
}

void SctpServer::worker_func() {
	int status;
	int conn_fd;

	while (1) {
		mlock();
		if (conn_q.empty()) {
			mwait(qempty);
			munlock();
		}
		else {
			conn_fd = conn_q.front();
			conn_q.pop();
			msignal(qfull);
			munlock();
			serve_client(conn_fd);
			close(conn_fd);
		}
	}
}

void SctpServer::accept_clients() {
	int conn_fd;
	int status;
	socklen_t sock_addr_len;
	struct sockaddr_in client_sock_addr;

	listen(listen_fd, 500);
	while (1) {
		conn_fd = accept(listen_fd, (struct sockaddr *)&client_sock_addr, &sock_addr_len);
		handle_type1_error(conn_fd, "Accept error: sctpserver_acceptclient");
		mlock();
		while (conn_q.size() >= max_qsize) {
			mwait(qfull);
		}
		conn_q.push(conn_fd);
		msignal(qempty);
		munlock();
	}	
}

void SctpServer::snd(int conn_fd,  Packet pkt) {
	int status;

	while (1) {
		status = write(conn_fd, pkt.data, pkt.len);
		if (errno == EPERM) {
			errno = 0;
			usleep(1000);
			continue;
		}
		else {
			break;
		}		
	}
	handle_type2_error(status, "Write error: sctpserver_snd");
}

void SctpServer::rcv(int conn_fd, Packet &pkt) {
	int status;

	pkt.clear_pkt();
	status = read(conn_fd, pkt.data, BUF_SIZE);
	handle_type2_error(status, "Read error: sctpserver_rcv");
	pkt.data_ptr = 0;
	pkt.len = status;
}

void SctpServer::mlock() {
	int status;

	status = pthread_mutex_lock(&mux);
	handle_type1_error(status, "Lock error: sctpserver_mlock");	
}

void SctpServer::munlock() {
	int status;

	status = pthread_mutex_unlock(&mux);
	handle_type1_error(status, "Unlock error: sctpserver_munlock");
}

void SctpServer::mwait(pthread_cond_t &arg_cond_var) {
	int status;

	status = pthread_cond_wait(&arg_cond_var, &mux);
	handle_type1_error(status, "Condition wait error: sctpserver_mwait");		
}

void SctpServer::msignal(pthread_cond_t &arg_cond_var) {
	int status;

	status = pthread_cond_signal(&arg_cond_var);
	handle_type1_error(status, "Condition signal error: sctpserver_msignal");
}

SctpServer::~SctpServer() {
	close(listen_fd);
}