#ifndef receive_h
#define receive_h


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


#define NODE_PACKET 0
#define LAST_PACKET 1
#define DISCARD_PACKET 2

#define ICMP_TIME_EXCEEDED 11 // somehow it's not present on MacOS?
#define TIMEOUT_MS 1000.0


bool receive_all (int socket, int ttl, pid_t pid, struct in_addr addr[3], float* time, int* packets_received);
int receive (int socket, int ttl, pid_t pid, float wait, struct in_addr* dest);


#endif
