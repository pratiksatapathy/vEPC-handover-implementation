#ifndef NETWORK_H
#define NETWORK_H

#include "utils.h"

/* (C) socket */
#include <sys/socket.h>

/* (C) open */
#include <fcntl.h>

/* (C) read, write, close */
#include <unistd.h>

/* (C) select */
#include <sys/select.h>

/* (C) inet_aton, inet_ntop */
#include <arpa/inet.h>

/* (C) ioctl */
#include <sys/ioctl.h>

/* (C) IFF_TUN, IFF_NO_PI, TUNSETIFF */
#include <linux/if_tun.h>

/* (C) ifreq, IFNAMSIZ */
#include <net/if.h>

void bind_sock(int, struct sockaddr_in);
void set_sock_addr(const char*, int, struct sockaddr_in&);
void get_sock_addr(int, struct sockaddr_in&);
void set_timeout(int);
void set_sock_reuse(int);

#endif /* NETWORK_H */