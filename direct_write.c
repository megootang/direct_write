#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
//#define BUF_SIZE 524288
int main(int argc, char * argv[])
{
	int fd;
	int ret;
	int i =0;
	long long cycle    = atoi(argv[1]);
	long long BUF_SIZE  = atoi(argv[2]);

	unsigned char *buf;
	if(BUF_SIZE%512)
		BUF_SIZE = (BUF_SIZE/512 )* 512;
	ret = posix_memalign((void **)&buf, 512, BUF_SIZE);
	if (ret) {
		perror("posix_memalign failed");
		exit(1);
	}
	memset(buf, 'c', BUF_SIZE);

	
/*	fd = open("./direct_io.data", O_WRONLY | O_DIRECT | O_CREAT | O_TRUNC, 0755);*/
	fd = open("./direct_io.data", O_WRONLY | O_DSYNC  | O_DIRECT | O_CREAT | O_LARGEFILE, 0755);
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
	} while (i  < cycle);
        fsync(fd);
	free(buf);
	close(fd);
        return 0;
}
