#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define DEFAULT_MCAST_ADDR ("224.1.2.3")
#define DEFAULT_MCAST_PORT (6050)
#define MAXLINE 80

int main (int argc, char *argv[])
{

  struct in_addr localInterface;
  struct sockaddr_in pubaddr;
  char   str[80];

  char *ipaddr = DEFAULT_MCAST_ADDR;
  char *lfaddr  = "127.0.0.1";

  int port = DEFAULT_MCAST_PORT;
  int fd;
  int opt;

  while (( opt = getopt(argc, argv, "a:p:i:h")) != -1) {
    switch (opt) {
    case 'a':
      ipaddr = optarg;
      break;
    case 'i':
      lfaddr = optarg;
      break;
    case 'p':
      port = atoi(optarg);
      break;
    case 'h':
      printf("Usage: %s -a <ip address> -p <port> -i <network device> \n", argv[0]);
      exit( 0);
    default:
      printf("unknown option: %c \n", optopt);
      break;
    }
  }

  /* Create a datagram socket on which to send. */
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0)
    {
      perror("Opening datagram socket error");
      exit(1);
    }

  /* Initialize the group sockaddr structure  */
  memset((char *) &pubaddr, 0, sizeof(pubaddr));

  pubaddr.sin_family = AF_INET;
  pubaddr.sin_addr.s_addr = inet_addr(ipaddr);
  pubaddr.sin_port = htons(port);


  localInterface.s_addr = inet_addr(lfaddr);
  if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
    {
      fprintf(stderr, "Error setting local interface %x (%s) \n",
	      errno, strerror(errno));
      exit(1);

    }

  while (fgets(str, MAXLINE, stdin) != NULL) {
    if (sendto (fd, str, MAXLINE, 0, (struct sockaddr*)&pubaddr, sizeof(pubaddr)) < 0) {
      fprintf(stderr, "Error sending datagram message: %x (%s) \n",
	      errno, strerror(errno));
      exit( -1);
    }
  }

  close(fd);

}
