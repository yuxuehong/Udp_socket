//客户端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
 
const int MAX_LEN = 4096;                //缓存区长度
const char serverip[] = "172.29.59.29"; //设置服务器地址 此处为本机地址
const int servercom = 7777;              //设置服务器监听端口
const int localcom = 9999;               //设置本机端口
 
int main()
{
    int fd;                                    //通信描述符
    int count;                                 //接收字节数
    char recvbuff[MAX_LEN], sendbuff[MAX_LEN]; //定义接收缓存区和发送缓存区
    struct sockaddr_in localaddr;
    struct sockaddr_in serveraddr;
 
    socklen_t locallen = sizeof(localaddr);
    socklen_t serverlen = sizeof(serveraddr);
 
    //建立UDP的通信文件描述符
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
         printf("create socket failed.\n");
	 return -1;
    }
 
    //填写服务器地址信息
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(servercom); //此处转换为网络字节序
    //填入服务器的IP并转换为网络字节序
    if (inet_pton(AF_INET, serverip, &serveraddr.sin_addr) <= 0)
    {
        return -1;
    }
 
    //设定本客户端的地址信息
   memset(&localaddr, 0, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(localcom);          //此处转换为网络字节序
    localaddr.sin_addr.s_addr = inet_addr("172.29.59.29"); //设置本地IP
 
    //绑定通信描述符和本地地址端口
	
    if ((bind(fd, (struct sockaddr *)&localaddr, locallen)) < 0)
    {
	printf("bind socket failed.\n");
        return -1;
    }
 
 
    while (1)
    {
	printf("client: ");
        memset(sendbuff, 0, sizeof(sendbuff)); //清零
        fgets(sendbuff, sizeof(sendbuff), stdin);        //等待写入
        //向服务器发送数据 从localaddr发到serveraddr
        if (sendto(fd, sendbuff, sizeof(sendbuff), 0, (struct sockaddr *)&serveraddr, serverlen) < 0)
        {
	    printf("sendto faild.\n");
            return -1;
        }
        //从服务器接受消息
        count = recvfrom(fd, recvbuff, sizeof(recvbuff), 0, (struct sockaddr *)&serveraddr, &serverlen);
        if (count == -1)
        {
	    printf("recv faild.\n");
            return -1;
        }
	printf("server(port:%d): %s", ntohs(serveraddr.sin_port), recvbuff);
    }
    close(fd);
    return 0;
}
