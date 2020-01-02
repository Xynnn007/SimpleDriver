#include <stdio.h>
#include <fcntl.h>
int main(){
int fd, i;
char data[256];
int retval;

	fd = open("/dev/xynnn-driver",O_RDWR);
	if(fd == -1){
	printf("open dev error.\n");
		exit(-1);
	}
	printf("open dev /dev/xynnn-driver successfully!\n");
	retval = write(fd, "hahaha", 6);
	if(retval == -1){
		printf("write error.\n");
		exit(-1);
	}
	retval = read(fd, data, 256);
	if(retval == -1){
		printf("read error.\n");
		exit(-1);
	}
	data[retval] = '\0';
	printf("read message %s\n",data);
	
	close(fd);
	return 0;
}


