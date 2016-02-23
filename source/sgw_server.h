#ifndef SGW_SERVER_H
#define SGW_SERVER_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "sgw_data.h"
#include "sgw.h"

void* process_traffic(void*);

void attach_process(SGW&);
void create_session(SGW&);
void modify_session(SGW&);

void data_transfer(SGW&);
void uplink_data_transfer(SGW&);
void downlink_data_transfer(SGW&);

void detach_process(SGW&);
void delete_session(SGW&);

void startup_hss(char**);

#endif //SGW_SERVER_H