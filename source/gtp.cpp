#include "gtp.h"

Gtpv1::Gtpv1() {
	flags = (3 << 4); /* flags : Binary(00110000) */
	seq_num = 0;
	npdu_num = 0;
	next_ext_header_type = 0;
}

void Gtpv1::init(uint8_t arg_msg_type, uint16_t arg_msg_len, uint32_t arg_teid) {
	msg_type = arg_msg_type;
	msg_len = arg_msg_len;
	teid = arg_teid;
}

Gtpv1::~Gtpv1() {

}

Gtpv2::Gtpv2() {
	flags = (2 << 5); /* flags : Binary(01000000) */
	seq_num = 0;
	spare = 0;
}

void Gtpv2::init(uint8_t arg_msg_type, uint16_t arg_msg_len, uint32_t arg_teid) {
	msg_type = arg_msg_type;
	msg_len = arg_msg_len;
	teid = arg_teid;
	if (teid > 0) {
		flags += 8;
	}
}

Gtpv2::~Gtpv2() {

}