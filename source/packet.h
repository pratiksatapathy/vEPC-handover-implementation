#ifndef PACKET_H
#define PACKET_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"

#define IP_HDR_LEN 20

class Packet {
public:
	Gtpv2 gtpc_hdr;
	Gtpv1 gtpu_hdr;
	S1ap s1ap_hdr;
	Diameter diameter_hdr;
	uint8_t *data;
	int data_ptr;
	int len;
	
	Packet();
	Packet(const Packet&);
	friend void swap(Packet&, Packet&);
	Packet& operator=(Packet);
	Packet(Packet&&);
	void add_data(int);
	void add_data(uint16_t);
	void add_data(unsigned long long);
	void add_data(uint8_t*, int);	
	void add_data(const char*);	
	void add_data(string);
	void add_data(Gtpv2);
	void add_data(Gtpv1);
	void add_data(S1ap);
	void add_data(Diameter);
	void add_gtpc_hdr(uint8_t, uint16_t, uint32_t);
	void add_gtpu_hdr(uint8_t, uint16_t, uint32_t);
	void add_s1ap_hdr(uint8_t, uint16_t, uint32_t, uint32_t);
	void add_diameter_hdr(uint8_t, uint16_t);
	void rem_data(int&);
	void rem_data(uint16_t&);
	void rem_data(unsigned long long&);
	void rem_data(uint8_t*, int&);	
	void rem_data(char*, int&);	
	void rem_data(string&, int&);
	void rem_gtpc_hdr();
	void rem_gtpu_hdr();
	void rem_s1ap_hdr();
	void rem_diameter_hdr();
	void clear_data();
	~Packet();
};

#endif //PACKET_H
