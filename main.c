#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

#include <arpa/inet.h>

#include "send.h"
#include "receive.h"


#define MAX_HOPS 30


void print (struct in_addr addr[3], int current_hops, float time, int packets_received) {
    char* leading_spaces = " ";
    if (current_hops >= 10) leading_spaces = "";

    printf("%s%d  ", leading_spaces, current_hops);

    if (packets_received == 0)
        printf("*\n");

    else {
        char a1[INET_ADDRSTRLEN], a2[INET_ADDRSTRLEN], a3[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &addr[0], a1, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &addr[1], a2, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &addr[2], a3, INET_ADDRSTRLEN);

        printf("%s", a1);

        if ((packets_received > 1) && strcmp(a1, a2))
            printf(" %s", a2);
        if ((packets_received > 2) && strcmp(a1, a3) && strcmp(a2, a3))
            printf(" %s", a3);

        if (packets_received == 3)
            printf("  %dms\n", (int)round(time/3.0));
        else
            printf("  ???\n");
    }
}


int main (int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "No IP address provided\n");
        return EXIT_FAILURE;
    }

    if (argc > 2) {
        fprintf(stderr, "Redundant arguments\n");
        return EXIT_FAILURE;
    }

    pid_t pid = getpid();
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;

    if (inet_pton(AF_INET, argv[1], &(dest.sin_addr)) == 0) {
        fprintf(stderr, "Error parsing IP address: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sockfd < 0) {
        fprintf(stderr, "Socket error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for (int current_hops = 1; current_hops <= MAX_HOPS; current_hops++) {
        for (int i = 0; i < 3; i++)
            if (send_packet(sockfd, dest, current_hops, pid) < 0) {
                fprintf(stderr, "Error sending packet no. %d, hop no. %d, terminating\n", i, current_hops);
                return EXIT_FAILURE;
            }

        struct in_addr addr[3];
        int packets_received = 0;
        float time = 0.0;
        bool should_end = receive_all(sockfd, current_hops, pid, addr, &time, &packets_received);

        print(addr, current_hops, time, packets_received);

        if (should_end)
            break;
    }

    return EXIT_SUCCESS;
}
