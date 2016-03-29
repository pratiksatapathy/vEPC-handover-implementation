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
	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */

	/* EPS session info */
	uint64_t apn_in_use; /* Access Point Name in Use */

	/* EPS Bearer info */
	uint8_t eps_bearer_id; /* Evolved Packet System Bearer Id */
	uint32_t s1_uteid_ul; /* S1 Userplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s1_uteid_dl; /* S1 Userplane Tunnel Endpoint Identifier - Downlink */
	uint32_t s5_uteid_ul; /* S5 Userplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_uteid_dl; /* S5 Userplane Tunnel Endpoint Identifier - Downlink */
	uint32_t s11_cteid_mme; /* S11 Controlplane Tunnel Endpoint Identifier - MME */
	uint32_t s11_cteid_sgw; /* S11 Controlplane Tunnel Endpoint Identifier - SGW */
	uint32_t s5_cteid_ul; /* S5 Controlplane Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_cteid_dl; /* S5 Controlplane Tunnel Endpoint Identifier - Downlink */

	/* PGW info */
	string pgw_s5_ip_addr;
	uint64_t pgw_s5_port;

	UeContext();
	void init(uint64_t, uint64_t, uint8_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, string, int);
	~UeContext();
};

class Sgw {
private:
	uint64_t get_imsi(Packet);

public:
	UdpServer s11_server;
	UdpServer s1_server;
	UdpServer s5_server;
	unordered_map<uint32_t, uint64_t> table1; /* UE identification table: s11_cteid_sgw -> imsi */
	unordered_map<uint32_t, UeContext> table2; /* UE context table: imsi -> UeContext */
	unordered_map<uint32_t, uint32_t> table3; /* SGW -> eNodeB TEID mapping table */
	unordered_map<uint32_t, uint32_t> table4; /* SGW -> PGW TEID mapping table */

	/* Lock parameter */
	pthread_mutex_t table1_mux; /* Handles table1 */
	pthread_mutex_t table234_mux; /* Handles table2, table3, table4 */

	Sgw();
	void handle_create_session(struct sockaddr_in, Packet);
	void handle_modify_bearer(struct sockaddr_in, Packet);
	~Sgw();
};

#endif /* SGW_H */