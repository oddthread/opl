/*This file was automatically generated.*/
#ifndef d_net_h
#define d_net_h

#include "oul/src/h/oul.h"

void init_net();

typedef struct tcp_socket tcp_socket;

tcp_socket *ctor_tcp_socket_connect(char const *ip_address,int port);
tcp_socket *ctor_tcp_socket_accept(tcp_socket *server_socket);
void dtor_tcp_socket_close(tcp_socket *sock);

char *opl_recv(tcp_socket *who, char *ptr, u32 bytes);
void opl_send(tcp_socket *who,char *data, u32 bytes);

#endif
