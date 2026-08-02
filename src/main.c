#include "ft_ping.h"

int print_usage(){
    printf("Usage: ping [OPTION...] HOST ...\n");
    printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
    printf("\nOptions valid for all request types:\n");
    printf("  -v, --verbose              verbose output\n");
    return 0;
}

int check_args(int ac, char **av, t_args *args){

    int r_getopt = 0;
    args->verbose = false;
    args->target = NULL;

    if (ac == 1){
        printf("ping: missing host operand\n");
        printf("Try 'ping -? for more information\n");
        return 64;
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