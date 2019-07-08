#include "send.h"


int send_packet (int socket, struct sockaddr_in dest, int ttl, pid_t pid) {
    struct icmp packet;

    packet.icmp_type = ICMP_ECHO;
    packet.icmp_code = 0;
    packet.icmp_cksum = 0;
    packet.icmp_id = pid;
    packet.icmp_seq = ttl;

    packet.icmp_cksum = compute_icmp_checksum((uint16_t *)&packet, sizeof(packet));

    setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    if (sendto(socket, &packet, sizeof(packet), 0, (const struct sockaddr *)&dest, sizeof(dest)) < 0) {
        fprintf(stderr, "Error while sending a packet: %s\n", strerror(errno));
        return -1;
    }

    return 0;
}


uint16_t compute_icmp_checksum (const void* buff, int length) {
    u_int32_t sum;
    const u_int16_t* ptr = buff;
    assert (length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t)(~(sum + (sum >> 16)));
}
