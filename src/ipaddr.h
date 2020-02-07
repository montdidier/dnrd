#ifndef _DNRD_IPADDR_H_
#define _DNRD_IPADDR_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

struct in_addr;
struct in6_addr;

void ipv4_mapped_pack(struct in6_addr *sin6_addr,  struct in_addr *sin_addr);

void ipv4_mapped_unpack(struct in_addr *sin_addr, struct in6_addr *sin6_addr);

#endif  /* _DNRD_IPADDR_H_ */
