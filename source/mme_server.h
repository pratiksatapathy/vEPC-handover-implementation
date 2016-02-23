#ifndef MME_SERVER_H
#define MME_SERVER_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "mme_data.h"
#include "mme.h"

void* process_reqs(void*);

void attach_process(MME&);
void init_autn(MME&);
void create_session(MME&);
void modify_session(MME&);

void detach_process(MME&);
void delete_session(MME&);

void startup_mme(char**);

#endif //MME_SERVER_H