/*
    File: ipaddr.h - helper code for IPv4-mapped IPv6 addresses

    Copyright (C) 2020 by Chris Kruger <montdidier@users.noreply.github.com>

    This source is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2, or (at your option)
    any later version.

    This source is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
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
