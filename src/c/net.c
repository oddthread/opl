#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IOS
	#include "SDL.h"
	#include "SDL_net.h"
    #else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_net.h>
    #endif
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../oul/src/h/oul.h"

typedef struct tcp_socket
{
	TCPsocket sock;
} tcp_socket;

tcp_socket *ctor_tcp_socket_accept(tcp_socket *server_socket){
	// accept a connection coming in on server_tcpsock

	tcp_socket *client=(tcp_socket*)malloc(sizeof(tcp_socket));
	
	client->sock=SDLNet_TCP_Accept(server_socket->sock);
	
	if(!client->sock) {
		return NULL;
	}
	else {
		return client;
	}
}
tcp_socket *ctor_tcp_socket_connect(char const *ip_address, int port)
{
	IPaddress myip;
	tcp_socket *sock;

	SDLNet_ResolveHost(&myip,ip_address,port);
	sock=(tcp_socket*)malloc(sizeof(tcp_socket));
	sock->sock=SDLNet_TCP_Open(&myip);
	if(!sock->sock){
		printf("SDLNet_TCP_Open: %s\n", SDL_GetError());
		SDL_ClearError();
		free(sock);
		return NULL;
	}
	return sock;
}

void dtor_tcp_socket_close(tcp_socket *sock)
{
	SDLNet_TCP_Close(sock->sock);
	free(sock);
}

int opl_send(tcp_socket *who, char *data, u32 bytes)
{
	/*
	somehow this function (which used to just be called send) was overwriting pulseaudio "send" symbol when linking which was causing bizarre bug
	should probably change the names of all these function to be prefixed by their package name
	*/
	char const *error;
	if(!who)return 0;
	if(!data)return 0;

	int len=SDLNet_TCP_Send(who->sock,data,bytes);
	error=SDL_GetError();
	if(error)
	{
		SDL_ClearError();
	}
	return len;
}
int opl_recv(tcp_socket *who, char *ptr, u32 bytes)
{
	/*
	from SDL documentation:
	If you read more than is sent from the other end, then it will wait until the full requested length is sent, or until the connection is closed from the other end.
	(so do not need to worry about zeroing the buffer)
	*/
	int received=SDLNet_TCP_Recv(who->sock,ptr,bytes);
	
	return received;
}

void init_net()
{
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init error.\n");
	}
}
