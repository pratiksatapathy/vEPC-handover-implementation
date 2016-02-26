#ifndef S1AP_H
#define S1AP_H

#include "utils.h"

#define S1AP_HDR_LEN 11

class S1ap {
public:
	/* 0 - 7 Message Type */
	uint8_t msg_type; 

	/* 8 - 23 Message length (Size of payload excluding S1AP header) */
	uint16_t msg_len;	

	/* 24 - 55 S1AP eNodeB UE ID */
	uint32_t enodeB_ue_id;

	/* 56 - 87 S1AP eNodeB UE ID */
	uint32_t mme_ue_id;

	S1ap();
	void init(uint8_t, uint16_t, uint32_t, uint32_t);
	~S1ap();
};

#endif /* S1AP_H */