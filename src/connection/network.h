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

typedef struct in6_addr Net_Addr;

typedef short Net_Port;
typedef int Net_Sock;

extern int Net_Init();
extern Net_Sock Net_NewSock(int type);

extern int Net_StartServer(Net_Sock sock, Net_Port port, int type);
extern Net_Sock Net_Accept(Net_Sock sock);

extern int Net_Connect(Net_Sock sock, Net_Addr addr, Net_Port port);
extern int Net_Bind(Net_Sock sock, Net_Addr addr, Net_Port port);

extern int Net_Poll(Net_Sock sock);

extern int Net_Send(Net_Sock sock, const void *data, int len);
extern int Net_SendTo(Net_Sock sock, const void *data, int len, Net_Addr dest, Net_Port port);

extern int Net_Recv(Net_Sock sock, void *data, int len);
extern int Net_RecvFrom(Net_Sock sock, void *data, int len, Net_Addr *src, Net_Port *port);

extern int Net_Close(Net_Sock sock);

#endif /* _NETWORK_H */
