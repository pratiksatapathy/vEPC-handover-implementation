#ifndef SECURITY_H
#define SECURITY_H

/* (C) OPENSSL_config */
#include <openssl/conf.h>

/* (C) EVP_* */
#include <openssl/evp.h>

/* (C) ERR_* */
#include <openssl/err.h>

#include "utils.h"

class Crypt {
private:
	uint8_t *key;
	uint8_t *iv;

public:
	Crypt();
	void load();
	int enc(uint8_t*, int, uint8_t*, uint64_t);
	int dec(uint8_t*, int, uint8_t*, uint64_t);
	void handle_crypt_error();
	~Crypt();
};



#endif /* SECURITY_H */