#ifndef SINK_H
#define SINK_H

#include "diameter.h"
#include "gtp.h"
#include "network.h"
#include "packet.h"
#include "s1ap.h"
#include "tun.h"
#include "udp_client.h"
#include "udp_server.h"
#include "utils.h"

class TrafficMonitor {
public:
	UdpServer server;
	Tun tun;

	TrafficMonitor();
	void handle_uplink_udata();
	void handle_downlink_udata();
	~TrafficMonitor();
};

#endif /* SINK_H */
