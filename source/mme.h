#ifndef MME_H
#define MME_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"
#include "sctp_server.h"
#include "sctp_client.h"
#include "udp_client.h"
#include "telecom.h"

class UeContext {
public:
	/* EMM and ECM states */
	int emm_state; /* EPS Mobililty Management state */
	int ecm_state; /* EPS Connection Management state */

	/* UE id */
	uint64_t imsi; /* International Mobile Subscriber Identity */
	string ip_addr;
	uint32_t enodeb_s1ap_id; /* eNodeB S1AP UE ID */
	uint32_t mme_s1ap_id; /* MME S1AP UE ID */

	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */
	vector<uint64_t> tai_list; /* Tracking Area Identifier list */

	/* UE security context */
	uint64_t k_asme;

	/* EPS info, EPS bearer info */
	uint64_t default_apn; /* Default Access Point Name */
	uint64_t apn_in_use; /* Access Point Name in Use */
	uint8_t eps_bearer_id; /* Evolved Packet System Bearer ID */
	uint32_t s1_teid_ul; /* S1 Tunnel Endpoint Identifier - Uplink */
	uint32_t s1_teid_dl; /* S1 Tunnel Endpoint Identifier - Downlink */
	uint32_t s5_teid_ul; /* S5 Tunnel Endpoint Identifier - Uplink */
	uint32_t s5_teid_dl; /* S5 Tunnel Endpoint Identifier - Downlink */

	/* Authentication Info */ 
	uint64_t xres;
};

class MmeIds {
public:
	uint16_t mcc; /* Mobile Country Code */
	uint16_t mnc; /* Mobile Network Code */
	uint16_t plmn_id; /* Public Land Mobile Network ID */
	uint16_t mmegi; /* MME Group Identifier */
	uint8_t mmec; /* MME Code */
	uint32_t mmei; /* MME Identifier */
	uint64_t gummei; /* Globally Unique MME Identifier */

	MmeIds();
	~MmeIds();
};

class Mme {
public:
	MmeIds mme_ids;
	uint64_t ue_count;
	unordered_map<uint32_t, uint64_t> ue_identification_table;
	unordered_map<uint64_t, UeContext> ue_context_table; 

	/* Lock parameters */
	pthread_mutex_t ue_identification_table_mux;
	pthread_mutex_t ue_context_table_mux;

	Mme();
	void handle_type1_attach(Packet&);
	void add_ue_context(Packet);
	void handle_authentication(Packet&);
	~Mme();
};

#endif /* MME_H */