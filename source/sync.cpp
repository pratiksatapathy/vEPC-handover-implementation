#include "sync.h"

void mux_init(pthread_mutex_t &mux) {
	pthread_mutex_init(&mux, NULL);
}

void cndvar_init(pthread_cond_t &cndvar) {
	pthread_cond_init(&cndvar, NULL);
}

void mlock(pthread_mutex_t &mux) {
	int status;

	status = pthread_mutex_lock(&mux);
	handle_type1_error(status, "Lock error: sync_mlock");
}

void munlock(pthread_mutex_t &mux) {
	int status;

	status = pthread_mutex_unlock(&mux);
	handle_type1_error(status, "Unlock error: sync_munlock");
}

void cndwait(pthread_cond_t &cndvar, pthread_mutex_t &mux) {
	int status;

	status = pthread_cond_wait(&cndvar, &mux);
	handle_type1_error(status, "Condition wait error: sync_cndwait");
}

void cndsignal(pthread_cond_t &cndvar) {
	int status;

	status = pthread_cond_signal(&cndvar);
	handle_type1_error(status, "Condition signal error: sync_cndsignal");
}