#include"myfun.h"
#define SER_PORT 8000              //端口号
#define SER_ADDR "192.168.19.144"     //地址
#define BUF_SIZE 4096     //缓存区内存大小
int main()
{
	int lfd ,cfd;
	char  buf[BUF_SIZE] ;
	struct sockaddr_in ser_addr,cli_addr;//定义网络地址
	bzero(&ser_addr,sizeof(ser_addr)); //对服务器清零
	bzero(&cli_addr,sizeof(cli_addr)); //对客户端清零
	
	ser_addr.sin_family =AF_INET ;   //网络类型
	ser_addr.sin_port = htons(SER_PORT) ; //端口号
	ser_addr.sin_addr.s_addr = htonl (INADDR_ANY);//将本地字节序转网络字节序
	
	lfd = Socket(AF_INET ,SOCK_STREAM ,0) ;  //创建套接字
	
	bind(lfd,(struct sockaddr*)&ser_addr,sizeof(ser_addr)); //为套接字绑定一//个地址结构
	
	listen(lfd , 128 ) ;   //设置监听上限
	
	int cli_len =sizeof(cli_len) ;
	cfd = accept(lfd , (struct sockaddr *)&cli_addr, &cli_len) ;
	while(1)
	{
		int rr=read(cfd,buf,sizeof(buf)); //读取 cfd 中的内容到缓存区
		write(STDOUT_FILENO,buf,rr); //将缓存区的内容写在标准输出
		write(cfd,buf,rr);
	}
	return 0;
}

