#include "sctp_server.h"

SctpServer::SctpServer() {
	listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
	g_utils.handle_type1_error(listen_fd, "Socket error: sctpserver_sctpserver");
	max_qsize = INT_MAX;
	clear_queue();
	g_sync.mux_init(mux);
	g_sync.cndvar_init(qempty);
	g_sync.cndvar_init(qfull);
}

void SctpServer::clear_queue() {
	while (!conn_q.empty()) {
		conn_q.pop();
	}
}

void SctpServer::run(const char *arg_ip_addr, int arg_port, int arg_workers_count, int serve_client(int)) {
	init(arg_ip_addr, arg_port, arg_workers_count, serve_client);
	create_workers();
	g_nw.set_sock_reuse(listen_fd);
	g_nw.bind_sock(listen_fd, sock_addr);
	accept_clients();
}

void SctpServer::init(const char *arg_ip_addr, int arg_port, int arg_workers_count, int arg_serve_client(int)) {
	int status;

	port = arg_port;
	ip_addr.assign(arg_ip_addr);
	g_nw.set_inet_sock_addr(ip_addr.c_str(), port, sock_addr);
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
		g_sync.mlock(mux);
		if (conn_q.empty()) {
			g_sync.cndwait(qempty, mux);
			g_sync.munlock(mux);
		}
		else {
			conn_fd = conn_q.front();
			conn_q.pop();
			g_sync.cndsignal(qfull);
			g_sync.munlock(mux);
			status = serve_client(conn_fd);
			if  (status == 1) {
				g_sync.mlock(mux);
				while (conn_q.size() >= max_qsize) {
					g_sync.cndwait(qfull, mux);
				}
				conn_q.push(conn_fd);
				g_sync.cndsignal(qempty);
				g_sync.munlock(mux);
			}
			else if (status == 0) {
				close(conn_fd);
			}
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
		g_utils.handle_type1_error(conn_fd, "Accept error: sctpserver_acceptclient");
		g_nw.set_rcv_timeout(conn_fd);
		g_sync.mlock(mux);
		while (conn_q.size() >= max_qsize) {
			g_sync.cndwait(qfull, mux);
		}
		conn_q.push(conn_fd);
		g_sync.cndsignal(qempty);
		g_sync.munlock(mux);
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
	g_utils.handle_type2_error(status, "Write error: sctpserver_snd");
}

void SctpServer::rcv(int conn_fd, Packet &pkt) {
	int status;

	pkt.clear_pkt();
	status = read(conn_fd, pkt.data, BUF_SIZE);
	g_utils.handle_type2_error(status, "Read error: sctpserver_rcv");
	pkt.data_ptr = 0;
	pkt.len = status;
}

SctpServer::~SctpServer() {
	close(listen_fd);
}