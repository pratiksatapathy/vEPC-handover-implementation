#ifndef DIAMETER_H
#define DIAMETER_H

#include "utils.h"

class Diameter {
public:
	/* 0 - 7 Message Type */
	uint8_t msg_type; 

	/* 8 - 23 Message length (Size of payload excluding Diameter header) */
	uint16_t msg_len;	

	Diameter();
	void init(uint8_t, uint16_t);
	~Diameter();
};

const uint64_t DIAMETER_HDR_LEN = sizeof(Diameter);

#endif /* DIAMETER_H */