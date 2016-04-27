#include "network.h"
#include <stdlib.h>
#include <string.h>

int Net_Init()
{
  /* Only really need to be done on windows so far */
  /* but is still run on all systems just in case */
#ifdef _WIN32
  int winerr;
  WSADATA wsadata;

  winerr = WSAStartup(MAKEWORD(2, 2), &wsadata);

  if(winerr)
  {
    PrintError("Unable to initialize WinSock2.\n");
    return -1;
  }
#endif /* _WIN32 */
  return 0;
}

Net_Sock Net_NewSock(int type)
{
  int out;

  if(type == NET_UDP)
    out = socket(AF_INET6, SOCK_DGRAM, 0);

  if(type == NET_TCP)
    out = socket(AF_INET6, SOCK_STREAM, 0);

  if(out == -1)
    PrintError("Problem opening a socket.\n");

  return out;
}

int Net_StartServer(Net_Sock sock, Net_Port port)
{
  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(struct sockaddr_in6));

  addr.sin6_family = AF_INET6;
  addr.sin6_addr = in6addr_any;
  addr.sin6_port = htons(port);

  if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
  {
    PrintError("Unable to bind server socket.\n");
    return -1;
  }

  if(listen(sock, NET_QUEUE))
  {
    PrintError("Unable to listen on server socket.\n");
    return -1;
  }

  return 0;
}

Net_Sock Net_Accept(Net_Sock sock)
{
  if(accept(sock, NULL, NULL) == -1)
  {
    PrintError("Problem accepting connections.\n");
    return -1;
  }

  return 0;
}

int Net_Connect(Net_Sock sock, Net_Addr addr, Net_Port port)
{
  struct sockaddr_in6 sin6;
  memset(&sin6, 0, sizeof(struct sockaddr_in6));

  sin6.sin6_family = AF_INET6;
  sin6.sin6_addr = addr;
  sin6.sin6_port = htons(port);

  if(connect(sock, (struct sockaddr*)&sin6, sizeof(sin6)) == -1)
  {
    PrintError("Problem connecting to another host.\n");
    return -1;
  }

  return 0;
}

int Net_Poll(Net_Sock sock)
{
  int ret;
  fd_set fds;
  struct timeval tv;

  FD_ZERO(&fds);
  FD_SET(sock, &fds);
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  ret = select(sock + 1, &fds, NULL, NULL, &tv);

  if(ret == -1)
  {
    PrintError("Unable to poll socket.\n");
    return -1;
  }
  else if(ret)
    return 1;
  else
    return 0;
}

int Net_Send(Net_Sock sock, const void *data, int len)
{
  int out = send(sock, data, len, 0);

  if(out == -1)
    PrintError("Problem sending data across a socket.\n");

  return out;
}

int Net_SendTo(Net_Sock sock, const void *data, int len, Net_Addr dest, Net_Port port)
{
  int out;
  struct sockaddr_in6 addr;
  memset(&addr, 0, sizeof(struct sockaddr_in6));

  addr.sin6_family = AF_INET6;
  addr.sin6_addr = dest;
  addr.sin6_port = htons(port);

  out = sendto(sock, data, len, 0, (struct sockaddr*)&addr, sizeof(addr));

  if(out == -1)
    PrintError("Problem sending data to an address.\n");

  return out;
}

int Net_Recv(Net_Sock sock, void *data, int len)
{
  int out = recv(sock, data, len, 0);

  if(out == -1)
    PrintError("Problem receiving data from a socket.\n");

  return out;
}

int Net_Close(Net_Sock sock)
{
#ifdef _WIN32
  return closesocket(sock);
#else
  return close(sock);
#endif /* _WIN32 */
}
