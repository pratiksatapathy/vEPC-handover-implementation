#ifndef MME_SERVER_H
#define MME_SERVER_H

#include "utils.h"
#include "gtp.h"
#include "s1ap.h"
#include "diameter.h"
#include "packet.h"
#include "network.h"
#include "sctp_server.h"
#include "sctp_client.h"
#include "udp_client.h"
#include "mme.h"

extern SctpServer g_mme_server;

#endif /* MME_SERVER_H */
