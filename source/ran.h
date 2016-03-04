#ifndef RAN_H
#define RAN_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"
#include "sctp_client.h"
#include "udp_client.h"

class RanContext {
public:
	/* EMM and ECM states */
	int emm_state; /* EPS Mobililty Management state */
	int ecm_state; /* EPS Connection Management state */
	
	/* UE id */
	uint64_t imsi; /* International Mobile Subscriber Identity */
	uint64_t guti; /* Globally Unique Temporary Identifier */
	string ip_addr;
	uint32_t enodeb_s1ap_id; /* eNodeB S1AP UE ID */
	uint32_t mme_s1ap_id; /* MME S1AP UE ID */

	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */
	vector<uint64_t> tai_list; /* Tracking Area Identifier list */

	/* UE security context */
	/* To be filled */

	/* EPS info, EPS bearer info */
	uint64_t apn_in_use; /* Access Point Name in Use */
	uint8_t eps_bearer_id; /* Evolved Packet System Bearer ID */
	uint8_t e_rab_id; /* Evolved Radio Access Bearer ID */
	uint32_t s1_teid_ul; /* S1 Tunnel Endpoint Identifier - Uplink */
	uint32_t s1_teid_dl; /* S1 Tunnel Endpoint Identifier - Downlink */
};

class RanIds {
public:


};

class EpcAddrs {
public:
	int mme_port;
	int sgw_port;
	string mme_ip_addr;
	string sgw_ip_addr;
};

class Ran {
public:
	RanContext ran_context;
	RanIds ran_ids;
	EpcAddrs epc_addrs;
	SctpClient to_mme;
	Packet pkt;

	Ran();
	~Ran();
};

#endif /* RAN_H */
