#ifndef GTP_H
#define GTP_H

#include "utils.h"

#define GTPV1_HDR_LEN 12
#define GTPV2_HDR_LEN 12

/* Used in user plane */
class Gtpv1 {
public:
	/* 
	0 - 2 Version (GTPv1 - 1)
	3 Protocol type (GTP - 1)
	4 Reserved (0)
	5 Externsion Header flag (0)
	6 Sequence number (0)
	7 N-PDU Flag number (0)
	*/
	uint8_t flags;	

	/* 8 - 15 Message Type */
	uint8_t msg_type; 

	/* 16 - 31 Message length (Size of payload excluding GTP header) */
	uint16_t msg_len; 

	/* 32 - 63 TEID (Dummy TEID) */
	uint32_t teid;

	/* 64 - 79 Sequence number (0) */
	uint16_t seq_num;

	/* 80 - 87 N-PDU number (0) */
	uint8_t npdu_num;

	/* 88 - 95 Next Extension Header type (0) */
	uint8_t next_ext_header_type;

	Gtpv1();
	void init(uint8_t, uint16_t, uint32_t);
	~Gtpv1();
};

/* Used in control plane */
class Gtpv2 {
public:
	/* 
	0 - 2 Version (GTPv2 - 2)
	3 Piggybacking (0)
	4 TEID (0 / 1)
	5 - 7 Spare (0)
	*/
	uint8_t flags;	

	/* 8 - 15 Message Type */
	uint8_t msg_type; 

	/* 16 - 31 Message length (Size of payload excluding GTP header) */
	uint16_t msg_len; 

	/* 32 - 63 TEID (Dummy TEID) */
	uint32_t teid;

	/* 64 - 87 Sequence number (0) */
	uint16_t seq_num;

	/* 88 - 95 Spare (0) */
	uint8_t spare;

	Gtpv2();
	void init(uint8_t, uint16_t, uint32_t);
	~Gtpv2();
};

#endif /* GTP_H */