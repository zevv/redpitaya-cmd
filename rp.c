#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char **argv)
{
	struct sockaddr_in sa;

	if(argc < 3) {
		printf("usage: %s <HOST> <FREQ>\n", argv[0]);
		exit(1);
	}

	char *host = argv[1];
	double freq = atof(argv[2]);

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1001);
	sa.sin_addr.s_addr = inet_addr(host);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1) {
		fprintf(stderr, "socket(): %s\n", strerror(errno));
		exit(1);
	}

	int r = connect(fd, (struct sockaddr *)&sa, sizeof(sa));
	if(r == -1) {
		fprintf(stderr, "connect(): %s\n", strerror(errno));
		exit(1);
	}

	int32_t cfg = freq * 1e3;
	r = send(fd, &cfg, sizeof(cfg), 0);
	if(r <= 0) {
		fprintf(stderr, "send(): %s\n", strerror(errno));
		exit(1);
	}

	for(;;) {
		int32_t data[1024];
		
		r = recv(fd, data, sizeof(data), 0);
		if(r <= 0) {
			fprintf(stderr, "recv(): %s\n", strerror(errno));
			exit(1);
		}

		int samples = r / sizeof(int32_t);
		int i;

		int32_t *p = data;
		float f;
		for(i=0; i<samples; i++) {
			f = *p++ / 10000;
			fwrite(&f, 1, sizeof(f), stdout);
		}
	}


	return 0;
}

