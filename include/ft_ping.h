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
typedef struct s_args
{
    bool    verbose;
    char    *hostname;
    char    *ip;
} t_args;
