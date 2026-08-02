#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>
#include <bits/getopt_core.h>


typedef struct s_args
{
    bool    verbose;
    char    *target;
} t_args;
