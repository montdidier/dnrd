/*

    File: srvnode.c
    
    Copyright (C) 2004 by Natanael Copa <n@tanael.org>

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



#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <assert.h>

#include "srvnode.h"
#include "lib.h"
#include "common.h"

#define V4MAPV6(sin6_addr, sin_addr) \
    sin6_addr.s6_addr[0] = 0;   \
    sin6_addr.s6_addr[1] = 0;   \
    sin6_addr.s6_addr[2] = 0;   \
    sin6_addr.s6_addr[3] = 0;   \
    sin6_addr.s6_addr[4] = 0;   \
    sin6_addr.s6_addr[5] = 0;   \
    sin6_addr.s6_addr[10] = 0xFF; \
    sin6_addr.s6_addr[11] = 0xFF; \
    sin6_addr.s6_addr[12] = sin_addr.s_addr >> 24 & 0xFF; \
    sin6_addr.s6_addr[13] = sin_addr.s_addr >> 16 & 0xFF; \
    sin6_addr.s6_addr[14] = sin_addr.s_addr >> 8 & 0xFF;  \
    sin6_addr.s6_addr[15] = sin_addr.s_addr & 0xFF;

srvnode_t *alloc_srvnode(void) {
  srvnode_t *p = allocate(sizeof(srvnode_t));
  p->inactive = -1;
  p->send_time = 0;
  p->send_count = 0;
  /* actually we return a new emty list... */
  return p->next=p;
}


/* init the linked server list
 * returns ptr to the head/tail dummy node in an empty list
 */

srvnode_t *init_srvlist(void) {
  srvnode_t *p = alloc_srvnode();
  /*  p->sock=0; */
  p->next = p;
  return p;
}

/* insert srvnode in the list 
 * returns the new node
 */
srvnode_t *ins_srvnode (srvnode_t *list, srvnode_t *p) {
  assert(list!=NULL);
  p->next = list->next;
  list->next = p;
  return p;
} 

/* removes a node from the list.
 * returns the deleted node 
 */
srvnode_t *del_srvnode_after(srvnode_t *list) {
  srvnode_t *p = list->next;
  assert(list!=NULL);
  list->next = p->next;
  return p;
}

/* closes the server socket and frees the mem */
srvnode_t *destroy_srvnode(srvnode_t *p) {
  /* close socket */
  assert(p!=NULL);
  /*  if (p->sock) close(p->sock); */
  free(p);
  return NULL;
}

/* emties a linked server list. returns the head */
srvnode_t *clear_srvlist(srvnode_t *head) {
  srvnode_t *p=head;
  assert(head != NULL);
  while (p->next != head) {
    destroy_srvnode(del_srvnode_after(p));
  }
  return (head);
}

/* destroys the server list, including the head */
srvnode_t *destroy_srvlist(srvnode_t *head) {
  assert(head != NULL);
  clear_srvlist(head);
  free(head);
  return NULL;
}


/* add a server.*/
srvnode_t *add_srv(srvnode_t *head, const char *ipaddr) {
  srvnode_t *p;
  struct addrinfo *res;
  struct addrinfo hints;
  int error;

  /* head should never be NULL. a new list is allocated with newdomnode */
  assert(head != NULL);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_flags = (AI_V4MAPPED | AI_ADDRCONFIG | AI_NUMERICHOST);

  if ((error = getaddrinfo(ipaddr, 0, 0, &res))) {
    log_debug(1, "getaddrinfo error: %s\n", gai_strerror(error));
    return NULL;
  }

  p = alloc_srvnode();

  if (res->ai_family == AF_INET) {
    log_debug(1, "adding ipv4 address %s\n", inet_ntoa(((struct sockaddr_in*)res->ai_addr)->sin_addr));
    V4MAPV6(p->addr.sin6_addr, ((struct sockaddr_in*)res->ai_addr)->sin_addr);
  } else if (res->ai_family == AF_INET6) {
    log_debug(1, "adding ipv6 address %s\n", res->ai_canonname);
    memcpy(&p->addr.sin6_addr, res->ai_addr, sizeof(p->addr.sin6_addr));
  }
  p->inactive = 0;
  ins_srvnode(head, p);
  freeaddrinfo(res);
  return p;
}

/* returns the last srvnode in the list */
srvnode_t *last_srvnode(srvnode_t *head) {
  srvnode_t *p = head;
  /* head should always be != NULL */
  assert(p != NULL);
  while (p->next != head) p = p->next;
  return p;
}

/* check if there are a list or not 
   retruns 1 if its empty or NULL
   returns 0 if there are servers in the list
*/
int no_srvlist(srvnode_t *head) {
  if (!head) return 1;
  return (head->next == head);
}

