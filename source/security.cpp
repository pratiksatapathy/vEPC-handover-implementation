#include "security.h"

Crypt::Crypt() {
	key = (uint8_t *)"01234567890123456789012345678901";
	iv = (uint8_t *)"01234567890123456";
	load();
}

void Crypt::load() {	
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);
}

void Crypt::enc(Packet &pkt, uint64_t k_nas_enc) {
	uint8_t *tem_data = g_utils.allocate_uint8_mem(BUF_SIZE);
	int new_len;

	new_len = enc_data(pkt.data, pkt.len, tem_data, k_nas_enc);
	pkt.clear_pkt();
	pkt.append_item(tem_data, new_len);
	free(tem_data);
}

int Crypt::enc_data(uint8_t *plain_text, int plain_text_len, uint8_t *cipher_text, uint64_t k_nas_enc) {
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

void Crypt::dec(Packet &pkt, uint64_t k_nas_enc) {
	uint8_t *tem_data = g_utils.allocate_uint8_mem(BUF_SIZE);
	int new_len;

	new_len = dec_data(pkt.data, pkt.len, tem_data, k_nas_enc);
	swap(pkt.data, tem_data);
	pkt.data_ptr = 0;
	pkt.len = new_len;
	free(tem_data);
}

int Crypt::dec_data(uint8_t *cipher_text, int cipher_text_len, uint8_t *plain_text, uint64_t k_nas_enc) {
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
	g_utils.handle_type1_error(-1, "Crypt error: security_handlecrypterror");
}

Crypt::~Crypt() {
	EVP_cleanup();
	ERR_free_strings();
}

Integrity::Integrity() {
	HMAC_CTX_init(&ctx);
	key = (uint8_t*)"012345678";
	hmac_len = 20;
}

void Integrity::add_hmac(Packet &pkt, uint64_t k_nas_int) {
	uint8_t *hmac;

	hmac = g_utils.allocate_uint8_mem(hmac_len);
	get_hmac(pkt.data, pkt.len, hmac, k_nas_int);
	pkt.prepend_item(hmac, hmac_len);
	free(hmac);
}

void Integrity::get_hmac(uint8_t *data, int data_len, uint8_t *result, uint64_t k_nas_int) {
	HMAC_Init_ex(&ctx, key, strlen((const char*)key), EVP_sha1(), NULL);
	HMAC_Update(&ctx, data, data_len);
	HMAC_Final(&ctx, result, (unsigned int*)&hmac_len);
}

bool Integrity::hmac_check(Packet &pkt, uint64_t k_nas_int) {
	uint8_t *hmac_res;
	uint8_t *hmac_xres;
	bool res;

	hmac_res = g_utils.allocate_uint8_mem(hmac_len);
	hmac_xres = g_utils.allocate_uint8_mem(hmac_len);
	rem_hmac(pkt, hmac_xres);
	get_hmac(pkt.data, pkt.len, hmac_res, k_nas_int);
	res = cmp_hmacs(hmac_res, hmac_xres);
	free(hmac_res);
	free(hmac_xres);
	return res;
}

void Integrity::rem_hmac(Packet &pkt, uint8_t *hmac) {
	pkt.extract_item(hmac, hmac_len);
	pkt.truncate();
}

bool Integrity::cmp_hmacs(uint8_t *hmac1, uint8_t *hmac2) {
	int i;

	for (i = 0; i < hmac_len; i++) {
		if ((unsigned int)hmac1[i] != (unsigned int)hmac2[i]) {
			return false;
		}
	}
	return true;
}

void Integrity::print_hmac(uint8_t *hmac) {
	int i;

	for (i = 0; i < hmac_len; i++) {
		printf("%02x", (unsigned int)hmac[i]);
	}
	cout << endl;
}

Integrity::~Integrity() {
	HMAC_CTX_cleanup(&ctx);
}