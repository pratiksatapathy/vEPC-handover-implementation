#include "diameter.h"

Diameter::Diameter() {

}

void Diameter::init(uint8_t msg_type_arg, uint16_t msg_len_arg) {
	msg_type = msg_type_arg;
	msg_len = msg_len_arg;
}

Diameter::~Diameter() {

}