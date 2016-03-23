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
public:
	SctpServer server;
	MySql mysql_client;
	pthread_mutex_t mysql_client_mux;

	Hss();
	void get_ue_info(uint64_t, uint64_t&, uint64_t&);
	void handle_autninfo_req(int, Packet&);
	~Hss();
};

#endif //HSS_H