/*This file was automatically generated.*/
#ifndef d_net_h
#define d_net_h

void init_net();

typedef struct tcp_socket tcp_socket;
char *malloc_recv(tcp_socket *who);
void opl_send(tcp_socket *who,char *data);
void dtor_tcp_socket_close(tcp_socket *sock);
tcp_socket *ctor_tcp_socket_connect(char const *ip_address,int port);
extern const int MAX_MESSAGE_SIZE_BYTES;

#endif
