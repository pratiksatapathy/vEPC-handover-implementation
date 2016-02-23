#ifndef HSS_SERVER_H
#define HSS_SERVER_H

#include "utils.h"
#include "packet.h"
#include "udp_server.h"
#include "mysql.h"
#include "hss.h"

void* process_reqs(void*);
void startup_hss(char**);

#endif //HSS_SERVER_H