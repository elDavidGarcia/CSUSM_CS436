#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define MAXLINE 256


void sig_handler(int signal);
void read_n_reply(int socket_fd);

const int backlog = 4;


int main(int argc, char *argv[])
{

  int    listenfd, connfd;
  pid_t   childpid;
  int     clilen;
  struct  sockaddr_in cliaddr, servaddr;

  if (argc != 3) {
    printf("Usage: tcpserver <address> <port> \n");
    return EXIT_FAILURE;
  }

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) {
    perror("socket error");
    return EXIT_FAILURE;
  }

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family        = AF_INET;
  servaddr.sin_addr.s_addr   = inet_addr(argv[1]);
  servaddr.sin_port          = htons(atoi(argv[2]));

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    perror("bind error");
    return EXIT_FAILURE;

  }
  
  if (listen(listenfd, backlog) == -1) {
    perror("listen error");
    return EXIT_FAILURE;
  }

  
  signal(SIGCHLD, sig_handler);

  while (1) {
    clilen = sizeof(cliaddr);
    if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0 ) {
      if (errno == EINTR)
	continue;
      else {
	perror("aceppt error");
	return EXIT_FAILURE;
      }
    }
    if ((childpid = fork()) == 0) {
      close(listenfd);
      read_n_reply(connfd);
      return EXIT_SUCCESS;
    }
    close(connfd);
  }

}

void read_n_reply(int socket_fd)
{

  int n;
  time_t  tnow;
  char    *ptime;
  char buf[1];

  while (1) {
    if ((n = read(socket_fd, buf, MAXLINE)) == 0)
      return;

    time(&tnow);
    ptime = ctime(&tnow);

    write(socket_fd, ptime, strlen(ptime));
  }

}


void sig_handler(int signal)
{

  pid_t    pid;
  int     status;


  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) 
    printf("child %d terminated \n", pid);

  return;

}
