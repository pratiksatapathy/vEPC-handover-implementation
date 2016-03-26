#include "security.h"

Crypt::Crypt() {
	key = (uint8_t *)"01234567890123456789012345678901";
	iv = (uint8_t *)"01234567890123456";
}

void Crypt::load() {	
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);
}

int Crypt::enc(uint8_t *plain_text, int plain_text_len, uint8_t *cipher_text, uint64_t k_nas_enc) {
	EVP_CIPHER_CTX *ctx;
	int len;
	int cipher_text_len;

	if (!(ctx = EVP_CIPHER_CTX_new())) {
		handle_crypt_error();
	}
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
		handle_crypt_error();
	}
	if (1 != EVP_EncryptUpdate(ctx, cipher_text, &len, plain_text, plain_text_len)) {
		handle_crypt_error();
	}
	cipher_text_len = len;
	if (1 != EVP_EncryptFinal_ex(ctx, cipher_text + len, &len)) {
		handle_crypt_error(); 
	}
	cipher_text_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return cipher_text_len;
}

int Crypt::dec(uint8_t *cipher_text, int cipher_text_len, uint8_t *plain_text, uint64_t k_nas_enc) {
	EVP_CIPHER_CTX *ctx;
	int len;
	int plain_text_len;

	if (!(ctx = EVP_CIPHER_CTX_new())) {
		handle_crypt_error();
	}
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
		handle_crypt_error();
	}
	if (1 != EVP_DecryptUpdate(ctx, plain_text, &len, cipher_text, cipher_text_len)) {
		handle_crypt_error();
	}
	plain_text_len = len;
	if (1 != EVP_DecryptFinal_ex(ctx, plain_text + len, &len)) { 
		handle_crypt_error(); 
	}
	plain_text_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return plain_text_len;
}

void Crypt::handle_crypt_error() {
	ERR_print_errors_fp(stderr);	
	handle_type1_error(-1, "Crypt error: security_handlecrypterror");
}

Crypt::~Crypt() {
	EVP_cleanup();
	ERR_free_strings();
}