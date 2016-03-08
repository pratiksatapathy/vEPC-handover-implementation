#include "s1ap.h"

S1ap::S1ap() {

}

void S1ap::init(uint8_t arg_msg_type, uint16_t arg_msg_len, uint32_t arg_enodeb_ue_id, uint32_t arg_mme_ue_id, uint64_t arg_ue_tai) {
	msg_type = arg_msg_type;
	msg_len = arg_msg_len;
	enodeb_ue_id = arg_enodeb_ue_id;
	mme_ue_id = arg_mme_ue_id;
	ue_tai = arg_ue_tai;
}

S1ap::~S1ap() {

}