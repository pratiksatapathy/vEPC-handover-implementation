#ifndef NETWORK_H
#define NETWORK_H

/* (C) inet_aton, inet_ntop */
#include <arpa/inet.h>

/* (C) open */
#include <fcntl.h>

/* (C) IFF_TUN, IFF_NO_PI, TUNSETIFF */
#include <linux/if_tun.h>

/* (C) ifreq, IFNAMSIZ */
#include <net/if.h>

/* (C) ioctl */
#include <sys/ioctl.h>

/* (C) select */
#include <sys/select.h>

/* (C) socket */
#include <sys/socket.h>

/* (C) read, write, close */
#include <unistd.h>

#include "utils.h"

extern socklen_t g_sock_addr_len;

void set_inet_sock_addr(const char*, int, struct sockaddr_in&);
void bind_sock(int, struct sockaddr_in);
void get_sock_addr(int, struct sockaddr_in&);
void set_sock_reuse(int);
void set_snd_timeout(int);
void set_rcv_timeout(int);

#endif /* NETWORK_H */