#include "telecom.h"

uint16_t get_plmn_id(uint16_t mcc, uint16_t mnc) {
	return stoull(to_string(mcc) + to_string(mnc));
}

uint32_t get_mmei(uint16_t mmegi, uint8_t mmec) {
	return stoull(to_string(mmegi) + to_string(mmec));
}

uint64_t get_gummei(uint16_t plmn_id, uint32_t mmei) {
	return stoull(to_string(plmn_id) + to_string(mmei));
}

uint64_t get_guti(uint64_t gummei, uint64_t m_tmsi) {
	return stoull(to_string(gummei) + to_string(m_tmsi));
}