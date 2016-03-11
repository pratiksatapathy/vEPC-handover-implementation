#ifndef PACKET_H
#define PACKET_H

/* (C) ip_hdr */
#include <netinet/ip.h>

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"

#define BUF_SIZE 1024
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
	void append_item(int);
	void append_item(uint16_t);
	void append_item(uint64_t);
	void append_item(uint8_t*, int);	
	void append_item(const char*);	
	void append_item(string);
	void prepend_gtpc_hdr(uint8_t, uint16_t, uint32_t);
	void prepend_gtpu_hdr(uint8_t, uint16_t, uint32_t);
	void prepend_s1ap_hdr(uint8_t, uint16_t, uint32_t, uint32_t);
	void prepend_diameter_hdr(uint8_t, uint16_t);
	void extract_item(int&);
	void extract_item(uint16_t&);
	void extract_item(uint64_t&);
	void extract_item(uint8_t*, int);	
	void extract_item(char*, int);	
	void extract_item(string&, int);
	void extract_gtpc_hdr();
	void extract_gtpu_hdr();
	void extract_s1ap_hdr();
	void extract_diameter_hdr();
	void clear_pkt();
	~Packet();
};

struct ip* allocate_ip_hdr_mem(int);

#endif /* PACKET_H */
