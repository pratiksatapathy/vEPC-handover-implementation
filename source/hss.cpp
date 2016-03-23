#include "hss.h"

Hss::Hss() {
	mux_init(mysql_client_mux);

	/* Lock not necessary since this is called only once per object. Added for uniformity in locking */
	mlock(mysql_client_mux);
	mysql_client.conn();
	munlock(mysql_client_mux);
}

void Hss::handle_autn_info_req(int conn_fd, Packet &pkt) {

}

Hss::~Hss() {

}
