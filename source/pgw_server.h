#ifndef PGW_SERVER_H
#define PGW_SERVER_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "udp_client.h"
#include "pgw_data.h"
#include "pgw_dlink.h"
#include "pgw.h"

void* process_downlink_traffic(void*);
void downlink_data_transfer(PGWDlink&);

void* process_traffic(void*);

void attach_process(PGW&);
void create_session(PGW&);

void data_transfer(PGW&);
void uplink_data_transfer(PGW&);

void detach_process(PGW&);
void delete_session(PGW&);

void startup_pgw();

#endif //PGW_SERVER_H
