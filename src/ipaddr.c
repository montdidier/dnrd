/*
    File: ipaddr.c - helper code for IPv4-mapped IPv6 addresses

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
#include "ipaddr.h"

#include <stdlib.h>
#include <assert.h>
#include <netinet/in.h>

void ipv4_mapped_pack(struct in6_addr *sin6_addr,  struct in_addr *sin_addr) {
  assert(sin6_addr != NULL);
  assert(sin_addr != NULL);

  sin6_addr->s6_addr[0] = 0;   
  sin6_addr->s6_addr[1] = 0;   
  sin6_addr->s6_addr[2] = 0;   
  sin6_addr->s6_addr[3] = 0;   
  sin6_addr->s6_addr[4] = 0;   
  sin6_addr->s6_addr[5] = 0;   
  sin6_addr->s6_addr[10] = 0xFF; 
  sin6_addr->s6_addr[11] = 0xFF; 
  sin6_addr->s6_addr[12] = (sin_addr->s_addr >> 24) & 0xFF; 
  sin6_addr->s6_addr[13] = (sin_addr->s_addr >> 16) & 0xFF; 
  sin6_addr->s6_addr[14] = (sin_addr->s_addr >> 8) & 0xFF;  
  sin6_addr->s6_addr[15] = sin_addr->s_addr & 0xFF;
}

void ipv4_mapped_unpack(struct in_addr *sin_addr, struct in6_addr *sin6_addr) {
  assert(sin_addr != NULL);
  assert(sin6_addr != NULL);

  sin6_addr->s6_addr[0] = 0;   
  sin6_addr->s6_addr[1] = 0;   
  sin6_addr->s6_addr[2] = 0;   
  sin6_addr->s6_addr[3] = 0;   
  sin6_addr->s6_addr[4] = 0;   
  sin6_addr->s6_addr[5] = 0;   
  sin6_addr->s6_addr[10] = 0xFF; 
  sin6_addr->s6_addr[11] = 0xFF; 
  sin6_addr->s6_addr[12] = (sin_addr->s_addr >> 24) & 0xFF; 
  sin6_addr->s6_addr[13] = (sin_addr->s_addr >> 16) & 0xFF; 
  sin6_addr->s6_addr[14] = (sin_addr->s_addr >> 8) & 0xFF;  
  sin6_addr->s6_addr[15] = sin_addr->s_addr & 0xFF;
}
