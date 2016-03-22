#ifndef HSS_SERVER_H
#define HSS_SERVER_H

#include "utils.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"
#include "sctp_server.h"
#include "mysql.h"
#include "hss.h"

extern SctpServer g_hss_server;
extern Hss g_hss;
extern int g_workers_count;

void check_usage(int);
void init(char**);
void run();
void handle_mme(int);

#endif //HSS_SERVER_H