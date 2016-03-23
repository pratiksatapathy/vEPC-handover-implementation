#ifndef HSS_H
#define HSS_H

#include "diameter.h"
#include "gtp.h"
#include "mysql.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "sctp_server.h"
#include "sync.h"
#include "utils.h"

class Hss {
private:
	pthread_mutex_t mysql_client_mux;

public:
	SctpServer server;
	MySql mysql_client;

	Hss();
	void handle_autn_info_req(int, Packet&);
	~Hss();
};

#endif //HSS_H