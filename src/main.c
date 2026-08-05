#include "ft_ping.h"

int print_usage(){
    printf("Usage: ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
    printf("\nOptions valid for all request types:\n");
    printf("  -v, --verbose              verbose output\n");
    return 0;
}

int  host_operand_in_args(int ac, char **av){
    int i = 1;
    while(i < ac){
        if(av[i][0] != '-')
            return i;
        i++;
    }
    return 0;
}

int get_ip(t_args *args){

  struct addrinfo hints, *res;
  void *addr;
  int status;
  char ipstr[INET_ADDRSTRLEN];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_RAW;

  if ((status = getaddrinfo(args->hostname, NULL, &hints, &res)) != 0) {
    return -1;
  }

  struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
  addr = &(ipv4->sin_addr);

  inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
  printf("IP: %s\n", ipstr);
  args->ip = ipstr;

  freeaddrinfo(res);

  return 0;
}

int check_args(int ac, char **av, t_args *args){

    int r_getopt = 0;
    args->verbose = false;
    args->hostname = NULL;

    if (!(host_operand_in_args(ac, av))){
        printf("ping: missing host operand\n");
        printf("Try 'ping -? for more information\n");
        return 64;
    }
    args->hostname = av[host_operand_in_args(ac, av)];

    if (get_ip(args) != 0) {
        fprintf(stderr, "ft_ping: unknown host\n");
        exit(1);
    }

    while((r_getopt = getopt(ac, av, "v?")) != -1){
        switch (r_getopt){
        case 'v':
            args->verbose = true;
            break;
        case '?':
            if (optopt != 0){
                fprintf(stderr, "Try 'ping -? for more information\n");
                return 64;
            }
            print_usage();
            return 0;
        default:
            fprintf(stderr, "ping: missing host operand\n");
            fprintf(stderr, "Try 'ping -? for more information\n");
            return 64;
        }
    }
    return 1;
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    t_args args;
    int r_parse;

    r_parse = check_args(ac, av, &args);
    if (r_parse != 1)
        return r_parse;

    int psocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (psocket == -1){
        printf("An error occured when initializing a socket\n");
        return 1;
    }
    printf("Socket ready\n");
    return (0);
}