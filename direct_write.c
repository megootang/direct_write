#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
static int usage() {
    fprintf(stderr, "direct_write <filename> <cycle> <bufsize>\n");
    return EXIT_FAILURE;
}

int main(int argc, char * argv[])
{
	int fd;
	int ret;
	int i =0;
	long long cycle    ;
	long long BUF_SIZE  ;
	char *filename ;
	unsigned char *buf;
	if (argc != 4) {
		fprintf(stderr, "Incorrect argument count\n");
		return usage();
	}

	filename = argv[1];
	cycle    = atoi(argv[2]);
	BUF_SIZE = atoi(argv[3]);

	if(BUF_SIZE%512) {
		if(BUF_SIZE/512)
			BUF_SIZE = (BUF_SIZE/512 + 1 )* 512;
		else
			BUF_SIZE = 512;
	}
	ret = posix_memalign((void **)&buf, 512, BUF_SIZE);
	if (ret) {
		perror("posix_memalign failed");
		exit(1);
	}
	memset(buf, 'c', BUF_SIZE);


/*	fd = open(filename, O_WRONLY | O_DIRECT | O_CREAT | O_TRUNC, 0755);*/
	fd = open(filename, O_WRONLY | O_DSYNC  | O_DIRECT | O_CREAT | O_LARGEFILE, 0755);
	if (fd < 0){
		perror("open ./direct_io.data failed");
		exit(1);
	}

	do {
		ret = write(fd, buf, BUF_SIZE);
		if (ret < 0) {
			perror("write ./direct_io.data failed");
		}
		i++;
	} while (i<cycle);
        fsync(fd);
	free(buf);
	close(fd);
        return 0;
}
