#ifndef SCTP_SERVER_H
#define SCTP_SERVER_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "sync.h"
#include "utils.h"

class SctpServer {
private:
	void clear_queue();
	void init(string, int, int, int (*)(int));
	void create_workers();
	void worker_func();
	void accept_clients();
	
public:
	/* Address parameters */
	int listen_fd;
	int port;
	string ip_addr;
	struct sockaddr_in sock_addr;

	/* Thread pool parameters */
	int max_qsize;
	int workers_count;
	queue<int> conn_q;	
	vector<thread> workers;	
	int (*serve_client)(int);

	/* Lock and signal parameters */
	pthread_mutex_t mux;
	pthread_cond_t qempty; /* arg for mwait/msignal - 1 */
	pthread_cond_t qfull; /* arg for mwait/msignal - 2 */

	SctpServer();
	void run(string, int, int, int (*)(int));
	void snd(int, Packet);
	void rcv(int, Packet&);
	~SctpServer();
};

#endif /* SCTP_SERVER_H */