#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MYPORT 10086
#define MAXFD 4096
#define MYFILE "hello.c"
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}

int get_line(int cfd, char *line, int line_size)
{
    int i = 0;
    char c = '\0';
    int ret_value;
    while(i < line_size && c != '\n')
    {
	int ret = read(cfd, &c, 1);//一次读一个字符
	if(ret < 0)//字符读取失败 返回-1
	{
	    ret_value = -1;
	    break;
	}
	else if(ret == 0)
	{
	    ret_value = 0;
	    break;
	}
	else if(ret > 0)//字符读取成功
	{
	    if(c == '\r')//读到了\r
	    {
		line[i] = c;//先把\r放里
		i++;

		int ret1 = read(cfd, &c, 1);//再读一个字符 看是否是\n
		if(ret1 < 0)//读取失败 返回-1
		{
		    ret_value = -1;
		    break;
		}
		else if(ret1 == 0)
		{
		    ret_value = 0;
		    break;
		}
		else if(ret1 > 0 && c == '\n')//接下来的是\n 
		{
		    line[i] = c;
		    line[++i] = '\0';
		    ret_value = 1;
		    break;
		}
		else if(ret1 > 0 && c != '\n')//接下来不是\n
		{
		    line[i] = c;
		    i++;
		    continue;
		}
	    }
	    line[i] = c;
	    i++;
	}
    }
    printf("%s\n",line);
    return ret_value;
}

int init_lfd(int port, int epfd)
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd < 0)
    {
	sys_err("socket error:");
    }
    //服务器结构体初始化
    struct sockaddr_in sev_addr;
    memset(&sev_addr, 0, sizeof(sev_addr));//结构体清零
    sev_addr.sin_family = AF_INET;
    sev_addr.sin_port = htons(port);
    sev_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //设置端口复用
    int flag = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    //绑定
    int ret = bind(lfd, (struct sockaddr *)&sev_addr, sizeof(sev_addr));
    if(ret < 0)
    {
	sys_err("bind error:");
    }
    //设置监听上限
    ret = listen(lfd, 128);
    if(ret < 0)
    {
	sys_err("listen error:");
    }
    //lfd上树
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = lfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &event);
    if(ret < 0)
    {
	sys_err("epoll_ctl lfd error:");
    }
    return lfd;
}

void send_error(int cfd, int status, char *title, char *text)
{
    char buf[4096];

    sprintf(buf, "%s %d %s\r\n", "HTTP/1.1", status, title);
    sprintf(buf + strlen(buf), "Content-Type:%s\r\n", "text/html");
    sprintf(buf + strlen(buf), "Content-Length:%d\r\n", -1);
    sprintf(buf + strlen(buf), "Connection: close\r\n");
    send(cfd, buf, strlen(buf), 0);
    send(cfd, "\r\n", 2, 0);
    memset(buf, 0, sizeof(buf));
    
    sprintf(buf, "<html><head><title>%d %s</title></head>\n", status, title);
    sprintf(buf + strlen(buf), "<body bgcolor=\"#EDFFD8\"><h2 align=\"center\">%d %s</h4>\n", status, title);
    sprintf(buf + strlen(buf), "%s\n", text);
    sprintf(buf + strlen(buf), "<hr>\n</body>\n</html>\n");
    send(cfd, buf, strlen(buf), 0);
    return;
}

void do_accept(int lfd, int epfd)
{
    char cli_IP[64];
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    //监听
    int cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if(cfd < 0)
    {
	sys_err("accept error:");
    }

    //打印客户端信息
    printf("Have a new client conect: IP = %s, Port = %d, cfd = %d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cli_IP, sizeof(cli_IP)), ntohs(cli_addr.sin_port), cfd);

    //设置cfd为非阻塞
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);

    //cfd上树
    struct epoll_event cevent;
    cevent.data.fd = cfd;
    cevent.events = EPOLLIN | EPOLLET; //设置ET模式

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cevent);
    if(ret < 0)
    {
	sys_err("epoll_ctl cfd err:");
    }
}
void del_node(int cfd, int epfd)
{
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
    if(ret < 0)
    {
	sys_err("epoll_ctl del cfd error:");
    }
    close(cfd);
}

void send_respond_head(int cfd, int no, const char *desp,const char *type, long len)
{
    char buf[1024];
    sprintf(buf, "http/1.1 %d %s\r\n", no, desp);
    send(cfd, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: %s\r\n", type);
    sprintf(buf + strlen(buf), "Content-Length: %ld\r\n", len);
    send(cfd, buf, strlen(buf), 0);
    send(cfd, "Connection: keep-alive\r\n", sizeof("Conneection: keep-alive\r\n"), 0);
    send(cfd, "\r\n", 2, 0);

}

void send_file(int cfd, const char * filename)
{
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
	send_error(cfd, 404, "NOT FOUND", "No such file or direntry");
	exit(1);
    }

    char buf[4096] = {0};
    int len = 0, ret = 0;
    while((len = read(fd, buf, sizeof(buf))) > 0)
    {
	ret = send(cfd, buf, strlen(buf), 0);
	if(ret == -1)
	{
	    if(errno == EAGAIN)
	    {
		perror("send error:");
		continue;
	    }
	    else if(errno == EINTR)
	    {
		perror("send error:");
		continue;
	    }
	    else
	    {
		sys_err("send error:");
	    }
	}
    }
    if(len == -1)
    {
	sys_err("read file error:");
    }
}

