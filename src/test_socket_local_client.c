
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include "test_common.h"


int main(int argc, char** argv)
{
    int fd;
    struct hostent *info;
    struct servent *serv;
    struct sockaddr_in addr;
    char buff[2048];

    // IPv4 TCP のソケットを作成する
    if((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        fprintf( stdout, "socket error\n" );
        return -1;
    }

#if 0    
    // hostsからホスト情報をを読み出す
    info = gethostbyname("localhost");
    if ( !info ){
        close( fd );
        fprintf( stdout, "gethostbyname error\n" );
        return -1;
    }
    serv = getservbyname( "vboxd", "tcp" );
    if ( !serv ){
        close( fd );
        fprintf( stdout, "getservbyname error\n" );
        return -1;
    }
#endif

#if 0
    // 送信先アドレスとポート番号を設定する
    addr.sin_family = AF_LOCAL;
//    addr.sin_port = serv->s_port;
//    addr.sin_addr = *(struct in_addr *)(info->h_addr_list[0]);
    addr.sin_port = TEST_PORT;
    addr.sin_addr.s_addr = inet_addr(TEST_IP);
    // サーバ接続
    connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
#endif
    // 通信開始のための目印の指定 
  // ソケットアドレス構造体
  struct sockaddr_un sun;
  memset(&sun, 0, sizeof(sun));
  // ソケットアドレス構造体に接続先(サーバー)を設定
  sun.sun_family = AF_LOCAL;
  strcpy(sun.sun_path, SOCKNAME);

    connect(fd, (struct sockaddr *)&sun, sizeof(sun));

    sprintf( buff, "This is send Message" );
    // パケットを送信
    if ( send( fd, buff, strlen(buff), 0 ) < 0 ) {
        close( fd );
        fprintf( stdout, "send error\n" );
        return -1;
    }
    close(fd);
    return 0;
}