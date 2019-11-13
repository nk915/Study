
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netdb.h>


#define VERSION "1.1"
#define MAXBUF 1400
#define MAXSCAN 100
#define DFL_TIMEOUT 5

typedef struct config {
	        int timeout;
} config;

typedef struct port_t {
	u_int16_t number;			// Number of udp port
	char *name;					// Name of udp port
	char *outstring;			// String to send (protocol dependent)
	int outstringlen;			// Len above
	char *instring;				// String to wait (protocol dependent, NULL for anything)
	int instringlen;			// Len above
	char match;					// Does port match? Allways initialized to 0
} scan;

// Port scanning probes definitions (protocol dependent)
struct port_t             port[] = {
	7, "echo", 
	"probe", 5, "probe", 5, 0,

	13, "daytime", 
	"\x0a", 1, NULL, 0, 0,

	19, "chargen", 
	"\x0a", 1, NULL, 0, 0,

	// dig @ip localhost A
	53, "dns", 
	"\x68\x6c\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x09\x6c\x6f\x63\x61\x6c\x68\x6f\x73\x74\x00\x00\x01\x00\x01", 27, NULL, 0, 0,

	// echo "get a" | tftp ip
	69, "tftp", 
	"\x00\x01\x61\x00\x6e\x65\x74\x61\x73\x63\x69\x69\x00", 13, NULL, 0, 0,

	// ntpq -p ip
	123, "ntp", 
	"\x16\x01\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00", 13, NULL, 1, 0,

	// nbtstat -A ip
	137, "ns-netbios", 
	"\x98\x38\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x20\x43\x4b\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x00\x00\x21\00\01", 50, NULL, 1, 0,

	// radtest a a localhost 1 a
	1812, "RADIUS(1812)", 
	"\x01\x86\x00\x35\x60\x05\x90\x90\x77\x74\x08\x14\xe8\xfa\xb9\x68\x96\x3d\xd1\xba\x01\x03\x61\x02\x12\xd1\x96\xe0\x60\x49\x22\xb5\x68\xca\xc0\xd3\xfc\xd5\x55\x43\x2f\x04\x06\xff\xff\xff\xff\x05\x06\x00\x00\x00\x01", 53, NULL, 1, 0,

	// radtest a a localhost 1 a
	1645, "RADIUS(1645)", 
	"\x01\x86\x00\x35\x60\x05\x90\x90\x77\x74\x08\x14\xe8\xfa\xb9\x68\x96\x3d\xd1\xba\x01\x03\x61\x02\x12\xd1\x96\xe0\x60\x49\x22\xb5\x68\xca\xc0\xd3\xfc\xd5\x55\x43\x2f\x04\x06\xff\xff\xff\xff\x05\x06\x00\x00\x00\x01", 53, NULL, 1, 0,

	// snmpwalk ip ILMI
	161, "snmp(ILMI)", 
	"\x30\x24\x02\x01\x00\x04\x04\x49\x4c\x4d\x49\xa1\x19\x02\x04\x18\x39\x99\xcd\x02\x01\x00\x02\x01\x00\x30\x0b\x30\x09\x06\x05\x2b\x06\x01\x02\x01\x05\x00", 38, NULL, 0, 0,

	// snmpwalk ip public
	161, "snmp(public)", 
	"\x30\x26\x02\x01\x00\x04\x06\x70\x75\x62\x6c\x69\x63\xa1\x19\x02\x04\x2c\x60\x2d\xb6\x02\x01\x00\x02\x01\x00\x30\x0b\x30\x09\x06\x05\x2b\x06\x01\x02\x01\x05\x00", 40, NULL, 0, 0,

	0, NULL, NULL, 0, NULL, 0, 0
};

void usage(char *program) {
	int i;

	fprintf( stderr,
			"%s v"VERSION" -   by: Fryxar\n"
			"usage: %s [options] <host>\n\n"
			"options:\n"
			" -t <timeout>     Set port scanning timeout\n"
			"\nSupported protocol:\n"
			, program, program);

	for( i=0; port[i].number; i++)
		fprintf( stderr, "%s ", port[i].name );
	fprintf( stderr, "\n\n" );

	exit(-1);
}

struct config conf;

int main(int argc, char *argv[]) {
	char                    buf[MAXBUF], opt, *host;
	int                     fd[MAXSCAN], nread, i, j, maxfd, repeat;
	time_t						seconds_start, seconds_now;
	socklen_t               socklen;
	struct timeval          tv;
	struct sockaddr_in      dst_addr, src_addr;
	fd_set                  fdset;
	struct  hostent         *he;


	// Set defaults
	conf.timeout = DFL_TIMEOUT;

	if(argc < 2) usage( argv[0] );

	while((opt = getopt(argc, argv, "t:")) != -1) {
		switch(opt) {
			case 't':
				if(strlen(optarg) == 0) usage(argv[0]);
				conf.timeout = atoi(optarg);
				break;

			default:
				usage(argv[0]);
				break;
		}
	}

	host = argv[argc-1];

	if( (he = gethostbyname(host)) == NULL) {
		fprintf(stderr, "Error: Cannot resolve %s!\n", host);
		exit(-1);
	}

	for( i = 0; port[i].number; i++ ) {
		if((fd[i] = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket: ");
			exit(2);
		}

		src_addr.sin_family = AF_INET;
		src_addr.sin_addr.s_addr = htonl(INADDR_ANY );
		src_addr.sin_port = 0;

		dst_addr.sin_family = AF_INET;
		dst_addr.sin_addr = *((struct in_addr *)he->h_addr);
		dst_addr.sin_port =  htons(port[i].number);

		if( bind(fd[i], (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0 ) {
			perror("bind: ");
			close(fd[i]);
			exit(4);
		}

		memcpy( buf, port[i].outstring, port[i].outstringlen );
		if(sendto(fd[i], buf, port[i].outstringlen, 0, 
					(struct sockaddr *)&dst_addr, sizeof(dst_addr)) < 0) {
			perror("sendto: ");
			close(fd[i]);
			exit(5);
		}
	}

	time( &seconds_start );

	while( 1 ) {
		time( &seconds_now );
		if( seconds_start+conf.timeout <= seconds_now ) break;
		tv.tv_sec  = conf.timeout - (seconds_now - seconds_start);
		tv.tv_usec = 0;

		FD_ZERO( &fdset );

		for( maxfd = 0, i = 0; port[i].number; i++ )
			if( port[i].match == 0 ) { FD_SET( fd[i], &fdset ); maxfd = fd[i]; }

		if( select( maxfd+1, &fdset, NULL, NULL, &tv ) < 0 ) {
			perror("select: ");
			exit(6);
		}

		for( i = 0; port[i].number; i++ ) {
			if( !FD_ISSET( fd[i], &fdset ) ) continue;

			if( (nread = recvfrom(fd[i], buf, MAXBUF, 0, 
							(struct sockaddr *)&dst_addr, &socklen)) <= 0 ) {
				port[i].match = 2;
				close( fd[i] );
				continue;
			}

			if( port[i].instring == NULL || 
					!memcmp( buf, port[i].instring, port[i].instringlen ) ) {
				for( repeat = 0, j = 0; j < i; j++ ) 
					if( port[i].number == port[j].number && port[j].match > 0 ) repeat = 1;

				if( !repeat ) printf( "%s\t%d/udp\n", host, port[i].number );
				port[i].match = 1;
				close( fd[i] );
			}
		}
	}
	for( i = 0; port[i].number; i++ )
		if( port[i].match == 0 ) close( fd[i] );

	exit(0);
}
