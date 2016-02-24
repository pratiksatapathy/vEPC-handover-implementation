#ifndef SCTP_SERVER_H
#define SCTP_SERVER_H

#include "utils.h"
#include "packet.h"

class SCTPServer {
public:
	int listen_fd;
	int conn_fd;
	int port;
	string ip_addr;
	struct sockaddr_in sock_addr;
	vector<thread> workers;
	Packet pkt;
	
	pthread_mutex_t mux;
	pthread_cond_t qempty;
	pthread_cond_t qfull;

	int max_qsize;
	int workers_count;
	queue<int> conn_q;	

	SCTPServer(int, string, int);
	void init();
	void bind();
	void create_workers(void (*serve_client)(int));
	void accept_clients();
	void worker_func(void (*serve_client)(int));
	void handle_failure(int, const char*);
	void handle_error(int, const char*);
	void clear_queue();
	~SCTPServer();
};

#endif /* SCTP_SERVER_H */