#ifndef PACKET_H
#define PACKET_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"

#define IP_HDR_LEN 20
#define TCP_HDR_LEN 20

class Packet {
public:
	Gtpv1 gtpu_hdr;
	Gtpv2 gtpc_hdr;
	S1ap s1ap_hdr;
	Diameter diameter_hdr;
	uint8_t *data;
	int data_pos;
	int len;
	
	Packet();
	Packet(const Packet&);
	friend void swap(Packet&, Packet&);
	Packet& operator=(Packet);
	Packet(Packet&&);


	void add_gtpc_hdr(uint16_t);
	void add_gtpu_hdr(uint16_t);
	void add_s1ap_hdr();

	void add_data(int);
	void add_data(uint16_t);
	void add_data(unsigned long long);
	void add_data(uint8_t*, int);	
	void add_data(const char*);	
	void add_data(string);

	void copy_gtpc_hdr();
	void copy_gtpu_hdr();

	void copy_data(int&);
	void copy_data(uint16_t&);
	void copy_data(unsigned long long&);
	void copy_data(uint8_t*, int&);	
	void copy_data(char*, int&);	
	void copy_data(string&, int&);

	
	void clear_data();

	~Packet();
};

#endif //PACKET_H
