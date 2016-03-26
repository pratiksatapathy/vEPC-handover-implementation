#ifndef SECURITY_H
#define SECURITY_H

/* (C) OPENSSL_config */
#include <openssl/conf.h>

/* (C) EVP_* */
#include <openssl/evp.h>

/* (C) ERR_* */
#include <openssl/err.h>

/* (C) HMAC_* */
#include <openssl/hmac.h>

#include "diameter.h"
#include "gtp.h"
#include "packet.h"
#include "s1ap.h"
#include "utils.h"

class Crypt {
private:	
	uint8_t *key;
	uint8_t *iv;

public:
	Crypt();
	void load();
	// void enc(Packet&, uint64_t);
	int enc(uint8_t*, int, uint8_t*, uint64_t);
	// void dec(Packet&, uint64_t);
	int dec(uint8_t*, int, uint8_t*, uint64_t);
	void handle_crypt_error();
	~Crypt();
};

class Integrity {
private:	
	HMAC_CTX ctx;
	uint8_t *key;

public:
	int hmac_len;

	Integrity();
	void add_hmac(Packet&, uint64_t);
	void get_hmac(uint8_t*, int, uint8_t*, uint64_t);
	void rem_hmac(Packet&, uint8_t*);
	bool cmp_hmacs(uint8_t*, uint8_t*);
	~Integrity();
};

#endif /* SECURITY_H */