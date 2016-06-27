#ifndef _NETWORK_H
#define _NETWORK_H

/* include the right headers */
/* should work on a lot of systems */
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netdb.h>
#endif /* _WIN32 */

#define NET_UDP 0
#define NET_TCP 1

#define NET_QUEUE 10

/* holds an IPv6 address */
typedef struct in6_addr Net_Addr;

typedef short Net_Port;

/* holds a socket */
typedef int Net_Sock;

/* initialise networking */
extern int Net_Init();

/* make a socket */
extern Net_Sock Net_NewSock(int type);

/* bind and start listening on a socket */
extern int Net_StartServer(Net_Sock sock, Net_Port port, int type);

/* accept incoming connections on a server socket */
extern Net_Sock Net_Accept(Net_Sock sock);

/* connect to another host */
extern int Net_Connect(Net_Sock sock, Net_Addr addr, Net_Port port);

/* bind the socket to an address */
extern int Net_Bind(Net_Sock sock, Net_Addr addr, Net_Port port);

/* see if there's any new data on the socket */
extern int Net_Poll(Net_Sock sock);

/* send data across a socket */
extern int Net_Send(Net_Sock sock, const void *data, int len);
extern int Net_SendTo(Net_Sock sock, const void *data, int len, Net_Addr dest, Net_Port port);

/* receive data from a socket */
extern int Net_Recv(Net_Sock sock, void *data, int len);
extern int Net_RecvFrom(Net_Sock sock, void *data, int len, Net_Addr *src, Net_Port *port);

/* close a connection */
/* on failure do not retry */
extern int Net_Close(Net_Sock sock);

#endif /* _NETWORK_H */
