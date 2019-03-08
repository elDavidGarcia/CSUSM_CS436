/*******************************************************************
 * NAME: UDP_Server.c
 *
 * DESCRIPTION:
 *
 *       This program creates an iterative connectionless server
 *and then returns the current time of day to the client
 *
 * MODIFICATION HISTORY:
 *       DATE:           AUTHOR:         PR #:/DESCRIPTION
 *       11/19/02        T. Springer     Original
 *
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>


int passive_udp(char *address, int port);

int main(int argc, char *argv[])
{


  int sockfd;
  int clilen;
  struct sockaddr_in cliaddr;

  time_t tnow;
  char *ptime;
  char buf[1];

  if (argc != 3) {
    printf("Usage: udp_server <address> <port> \n");
    exit(-1);
  }

  sockfd = passive_udp(argv[1], atoi(argv[2]));

  while (1) { 
    clilen = sizeof(cliaddr);
    if (recvfrom(sockfd, buf, sizeof(buf), 0,  
		 (struct sockaddr *)&cliaddr,
		 &clilen) < 0) {
      perror("recvfrom error");
      exit(-1);
    }
    time(&tnow);
    ptime = ctime(&tnow);
    sendto(sockfd, (const void*)ptime, strlen(ptime), 0, 
	   (struct sockaddr *)&cliaddr, sizeof(cliaddr));
  }

}

int passive_udp(char *address, int port)
{

  int sockfd, n;
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket error");
    return(-1);
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(address);
  servaddr.sin_port = htons(port);


  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    perror("bind error");
    return(-1);
  }

  return(sockfd);
}
