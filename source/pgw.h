#ifndef PGW_H
#define PGW_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "sync.h"
#include "udp_client.h"
#include "udp_server.h"
#include "utils.h"

class UeContext {
public:
	/* UE id */
	uint64_t imsi; /* International Mobile Subscriber Identity */
	string ip_addr;	

	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */

	/* EPS info, EPS bearer info */
	uint64_t apn_in_use; /* Access Point Name in Use */
	uint32_t s5_uteid_ul; /* S5 Userplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_uteid_dl; /* S5 Userplane Tunnel Endpoint Identifier - Downlink */

	/* Control plane info */
	uint32_t s5_cteid_ul; /* S5 Controlplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_cteid_dl; /* S5 Controlplane Tunnel Endpoint Identifier - Downlink */

	UeContext();
	~UeContext();
};

class Pgw {
private:

public:
	UdpServer s5_server;
	UdpServer sgi_server;
	unordered_map<uint8_t, UeContext> table1; /* UE Context table */

	/* Lock parameter */
	pthread_mutex_t table1_mux; /* Handles table1 */

	Pgw();
	~Pgw();
};

#endif /* PGW_H */