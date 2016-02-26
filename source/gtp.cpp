#include "gtp.cpp"

Gtpv1::Gtpv1() {
	flags = (3 << 4); /* flags : Binary(00110000) */
	seq_num = 0;
	npdu_num = 0;
	next_ext_header_type = 0;
}

void Gtpv1::init(uint8_t msg_type_arg, uint16_t msg_len_arg, uint32_t teid_arg) {
	msg_type = msg_type_arg;
	msg_len = msg_len_arg;
	teid = teid_arg;
}

Gtpv1::~Gtpv1() {

}

Gtpv2::Gtpv2() {
	flags = (3 << 5); /* flags : Binary(01100000) */
	seq_num = 0;
	spare = 0;
}

void Gtpv2::init(uint8_t msg_type_arg, uint16_t msg_len_arg, uint32_t teid_arg) {
	msg_type = msg_type_arg;
	msg_len = msg_len_arg;
	teid = teid_arg;
	if (teid > 0) {
		flags += 8;
	}
}

Gtpv2::~Gtpv2() {

}