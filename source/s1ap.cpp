#include "s1ap.h"

S1ap::S1ap() {

}

void S1ap::init(uint8_t msg_type_arg, uint16_t msg_len_arg, uint32_t enodeb_ue_id_arg, uint32_t mme_ue_id_arg) {
	msg_type = msg_type_arg;
	msg_len = msg_len_arg;
	enodeb_ue_id = enodeb_ue_id_arg;
	mme_ue_id = mme_ue_id_arg;
}

S1ap::~S1ap() {

}