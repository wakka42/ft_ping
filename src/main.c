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

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_RAW;

  if ((status = getaddrinfo(args->hostname, NULL, &hints, &res)) != 0) {
    // return -1;
    fprintf(stderr, "ft_ping: unknown host\n");
    exit(1);
  }

  struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
  args->addr = *res->ai_addr;
  addr = &(ipv4->sin_addr);
  inet_ntop(res->ai_family, addr, args->ip, sizeof(args->ip));
  printf("IP: %s\n", args->ip);

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
        exit(64);
    }
    args->hostname = av[host_operand_in_args(ac, av)];
    get_ip(args);
    while((r_getopt = getopt(ac, av, "v?")) != -1){
        switch (r_getopt){
        case 'v':
            args->verbose = true;
            break;
        case '?':
            if (optopt != 0){
                fprintf(stderr, "Try 'ping -? for more information\n");
                exit(64);
            }
            print_usage();
            exit(0);
        default:
            fprintf(stderr, "ping: missing host operand\n");
            fprintf(stderr, "Try 'ping -? for more information\n");
            exit(64);
        }
    }
    return 1;
}

static void handleSignal( int signal ) {
    if (signal == 2){
        printf("\nCtrl+c has been pressed, go terminate and print wht happens till now\n");
        exit(1);
    }
}

uint16_t checksum(int count, void* addr){
    
    long sum = 0;
    
    while(count > 1){
        sum += *(unsigned short *)addr;
        addr += 2;
        count -= 2;
    }    
    if(count > 0){
        sum += * (unsigned char *) addr;
    }
    while(sum >> 16){
        sum = (sum & 0xffff) + (sum >> 16);
    }
    long checksum = ~sum;
    return checksum;
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    t_args args;
    t_msg msg;
    // int r_parse;

    check_args(ac, av, &args);
    signal(SIGINT, handleSignal);
    int psocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (psocket == -1){
        printf("An error occured when initializing a socket\n");
        exit(1);
    }
    printf("Socket ready\n");

    msg.identifier = htons(getpid());
    msg.sequence = htons(1);
    msg.code = 0;
    msg.type = 8;
    msg.checksum = 0;
    memcpy(msg.data, "Ping 42", 7);
    msg.checksum = checksum(sizeof(msg), &msg);
    if ((sendto(psocket, (void *)&msg, sizeof(msg), 0, &args.addr, 16)) < 1)
        printf("C'est la loose");
    while (1){
        int res = sendto(psocket, (void *)&msg, sizeof(msg), 0, &args.addr, 16);
        printf("%d bytes send\n", res);
        sleep(1);
        unsigned char buf[4096];
        struct sockaddr_in sender;
        socklen_t addr_len = sizeof(sender);
        int result = recvfrom(psocket, buf, sizeof(buf), 0, (struct sockaddr *)&sender, &addr_len);
        struct iphdr *ip_header = (struct iphdr *)buf;
        struct icmphdr *icmp_header = (struct icmphdr *)(buf + ip_header->ihl * 4);
        printf("%d bytes from %s: icmp_seq=%u ttl=%u\n", result, args.ip, ntohs(icmp_header->un.echo.sequence), ip_header->ttl);
    }
    return (0);
}