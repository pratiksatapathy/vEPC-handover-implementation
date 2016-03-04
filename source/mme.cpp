#include "mme.h"

MmeIds::MmeIds() {
	mcc = 1;
	mnc = 1;
	plmn_id = get_plmn_id(mcc, mnc);
	mmegi = 1;
	mmec = 1;
	mmei = get_mmei(mmegi, mmec);
	gummei = get_gummei(plmn_id, mmei);
}

MmeIds::~MmeIds() {
	
}

Mme::Mme() {
	pthread_mutex_init(&ue_context_table_mux, NULL);	
	pthread_mutex_init(&ue_identification_table_mux, NULL);
}

Mme::~Mme() {

}
