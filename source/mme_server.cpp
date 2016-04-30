#include "mme_server.h"

Mme g_mme;
int g_workers_count;

void check_usage(int argc) {
	if (argc < 2) {
		cout << "Usage: ./<mme_server_exec> THREADS_COUNT" << endl;
		g_utils.handle_type1_error(-1,
				"Invalid usage error: mmeserver_checkusage");
	}
}

void init(char *argv[]) {
	g_workers_count = atoi(argv[1]);
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	cout << "MME server started" << endl;
	g_mme.server.run(g_mme_ip_addr, g_mme_port, g_workers_count, handle_ue);
}

int handle_ue(int conn_fd) {
	bool res;
	Packet pkt;
	//cout<<"waiting..\n";
	g_mme.server.rcv(conn_fd, pkt);
	//cout<<"waiting2..\n";

	//cout <<"ransimulator ln 49"<<pkt.s1ap_hdr.msg_type<<"\n";

	if (pkt.len <= 0) {
		cout<<"packet length:"<<pkt.len<<"\n";
		cout << "mmeserver_handleue:" << " Connection closed" << endl;
		return 0;
	}
	pkt.extract_s1ap_hdr();
	if (pkt.s1ap_hdr.mme_s1ap_ue_id == 0) {
		switch (pkt.s1ap_hdr.msg_type) {
		/* Initial Attach request */
		case 1:
			cout << "mmeserver_handleue:" << " case 1: initial attach" << endl;
			g_mme.handle_initial_attach(conn_fd, pkt);
			break;

			/* For error handling */
		default:
			cout << "mmeserver_handleue:" << " default case: new" << endl;
			break;
		}
	} else if (pkt.s1ap_hdr.mme_s1ap_ue_id > 0) {
		cout<<"msgtype :"<<pkt.s1ap_hdr.msg_type<<"\n";
		switch (pkt.s1ap_hdr.msg_type) {
		/* Authentication response */

		case 2:
			cout << "mmeserver_handleue:" << " case 2: authentication response"
			<< endl;
			res = g_mme.handle_autn(conn_fd, pkt);
			if (res) {
				g_mme.handle_security_mode_cmd(conn_fd, pkt);
			}
			break;

			/* Security Mode Complete */
		case 3:
			cout << "mmeserver_handleue:" << " case 3: security mode complete"
			<< endl;
			res = g_mme.handle_security_mode_complete(conn_fd, pkt);
			if (res) {
				g_mme.handle_location_update(pkt);
				g_mme.handle_create_session(conn_fd, pkt);
			}
			break;

			/* Attach Complete */
		case 4:
			cout << "mmeserver_handleue:" << " case 4: attach complete" << endl;
			g_mme.handle_attach_complete(pkt);
			g_mme.handle_modify_bearer(pkt);
			break;

			/* Detach request */
		case 5:
			cout << "mmeserver_handleue:" << " case 5: detach request" << endl;
			g_mme.handle_detach(conn_fd, pkt);
			break;
			/* Initiate Handover */
		case 7:
			cout << "mmeserver_handleue:" << " case 7:" << endl;
			g_mme.handle_handover(pkt);

			break;
		case 8:
			cout << "mmeserver_handleue:" << " case 8:" << endl;
			g_mme.setup_indirect_tunnel(pkt);

			break;
		case 9:
			cout << "mmeserver_handleue:" << " case 9:" << endl;
			g_mme.handle_handover_completion(pkt);

			break;
		case 10:
			cout << "send indirect tunnel teardwn req:" << " case 10:" << endl;
			g_mme.teardown_indirect_tunnel(pkt);

			break;
			/* For error handling */
			/* For error handling */
		default:
			cout << "mmeserver_handleue:" << " default case: attached" << endl;
			break;
		}
	}
	return 1;
}

int main(int argc, char *argv[]) {
	check_usage(argc);
	init(argv);
	run();
	return 0;
}
