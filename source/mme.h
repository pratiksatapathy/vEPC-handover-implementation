#ifndef MME_H
#define MME_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "security.h"
#include "sctp_client.h"
#include "sctp_server.h"
#include "sync.h"
#include "telecom.h"
#include "udp_client.h"
#include "utils.h"

class UeContext {
public:
	/* EMM state 
	 * 0 - Deregistered
	 * 1 - Registered */
	int emm_state; /* EPS Mobililty Management state */

	/* ECM state 
	 * 0 - Disconnected
	 * 1 - Connected 
	 * 2 - Idle */	 
	int ecm_state; /* EPS Connection Management state */

	/* UE id */
	uint64_t imsi; /* International Mobile Subscriber Identity */
	string ip_addr;
	uint32_t enodeb_s1ap_ue_id; /* eNodeB S1AP UE ID */
	uint32_t mme_s1ap_ue_id; /* MME S1AP UE ID */

	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */
	vector<uint64_t> tai_list; /* Tracking Area Identifier list */

	/* UE security context */
	uint64_t ksi_asme; /* Key Selection Identifier for Access Security Management Entity */	
	uint64_t k_asme; /* Key for Access Security Management Entity */	
	uint64_t k_nas_enc; /* Key for NAS Encryption / Decryption */
	uint64_t k_nas_int; /* Key for NAS Integrity check */
	uint64_t nas_enc_algo; /* Idenitifier of NAS Encryption / Decryption */
	uint64_t nas_int_algo; /* Idenitifier of NAS Integrity check */
	uint64_t count;
	uint64_t bearer;
	uint64_t dir;

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

	/* UE Operator network info */
	uint16_t nw_type;
	uint16_t nw_capability;

	/* State of UE */
	uint64_t curr_state;

	UeContext();
	void init(uint64_t, uint32_t, uint32_t, uint64_t, uint16_t);
	~UeContext();
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
	SctpServer server;
	MmeIds mme_ids;
	Crypt crypt;
	Integrity integrity;
	int ue_count;
	unordered_map<uint32_t, uint64_t> table1; /* UE Identification table */
	unordered_map<uint64_t, UeContext> table2; /* UE Context table */

	/* Lock parameters */
	pthread_mutex_t table1_mux; /* Handles table1 and ue_count */
	pthread_mutex_t table2_mux; /* Handles table2 */

	Mme();
	void handle_type1_attach(int, Packet&);
	bool handle_autn(int, Packet&);
	void handle_security_mode_cmd(int, Packet&);
	void setup_crypt_context(uint64_t);
	void setup_integrity_context(uint64_t);
	bool handle_security_mode_complete(int, Packet&);
	void handle_ue_location_update();
	bool check_table1_entry(uint32_t);
	bool check_table2_entry(uint64_t);	
	void rem_table1_entry(uint32_t);
	void rem_table2_entry(uint64_t);
	~Mme();
};

#endif /* MME_H */