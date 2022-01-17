//服务器端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
 
const int MAX_LEN = 4096;   //缓存区长度
const int servercom = 7777; //服务器端口
 
int main()
{
    int listenfd;
    struct sockaddr_in servaddr; //服务器监听信息
    struct sockaddr_in clieaddr; //获取客户端地址信息
    socklen_t serverlen = sizeof(servaddr);
    socklen_t clienlen = sizeof(clieaddr);
 
    char buff[MAX_LEN];
    int recv_count = 0;
 
    //建立监听文件描述符
    if ((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return -1;
    }
 
    //填入服务器通信地址信息
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("172.29.59.29"); //地址为任意本机地址
    servaddr.sin_port = htons(servercom);
 
    //将地址信息与描述符绑定
    if (bind(listenfd, (struct sockaddr *)&servaddr, serverlen) < 0)
    {
        return -1;
    }
 
    //必要信息输出
while(1)
{

        //等待从clieaddr地址接收数据，监听数据端口为servaddr，同时把客户端信息放在clieaddr中
        recv_count = recvfrom(listenfd, buff, MAX_LEN, 0, (struct sockaddr *)&clieaddr, &clienlen);
        if (recv_count == -1)
        {
            return -1;
        }
	printf("client(port:%d): %s", ntohs(clieaddr.sin_port), buff);
	printf("server: ");
	memset(buff, 0, sizeof(buff));
	fgets(buff,sizeof(buff),stdin);
        sendto(listenfd, buff, sizeof(buff), 0, (struct sockaddr *)&clieaddr, clienlen);
        //显示客户端的地址和服务端的地址
    }
    close(listenfd);
    return 0;
}
