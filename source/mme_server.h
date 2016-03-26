#ifndef MME_SERVER_H
#define MME_SERVER_H

#include "diameter.h"
#include "gtp.h"
#include "mme.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "sctp_client.h"
#include "sctp_server.h"
#include "sync.h"
#include "telecom.h"
#include "udp_client.h"
#include "utils.h"

extern Mme g_mme;
extern int g_workers_count;

void check_usage(int);
void init(char**);
void run();
int handle_ue(int);

#endif /* MME_SERVER_H */