const char *get_file_type(const char *name)
{
    char *dot;

    dot = strrchr(name, '.');
    if(dot == NULL)
    {
	return "text/plain; charset=utf-8";
    }
    if(strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
    {
	return "text/html; charset=utf-8";
    }
    if(strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
    {
	return "image/jpeg";
    }
    if(strcmp(dot, ".gif") == 0)
    {
	return "image/gif";
    }
    if(strcmp(dot, ".png") == 0)
	return "image/png";
    
    if(strcmp(dot, ".css") == 0)
	return "text/css";
    
    if(strcmp(dot, ".au") == 0)
	return "audio/basic";

    if(strcmp(dot, ".wav") == 0)
	return "audio/wav";
    if(strcmp(dot, ".avi") == 0)
	return "video/x-msvideo";
    if(strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
	return "video/quicktime";
    if(strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
	return "video/mpeg";
    if(strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
	return "model/vrml";
    if(strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
	return "audio/midi";
    if(strcmp(dot, ".mp3") == 0)
	return "audio/mpeg";
    if(strcmp(dot, ".pac") == 0)
	return "application/x-ns-proxy-autoconfig";
    if(strcmp(dot, ".ogg") == 0)
	return "application/ogg";

    return "text/plain; charset=utf-8";
}

void http_request(char *line, int cfd)
{
    char method[12], path[1024], protocol[12];
    sscanf(line, "%[^ ] %[^ ] %s", method, path, protocol);
    printf("method = %s, path = %s, protocol = %s\n", method, path, protocol);
    
    char *file = path + 1;//指向位置 后移一位 去掉 /
    if(*file == '\0')
    {
	//调用函数发送消息报头
	send_respond_head(cfd, 200, "OK", "text/html; charset=utf-8", -1);
	//发送文件内容
	send_file(cfd, MYFILE);
    }
    else
    {
	//获取文件属性
	struct stat st;
	int ret = stat(file, &st);
	if(ret == -1)
	{
	    send_error(cfd, 404, "NOT FOUND", "No such file or direntry");
	    return;
	}
	else if(S_ISREG(st.st_mode))
	{
	    send_respond_head(cfd, 200, "OK", get_file_type(file), st.st_size);
	    send_file(cfd, file);
	}
	else
	{
	    send_error(cfd, 404, "NOT FOUND", "No such file or direntry");
	    return;
	}
    }
}

void do_read(int cfd, int epfd)
{
    char line[1024] = {0};//缓冲区存 从socket中读出来的第一行
    //读取 第一个行  成功返回1  失败返回-1  对端关闭返回0
    int len = get_line(cfd, line, sizeof(line));
    if(len == -1)//read error
    {
	sys_err("read the one line error:");
    }
    else if(len == 0)//对端关闭
    {
	printf("---查看到客户端关闭---\n");
	//调用函数 关闭cfd
	del_node(cfd, epfd);
	return;
    }
    else
    {
	printf("-----HTTP-HEAD-----\n");
	printf("GET LINE:%s", line);
	//读取剩下的数据
	while(1)
	{
	    char buf[1024] = {0};
	    //一行  一行 读
	    len = get_line(cfd, buf, sizeof(buf));
	    if(buf[0] == '\n')//读到空行
	    {
		break;
	    }
	    else if(len == -1)//read error
	    {
		//sys_err("read the other line error:");
		break;
	    }
	}
	printf("-----结束-----\n");
    }
    //判断前三个字符是不是 get
    if(strncasecmp("get", line, 3) == 0)
    {
	//调用函数 处理 请求  解析
	http_request(line, cfd);
	//close socket 
	del_node(cfd, epfd);
    }
    else
    {
	printf("%s\n",line);
	char method[12], path[1024], protocol[12];
	sscanf(line, "%[^ ] %[^ ] %[^ ]", method, path, protocol);
	printf("method = %s, path = %s, protocol = %s\n", method, path, protocol);
	printf("其他请求  暂时不做处理\n");
    }
}

void epoll_run(int port)
{
    int i;

    struct epoll_event events[MAXFD];//结构体数组 用于epoll_wait

    int epfd = epoll_create(MAXFD);//创建epoll跟节点
    if(epfd < 0)
    {
	sys_err("epoll_create error:");
    }

    int lfd = init_lfd(port, epfd);//创建lfd 进行初始化 并将lfd挂上epoll树
    
    while(1)
    {
	int ret = epoll_wait(epfd, events, MAXFD, -1);
	if(ret < 0)
	{
	    sys_err("epoll_wait error:");
	}
	//遍历满足事件的节点 看是lfd 还是cfd
	for(i = 0; i < ret; i++)
	{
	    //将节点取出 判断 只处理读事件 
	    struct epoll_event *pevent = &events[i];
	    if(!(pevent->events & EPOLLIN))
	    {
		//如果不是读事件 continue跳过
		continue;
	    }
	    if(pevent->data.fd == lfd)
	    {
		//如果是浏览器连接请求 调用函数do_accept
		//参数 lfd epfd
		do_accept(lfd, epfd);
	    }
	    else
	    {
		//不是连接请求 进行读数据
		do_read(pevent->data.fd, epfd);
	    }
	}
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
	printf("主函数未获取到文件路径。\n");
	exit(1);
    }

    int ret = chdir(argv[1]);//改变进程的工作目录，浏览器访问默认为根目录 需要传参 将固定目录传进来
    if(ret < 0)
    {
	sys_err("chdir error:");
    }

    epoll_run(MYPORT);//参数：端口号
    return 0;
}
