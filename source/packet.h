#ifndef PACKET_H
#define PACKET_H

#include "utils.h"

#define GTPC_LEN 4
#define GTPU_LEN 4
#define IP_LEN 20
#define TCP_LEN 20

struct GTPc {
	uint16_t cteid;
};

struct GTPu { 
	uint16_t uteid;
};

struct Packet {
	struct GTPc gtpc_hdr;
	struct GTPu gtpu_hdr;

	uint8_t *data;
	int data_len;
	int curr_pos;
	
	Packet();
	Packet(const Packet&);
	friend void swap(Packet&, Packet&);
	Packet& operator=(Packet);
	Packet(Packet&&);

	void add_gtpc_hdr(uint16_t);
	void add_gtpu_hdr(uint16_t);

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

	void add_metadata(int, int, int);	
	void copy_metadata(int&, int&, int&);
	
	void copy_pkts(Packet&, Packet&);
	void copy_frompkt(Packet&);
	void copy_topkt(Packet&);

	void clear_data();

	~Packet();
};

#endif //PACKET_H
