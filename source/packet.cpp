#include "packet.h"

Packet::Packet() {
	data = allocate_uint8_mem(BUF_SIZE);
	data_ptr = 0;
	len = 0;
}

Packet::Packet(const Packet &src_obj) {
	gtpc_hdr = src_obj.gtpc_hdr;
	gtpu_hdr = src_obj.gtpu_hdr;
	s1ap_hdr = src_obj.s1ap_hdr;
	diameter_hdr = src_obj.diameter_hdr;
	data = allocate_uint8_mem(BUF_SIZE);
	data_ptr = src_obj.data_ptr;
	len = src_obj.len;
}

void swap(Packet &src_obj, Packet &dst_obj) {
	using std::swap;

	swap(src_obj.gtpc_hdr, dst_obj.gtpc_hdr);
	swap(src_obj.gtpu_hdr, dst_obj.gtpu_hdr);
	swap(src_obj.s1ap_hdr, dst_obj.s1ap_hdr);
	swap(src_obj.diameter_hdr, dst_obj.diameter_hdr);
	swap(src_obj.data, dst_obj.data);
	swap(src_obj.data_ptr, dst_obj.data_ptr);
	swap(src_obj.len, dst_obj.len);
}

Packet& Packet::operator=(Packet src_obj) {
	swap(*this, src_obj);
	return *this;	
}

Packet::Packet(Packet &&src_obj)
	:Packet() {
	swap(*this, src_obj);	
}

void Packet::append_item(int arg) {
	int data_len = sizeof(int);

	memmove(data + data_ptr, &arg, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_item(uint16_t arg) {
	int data_len = sizeof(uint16_t);
	
	memmove(data + data_ptr, &arg, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_item(uint64_t arg) {
	int data_len = sizeof(uint64_t);

	memmove(data + data_ptr, &arg, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_item(uint8_t *message, int data_len) {
	memmove(data + data_ptr, message, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_item(const char *message) {
	int data_len = strlen(message);
	
	memmove(data + data_ptr, message, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_item(string message) {
	int data_len = message.size();

	memmove(data + data_ptr, message.c_str(), data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::prepend_gtpc_hdr(uint8_t msg_type, uint16_t msg_len, uint32_t teid) {
	uint8_t *tem_data = allocate_uint8_mem(BUF_SIZE);
	int data_len = GTPV2_HDR_LEN;

	gtpc_hdr.init(msg_type, msg_len, teid);
	memmove(tem_data, &gtpc_hdr, data_len * sizeof(uint8_t));
	memmove(tem_data + data_len, data, len * sizeof(uint8_t));
	swap(data, tem_data);
	len += data_len;
	data_ptr += data_len;
	free(tem_data);
}

void Packet::prepend_gtpu_hdr(uint8_t msg_type, uint16_t msg_len, uint32_t teid) {
	uint8_t *tem_data = allocate_uint8_mem(BUF_SIZE);	
	int data_len = GTPV1_HDR_LEN;

	gtpu_hdr.init(msg_type, msg_len, teid);
	memmove(tem_data, &gtpu_hdr, data_len * sizeof(uint8_t));
	memmove(tem_data + data_len, data, len * sizeof(uint8_t));
	swap(data, tem_data);
	len += data_len;
	data_ptr += data_len;
	free(tem_data);
}

void Packet::prepend_s1ap_hdr(uint8_t msg_type, uint16_t msg_len, uint32_t enodeb_ue_id, uint32_t mme_ue_id, uint64_t ue_tai) {
	uint8_t *tem_data = allocate_uint8_mem(BUF_SIZE);	
	int data_len = S1AP_HDR_LEN;

	s1ap_hdr.init(msg_type, msg_len, enodeb_ue_id, mme_ue_id, ue_tai);
	memmove(tem_data, &s1ap_hdr, data_len * sizeof(uint8_t));
	memmove(tem_data + data_len, data, len * sizeof(uint8_t));
	swap(data, tem_data);
	len += data_len;
	data_ptr += data_len;
	free(tem_data);
}

void Packet::prepend_diameter_hdr(uint8_t msg_type, uint16_t msg_len) {
	uint8_t *tem_data = allocate_uint8_mem(BUF_SIZE);	
	int data_len = DIAMETER_HDR_LEN;

	diameter_hdr.init(msg_type, msg_len);
	memmove(tem_data, &diameter_hdr, data_len * sizeof(uint8_t));
	memmove(tem_data + data_len, data, len * sizeof(uint8_t));
	swap(data, tem_data);
	len += data_len;
	data_ptr += data_len;
	free(tem_data);
}

void Packet::extract_item(int &arg){
	int data_len = sizeof(int);

	memmove(&arg, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(uint16_t &arg){
	int data_len = sizeof(uint16_t);

	memmove(&arg, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(uint64_t &arg){
	int data_len = sizeof(uint64_t);

	memmove(&arg, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(uint8_t *message, int data_len){
	memmove(message, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(char *message, int data_len){
	memmove(message, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(string &arg, int data_len){
	memmove(&arg, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_gtpc_hdr(){
	int data_len = GTPV2_HDR_LEN;

	memmove(&gtpc_hdr, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_gtpu_hdr(){
	int data_len = GTPV1_HDR_LEN;

	memmove(&gtpu_hdr, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_s1ap_hdr(){
	int data_len = S1AP_HDR_LEN;

	memmove(&s1ap_hdr, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_diameter_hdr(){
	int data_len = DIAMETER_HDR_LEN;

	memmove(&diameter_hdr, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::clear_pkt() {
	int data_len = BUF_SIZE;
	
	memset(data, 0, data_len * sizeof (uint8_t));	
	len = 0;
	data_ptr = 0;
}

Packet::~Packet() {	
	free(data);
}

struct ip* allocate_ip_hdr_mem(int len) {
	struct ip *ip_hdr;

	if (len <= 0) {
		handle_type1_error(-1, "Memory length error: packet_allocateiphdrmem");
	}
	ip_hdr = (ip*)malloc(len * sizeof (uint8_t));
	if (ip_hdr != NULL) {
		memset(ip_hdr, 0, len * sizeof (uint8_t));
		return ip_hdr;
	} 
	else {
		handle_type1_error(-1, "Memory allocation error: packet_allocateiphdrmem");
	}
}