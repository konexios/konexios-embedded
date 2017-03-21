#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>

#define BUFLEN 512
#define DEFAULT_SERVER_PORT 8888
#define DEFAULT_CLIENT_PORT 8889
#define MSG_FREQUENCY 5 //sec

static int loop_exit = 0;

void *server(void *arg) {
    uint16_t port;

    if ( !arg ) return NULL;
    port = *((uint16_t*)arg);


    struct sockaddr_in si_server, si_other;

    int s;
    socklen_t slen = sizeof(si_other);
    ssize_t recv_len;
    char buf[BUFLEN];

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("[server]: socket error\n");
        return NULL;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt");
    }

    bzero(&si_server, sizeof(si_server));

    si_server.sin_family = AF_INET;
    si_server.sin_port = htons(port);
    si_server.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(s , (struct sockaddr*)&si_server, sizeof(si_server) ) == -1) {
        perror("bind");
        return NULL;
    }

    while(!loop_exit) {
        if ((recv_len = recvfrom(s, (void*)buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {
          if (errno != EAGAIN) {
            perror("recvfrom()");
            break;
          } else continue;
        }
        buf[recv_len] = 0;

        printf("Received cmd from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Command: %s\n" , buf);
    }
    close(s);
    return NULL;
}

int generate_rnd_tmp_msg(char *txt) {
  float temp = (rand() % 5)/10.0 + 20.0;
  int rate = rand() % 10 + 70;
  return sprintf( txt, "{\"f|temperature\":\"%2.1f\",\"i|heartRate\":\"%d\"}", temp, rate);
}

int client(char *ip, uint16_t port) {
  struct sockaddr_in si_other;
  int s;
  socklen_t slen = sizeof(si_other);
  char message[BUFLEN];
  int msg_len = 0;

  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("[client]: socket");
    return -1;
  }

  bzero((void *) &si_other, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port);

  if (inet_aton(ip?ip:"127.0.0.1" , &si_other.sin_addr) == 0) {
    perror("inet_aton() failed");
    return -1;
  }

  while(!loop_exit) {
    msg_len = generate_rnd_tmp_msg(message);
    if ( sendto(s, message, msg_len, 0, (struct sockaddr *) &si_other, slen) == -1 ) {
      perror("sendto()");
      return -1;
    }
    sleep(MSG_FREQUENCY);
  }
  close(s);
  return 0;
}

static void sigexit(int signum) {
  if ( signum == SIGINT ) {
    loop_exit = 1;
  }
}

int main(int argc, char **argv) {
    int c;
    char *client_ip = NULL;
    uint16_t server_port = DEFAULT_SERVER_PORT;
    uint16_t client_port = DEFAULT_CLIENT_PORT;

    while ( (c = getopt(argc, argv, "vp:P:D:")) != EOF ) {
        switch(c) {
        case 'v' : printf("verbose\n");
            break;
        case 'p' : server_port = atoi(optarg);
            break;
        case 'D' : client_ip = optarg;
            break;
        case 'P' : client_port = atoi(optarg);
            break;
        default: printf("unknown\n"); exit(1);
        }
    }

    signal(SIGINT, sigexit);

    pthread_t listener;
    pthread_create(&listener, NULL, server, &server_port);

    client(client_ip, client_port);

    pthread_join(listener, NULL);
    return 0;
}
