#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

/*-------- PATH is indicats x,y,z values-----------*/

int main(){

int mcp_fd;
char mcp_buf[3];


	mcp_fd = open("PATH",O_RDONLY);
	perror("open");
	read(mcp_fd,mcp_buf[0],sizeof(mcp_buf[0]));
	perror("read");
	printf("%s",mcp_buf[0]);
	close(mcp_fd);
	mcp_fd = open("PATH",O_RDONLY);
	perror("open");
	read(mcp_fd,mcp_buf[1],sizeof(mcp_buf[1]));
	perror("read");
	printf("%s",mcp_buf[1]);
	close(mcp_fd);
	mcp_fd = open("PATH",O_RDONLY);
	perror("open");
	read(mcp_fd,mcp_buf[2],sizeof(mcp_buf[2]));
	perror("read");
	printf("%s",mcp_buf[2]);
	close(mcp_fd);



}
