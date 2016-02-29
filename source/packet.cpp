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

void Packet::append_item(unsigned long long arg) {
	int data_len = sizeof(unsigned long long);

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

void Packet::append_gtpc_hdr(uint8_t msg_type, uint16_t msg_len, uint32_t teid) {
	int data_len = GTPV2_HDR_LEN;

	gtpc_hdr.init(msg_type, msg_len, teid);
	memmove(data + data_ptr, &gtpc_hdr, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;	
}

void Packet::append_gtpu_hdr(uint8_t msg_type, uint16_t msg_len, uint32_t teid) {
	int data_len = GTPV1_HDR_LEN;

	gtpu_hdr.init(msg_type, msg_len, teid);
	memmove(data + data_ptr, &gtpu_hdr, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_s1ap_hdr(uint8_t msg_type, uint16_t msg_len, uint32_t enodeb_ue_id, uint32_t mme_ue_id) {
	int data_len = S1AP_HDR_LEN;

	s1ap.init(msg_type, msg_len, enodeb_ue_id, mme_ue_id);
	memmove(data + data_ptr, &s1ap_hdr, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
}

void Packet::append_diameter_hdr(uint8_t msg_type, uint16_t msg_len {
	int data_len = DIAMETER_HDR_LEN;

	diameter.init(msg_type, msg_len);
	memmove(data + data_ptr, &diameter_hdr, data_len * sizeof(uint8_t));
	len += data_len;
	data_ptr += data_len;
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

void Packet::extract_item(unsigned long long &arg){
	int data_len = sizeof(unsigned long long);

	memmove(&arg, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(uint8_t *message, int &data_len){
	memmove(message, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(char *message, int &data_len){
	memmove(message, data + data_ptr, data_len * sizeof(uint8_t));
	data_ptr += data_len;
}

void Packet::extract_item(string &arg, int &data_len){
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