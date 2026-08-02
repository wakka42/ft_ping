# FT_PING

The purpose of this project is to recreate the ping command from inetutils_2.0 in order to have a better understanding of it.

The ping command uses ICMP so our programm need to communicate via this protocol.
As seen in the man ip(7), if we want to use the ICMP, we have to use a raw socket and specify the protocol.

       tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
       udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
       raw_socket = socket(AF_INET, SOCK_RAW, protocol);

## What is ICMP ?

Internet Control Message Protocol (ICMP) is a network layer protocol widely used by network devices such as routers, gateways and hosts to send error messages and operational information. Since the Internet Protocol (IP) itself does not have an inbuilt error-reporting or correction mechanism, ICMP is a supporting protocol within the IP suite that helps in reporting errors and sending diagnostic messages. It is primarily used for:

- Error reporting: When data packets cannot reach their destination due to issues such as unreachable hosts, timeouts or fragmentation errors.
- Operational queries: For example, echo requests and replies used in tools like ping.

## How ICMP Works

- ICMP is connectionless (unlike TCP) and does not require a handshake.
- Messages are encapsulated within IP datagrams, consisting of an IP header followed by an ICMP header and payload.
- Devices send ICMP packets when encountering errors such as unreachable hosts, expired time-to-live (TTL) or routing issues.

## Ping command

With he -v options, we can see that we need to gather many informations:

        ping -v -c 2  8.8.8.8 1.1.1.1

        PING 8.8.8.8 (8.8.8.8): 56 data bytes, id 0x56b7 = 22199
        64 bytes from 8.8.8.8: icmp_seq=0 ttl=115 time=26,999 ms
        64 bytes from 8.8.8.8: icmp_seq=1 ttl=115 time=18,156 ms
        --- 8.8.8.8 ping statistics ---
        2 packets transmitted, 2 packets received, 0% packet loss
        round-trip min/avg/max/stddev = 18,156/22,578/26,999/4,422 ms
        PING 1.1.1.1 (1.1.1.1): 56 data bytes, id 0x56b7 = 22199
        64 bytes from 1.1.1.1: icmp_seq=0 ttl=56 time=17,328 ms
        64 bytes from 1.1.1.1: icmp_seq=1 ttl=56 time=32,423 ms
        --- 1.1.1.1 ping statistics ---
        2 packets transmitted, 2 packets received, 0% packet loss
        round-trip min/avg/max/stddev = 17,328/24,876/32,423/7,548 ms

Furthermore, we'll have to handle signals such as crtl+c
