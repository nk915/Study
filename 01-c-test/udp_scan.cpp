

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <rpc/rpc.h>
#include <rpc/xdr.h>
#include <rpc/pmap_prot.h>
#include <rpc/pmap_clnt.h>
#include <rpcsvc/yp_prot.h>
#include <rpcsvc/ypclnt.h>
#include <errno.h>
#include <getopt.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define DEFAULT_LOW_PORT 1
#define DEFAULT_HIGH_PORT 2000

#define MAJOR_VERSION 1
#define MINOR_VERSION 2

void tx_packet(int fd, int port)
{
	char buf[1024] = {0,};
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr("172.16.1.200");

	if(sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("*** sendto() failed ***");
	}
}

int rx_packet(int fd)
{
	fd_set fds;
	struct timeval poll;
	char buf[1024] = {0,};

	poll.tv_sec = 1;
	poll.tv_usec = 0;

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		if(select(fd + 1, &fds, NULL, NULL, &poll) > 0)
		{
			recvfrom(fd, &buf, sizeof(buf), 0x0, NULL, NULL);
		}
		else if(!FD_ISSET(fd, &fds))
			return 1;
		else
			perror("*** recvfrom() failed ***");

		struct ip* piphdr = (struct ip *)buf;
		int iplen = piphdr->ip_hl << 2;

		struct icmp* picmp = (struct icmp *)(buf + iplen);

		if((picmp->icmp_type == ICMP_UNREACH) && (picmp->icmp_code == ICMP_UNREACH_PORT))
			return 0;
	}
}


int udp_scan()
{
	int sendfd, recvfd;
	int portlow=1;
	int porthigh=10240;
	struct servent *srvport;

	// open send UDP socket
	if((sendfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("*** socket(,,IPPROTO_UDP) failed ***n");
		exit(-1);
	}
	// open receive ICMP socket
	if((recvfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		perror("*** socket(,,IPPROTO_ICMP) failed ***n");
		exit(-1);
	}

	for(int port = portlow; port <= porthigh; port++)
	{
		tx_packet(sendfd, port);

		if(rx_packet(recvfd) == 1)
		{
			srvport = getservbyport(htons(port), "udp");

			if (srvport != NULL)
				printf("tport %d: %s\n", port, srvport->s_name);
			fflush(stdout); 
		}
	}
}



int main()
{
	printf("START \n");
	udp_scan();
	printf("END \n");
	return 0;
}
