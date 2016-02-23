#include "packet.h"

Packet::Packet() {
	
	gtpc_hdr.cteid = -1;
	gtpu_hdr.uteid = -1;
	data = allocate_uint8_mem(BUF_SIZE);
	data_len = 0;
	curr_pos = 0;
}

Packet::Packet(const Packet &src_obj) {
	
	data = allocate_uint8_mem(BUF_SIZE);
	gtpc_hdr = src_obj.gtpc_hdr;
	gtpu_hdr = src_obj.gtpu_hdr;
	memmove(data, src_obj.data, src_obj.data_len);
	data_len = src_obj.data_len;
	curr_pos = src_obj.curr_pos;
}

void swap(Packet &src_obj, Packet &dst_obj) {
	using std::swap;

	swap(src_obj.gtpc_hdr, dst_obj.gtpc_hdr);
	swap(src_obj.gtpu_hdr, dst_obj.gtpu_hdr);
	swap(src_obj.data, dst_obj.data);
	swap(src_obj.data_len, dst_obj.data_len);
	swap(src_obj.curr_pos, dst_obj.curr_pos);
}

Packet& Packet::operator=(Packet src_obj) {
	
	swap(*this, src_obj);
	return *this;	
}

Packet::Packet(Packet &&src_obj)
	:Packet() {

	swap(*this, src_obj);	
}

void Packet::add_gtpc_hdr(uint16_t cteid) {

	add_data(cteid);
}

void Packet::add_gtpu_hdr(uint16_t uteid) {

	add_data(uteid);
}

void Packet::add_data(int arg) {
	int len = sizeof(int);

	memmove(data + curr_pos, &arg, len * sizeof(uint8_t));
	data_len += len;
	curr_pos += len;
}

void Packet::add_data(uint16_t arg) {
	int len = sizeof(uint16_t);
	
	memmove(data + curr_pos, &arg, len * sizeof(uint8_t));
	data_len += len;
	curr_pos += len;
}

void Packet::add_data(unsigned long long arg) {
	int len = sizeof(unsigned long long);

	memmove(data + curr_pos, &arg, len * sizeof(uint8_t));
	data_len += len;
	curr_pos += len;
}

void Packet::add_data(uint8_t *message, int len) {
	
	memmove(data + curr_pos, message, len * sizeof(uint8_t));
	data_len += len;
	curr_pos += len;
}

void Packet::add_data(const char *message) {
	int len = strlen(message);
	
	memmove(data + curr_pos, message, len * sizeof(uint8_t));
	data_len += len;
	curr_pos += len;
}

void Packet::add_data(string message) {
	int len = message.size();

	memmove(data + curr_pos, message.c_str(), len * sizeof(uint8_t));
	data_len += len;
	curr_pos += len;
}

void Packet::copy_gtpc_hdr() {

	copy_data(gtpc_hdr.cteid);
}

void Packet::copy_gtpu_hdr() {

	copy_data(gtpu_hdr.uteid);
}

void Packet::copy_data(int &arg){
	int len = sizeof(int);

	memmove(&arg, data + curr_pos, len * sizeof(uint8_t));
	curr_pos += len;
}

void Packet::copy_data(uint16_t &arg){
	int len = sizeof(uint16_t);

	memmove(&arg, data + curr_pos, len * sizeof(uint8_t));
	curr_pos += len;
}

void Packet::copy_data(unsigned long long &arg){
	int len = sizeof(unsigned long long);

	memmove(&arg, data + curr_pos, len * sizeof(uint8_t));
	curr_pos += len;
}

void Packet::copy_data(uint8_t *message, int &len){

	memmove(message, data + curr_pos, len * sizeof(uint8_t));
	curr_pos += len;
}

void Packet::copy_data(char *message, int &len){

	memmove(message, data + curr_pos, len * sizeof(uint8_t));
	curr_pos += len;
}

void Packet::copy_data(string &arg, int &len){

	memmove(&arg, data + curr_pos, len * sizeof(uint8_t));
	curr_pos += len;
}

void Packet::add_metadata(int type, int subtype, int ue_num){

	clear_data();
	add_data(type);
	add_data(subtype);
	add_data(ue_num);
}

void Packet::copy_metadata(int &type, int &subtype, int &ue_num){

	copy_data(type);
	copy_data(subtype);
	copy_data(ue_num);
}

void Packet::copy_pkts(Packet &dst, Packet &src) {

	dst.clear_data();
	dst.add_data(src.data, src.data_len);
}

void Packet::copy_frompkt(Packet &src) {

	this->clear_data();
	this->add_data(src.data, src.data_len);
}

void Packet::copy_topkt(Packet &dst) {

	dst.clear_data();
	dst.add_data(data, data_len);
}

void Packet::clear_data() {
	int len = BUF_SIZE;
	
	memset(data, 0, len * sizeof (uint8_t));	
	data_len = 0;
	curr_pos = 0;
}

Packet::~Packet() {
	
	free(data);
}
