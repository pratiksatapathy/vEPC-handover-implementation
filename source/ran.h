#ifndef RAN_H
#define RAN_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "security.h"
#include "sctp_client.h"
#include "telecom.h"
#include "udp_client.h"
#include "utils.h"

class RanContext {
public:
	/* EMM state 
	 * 0 - Deregistered
	 * 1 - Registered 
	 */	
	int emm_state; /* EPS Mobililty Management state */

	/* ECM state 
	 * 0 - Disconnected
	 * 1 - Connected 
	 * 2 - Idle 
	 */
	int ecm_state; /* EPS Connection Management state */

	/* UE id */
	uint64_t imsi; /* International Mobile Subscriber Identity */
	uint64_t guti; /* Globally Unique Temporary Identifier */
	string ip_addr;
	uint32_t enodeb_s1ap_ue_id; /* eNodeB S1AP UE ID */
	uint32_t mme_s1ap_ue_id; /* MME S1AP UE ID */

	/* UE location info */
	uint64_t tai; /* Tracking Area Identifier */
	vector<uint64_t> tai_list; /* Tracking Area Identifier list */

	/* UE security context */
	uint64_t key; /* Primary key used in generating secondary keys */
	uint64_t k_asme; /* Key for Access Security Management Entity */
	uint64_t ksi_asme; /* Key Selection Identifier for Access Security Management Entity */
	uint64_t k_nas_enc; /* Key for NAS Encryption / Decryption */
	uint64_t k_nas_int; /* Key for NAS Integrity check */
	uint64_t nas_enc_algo; /* Idenitifier of NAS Encryption / Decryption */
	uint64_t nas_int_algo; /* Idenitifier of NAS Integrity check */
	uint64_t count;
	uint64_t bearer;
	uint64_t dir;


	/* EPS info, EPS bearer info */
	uint64_t apn_in_use; /* Access Point Name in Use */
	uint8_t eps_bearer_id; /* Evolved Packet System Bearer ID */
	uint8_t e_rab_id; /* Evolved Radio Access Bearer ID */
	uint32_t s1_teid_ul; /* S1 Tunnel Endpoint Identifier - Uplink */
	uint32_t s1_teid_dl; /* S1 Tunnel Endpoint Identifier - Downlink */

	/* Network Operator info */
	uint16_t mcc; /* Mobile Country Code */
	uint16_t mnc; /* Mobile Network Code */
	uint16_t plmn_id; /* Public Land Mobile Network ID */	
	uint64_t msisdn; /* Mobile Station International Subscriber Directory Number - Mobile number */
	uint16_t nw_capability;

	RanContext();
	void init(uint32_t);
	~RanContext();	
};

class EpcAddrs {
public:
	int mme_port;
	int sgw_port;
	string mme_ip_addr;
	string sgw_ip_addr;

	EpcAddrs();
	~EpcAddrs();
};

class Ran {
public:
	RanContext ran_context;
	EpcAddrs epc_addrs;
	SctpClient mme_client;
	Crypt crypt;
	Integrity integrity;
	Packet pkt;

	Ran();
	void init(int);
	void initial_attach();
	void authenticate();
	void setup_security();
	void setup_crypt_context();
	void setup_integrity_context();
	void setup_eps_session();
	void transfer_data();
	void detach();	
	~Ran();
};

#endif /* RAN_H */
