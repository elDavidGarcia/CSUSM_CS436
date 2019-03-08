/*******************************************************************
 * NAME: UDP_Client.c
 *
 * DESCRIPTION:
 *
 *       This program creates a connectionless client
 *       and then request the current time of day from the server.
 *
 * MODIFICATION HISTORY:
 *       DATE:           AUTHOR:         PR #:/DESCRIPTION
 *       11/19/02        T. Springer     Original
 *
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 80

int main(int argc, char *argv[])
{
  struct  sockaddr_in servaddr;
  int sockfd;
  int servlen;
  int ntimes;
  int n;

  char buf[1];
  char ptime[MAXLINE];

  if (argc != 4) {
    printf("Usage: udp_client <address> <port> <ntimes>\n");
    exit(-1);
  }

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket error");
    exit(-1);
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(atoi(argv[2]));

  servlen = sizeof(servaddr);

  ntimes = atoi(argv[3]);

  while (ntimes--) {

    if (sendto(sockfd, (void *)buf, sizeof(buf), 0, 
	       (struct sockaddr *)&servaddr, servlen) < 0) {
      perror("sendto error");
      exit(-1);
    }

    n = recvfrom(sockfd, ptime, MAXLINE, 0, (struct sockaddr *)&servaddr,
		 &servlen);     

    fputs(ptime, stdout);
    memset(ptime, 0, MAXLINE);

    sleep(1);
  }

}
