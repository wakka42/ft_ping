#pragma once

#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
typedef struct s_args
{
    bool    verbose;
    char    *hostname;
    char    ip[INET_ADDRSTRLEN];
    struct  sockaddr addr;
} t_args;

typedef struct s_msg
{
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence;
    char     data[56];
} t_msg;