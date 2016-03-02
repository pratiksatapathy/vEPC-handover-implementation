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

struct UeContext {
	/* UE id */
	uint64_t imsi;
	string ip_addr;
	uint32_t enodeb_s1ap_id;
	uint32_t mme_s1ap_id;

	/* UE location info */
	uint64_t tai;
	vector<uint64_t> tai_list;

	/* UE security context */
	/* To be filled */

	/* EPS info, EPS bearer info */
	uint64_t default_apn;
	uint64_t apn_in_use;
	uint8_t eps_bearer_id;
	uint32_t s1_teid_ul;
	uint32_t s1_teid_dl;
	uint32_t s5_teid_ul;
	uint32_t s5_teid_dl;
};

class Mme {
public:
	uint16_t mcc;
	uint16_t mnc;
	uint16_t plmn_id;
	uint16_t mmegi;
	uint8_t mmec;
	uint32_t mmei;
	uint64_t gummei;
	unordered_map<uint64_t, UeContext> ue_context_table;
	unordered_map<uint32_t, uint64_t> ue_identification_table;

	Mme();
	~Mme();
};

#endif /* MME_H */
