#ifndef SCTP_SERVER_H
#define SCTP_SERVER_H

#include "utils.h"
#include "packet.h"

class SCTPServer {
public:
	/* Address parameters */
	int port;
	int listen_fd;
	string ip_addr;
	struct sockaddr_in sock_addr;

	/* Thread pool parameters */
	int max_qsize;
	int workers_count;
	queue<int> conn_q;	
	vector<thread> workers;	
	void (*serve_client)(int);

	/* Lock and signal parameters */
	pthread_mutex_t mux;
	pthread_cond_t qempty;
	pthread_cond_t qfull;

	SCTPServer();
	void run(const char*, int, int, void (*)(int));
	void init(const char*, int, int, void (*)(int));
	void create_workers();
	void worker_func();
	void bind_server();
	void accept_clients();
	void clear_queue();
	void handle_failure(int, const char*);
	void handle_error(int, const char*);	
	~SCTPServer();
};

#endif /* SCTP_SERVER_H */