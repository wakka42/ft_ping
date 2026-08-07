# FT_PING

The purpose of this project is to recreate the ping command from inetutils_2.0 in order to have a better understanding of it.

The ping command uses ICMP so the programm need to communicate via this protocol.
As seen in the man ip(7), if I want to use the ICMP, I have to use a raw socket and specify the protocol.

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

        ping -v -c 2  8.8.8.8

        PING 8.8.8.8 (8.8.8.8): 56 data bytes, id 0x56b7 = 22199
        64 bytes from 8.8.8.8: icmp_seq=0 ttl=115 time=26,999 ms
        64 bytes from 8.8.8.8: icmp_seq=1 ttl=115 time=18,156 ms
        --- 8.8.8.8 ping statistics ---
        2 packets transmitted, 2 packets received, 0% packet loss
        round-trip min/avg/max/stddev = 18,156/22,578/26,999/4,422 ms

Furthermore, I'll have to handle signals such as crtl+c

## Parsing

The ping command can handle options and host operand, so I need to check the options passed to the programm and if an host operand is given.

The next step is to check if the host operand is known, I'll use the libc function getaddrinfo to get the IPs or check if it is already an IP address or I can use inet_aton to check if this is a valid ip address even it is 1 (0.0.0.1) or 127.1 (127.0.0.1). In modern implementation, it is better to use inet_pton. I can then convert that 1 or 127.1 in dotted decimal IP with inet_ntoa for exemple. I used getaddrinfo function to see if the hostname can be resolved or if the ip address provided was valid.

## Create an ICMP echo message

Now that the parsing is ok and that I have a socket ready, I'll have to create an ICMP mesage to send
In the internet, I found a lot of documentation about the ICMP packet:

http://www.tcpipguide.com/free/t_ICMPv4EchoRequestandEchoReplyMessages-2.htm

In this packet there is many fields. I created a struct like this one:

        typedef struct s_msg
        {
                uint8_t  type;          -> 8
                uint8_t  code;          -> 0
                uint16_t checksum;      -> initialized at 0
                uint16_t identifier;    -> PID
                uint16_t sequence;      -> Begins to 1
                char     data[56];
        } t_msg;

In order to ensure that the packet is not compromise, there is a checksum field unitialize to 0, then I had to use the checksum algorithm referenced in the RFC 1071

Documentation:

- https://en.wikipedia.org/wiki/Internet_checksum#:~:text=To%20calculate%20the%20checksum%2C%20we,values%20are%20in%20hexadecimal%20notation.&text=This%20checksum%20value%20is%20shown,original%20IP%20packet%20header%20above.
- https://stackoverflow.com/questions/20247551/icmp-echo-checksum
