#include "ft_ping.h"

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    int psocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (psocket == -1){
        printf("An error occured when initializing a socket\n");
        return 1;
    }
    printf("Socket ready\n");
    return (0);
}