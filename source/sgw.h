#ifndef SGW_H
#define SGW_H

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

	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */

	/* EPS info, EPS bearer info */
	uint64_t apn_in_use; /* Access Point Name in Use */
	uint32_t s1_uteid_ul; /* S1 Userplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s1_uteid_dl; /* S1 Userplane Tunnel Endpoint Identifier - Downlink */
	uint32_t s5_uteid_ul; /* S5 Userplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_uteid_dl; /* S5 Userplane Tunnel Endpoint Identifier - Downlink */

	/* PGW info */
	uint64_t pgw_port;
	string pgw_ip_addr;

	/* Control plane info */
	uint32_t s11_cteid_mme; /* S11 Controlplane Tunnel Endpoint Identifier - MME */
	uint32_t s11_cteid_sgw; /* S11 Controlplane Tunnel Endpoint Identifier - SGW */
	uint32_t s5_cteid_ul; /* S5 Controlplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_cteid_dl; /* S5 Controlplane Tunnel Endpoint Identifier - Downlink */

	UeContext();
	~UeContext();
};

class Sgw {
private:
	bool check_table1_entry(uint8_t);
	void rem_table1_entry(uint8_t);

public:
	UdpServer s11_server;
	UdpServer s1_server;
	UdpServer s5_server;
	unordered_map<uint8_t, UeContext> table1; /* UE Context table */

	/* Lock parameter */
	pthread_mutex_t table1_mux; /* Handles table1 */

	Sgw();
	~Sgw();
};

#endif /* SGW_H */