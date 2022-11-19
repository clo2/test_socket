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
    int fd2;
    struct servent *serv;
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in from_addr;
    char buf[2048];


 remove(SOCKNAME);  // socket作る前に前回のファイルを消しておく。終了処理でやってもいいけど。

    // 受信バッファを初期化する
    memset(buf, 0, sizeof(buf));
    // ソケットを作成する
    printf("socket\n");
    if (( fd = socket(AF_LOCAL, SOCK_STREAM, 0 )) < 0 ) {
        fprintf( stdout, "socket error : fd = %d\n", fd );
        return -1;
    }

#if 0
    serv = getservbyname( "vboxd", "tcp" );
    if ( !serv ){
        close( fd );
        return -1;
    }
#endif

#if 0
    // IPアドレス、ポート番号を設定
    addr.sin_family = AF_LOCAL;
//    addr.sin_port = serv->s_port;
    addr.sin_port = TEST_PORT;
    addr.sin_addr.s_addr = INADDR_ANY;
#endif

  // ソケットアドレス構造体←今回はここがUNIXドメイン用のやつ
  struct sockaddr_un sun, sun_client;
  memset(&sun, 0, sizeof(sun));
  memset(&sun_client, 0, sizeof(sun_client));
  // ソケットアドレス構造体を設定
  sun.sun_family = AF_LOCAL;               // UNIXドメイン
  strcpy(sun.sun_path, SOCKNAME);  // UNIXドメインソケットのパスを指定

    // バインドする
    printf("bind\n");
    if ( bind( fd, (struct sockaddr *)&sun, sizeof(sun)) < 0 ) {
        fprintf( stdout, "bind error\n" );
        return -1;
    }
    // パケット受信待ち状態とする。待ちうけるコネクト要求は１
    printf("listen \n");
    if ( listen( fd, 1 ) < 0 ) {
        fprintf( stdout, "listen error\n" );
        return -1;
    }
    // クライアントからの接続を確立する
    printf("accept \n");
    if (( fd2 = accept(fd, (struct sockaddr *)&from_addr, &len )) < 0 ) {
        fprintf( stdout, "accept error : fd2 = %d\n", fd2 );
        return -1;
    }
    // パケット受信
    printf("recv \n");
    if ( recv( fd2, buf, sizeof(buf), 0 ) < 0 ) {
        fprintf( stdout, "recv");
        return -1;
    }
    // パケット送受信用ソケットクローズ
    close(fd2);
    // 接続要求待ち受け用ソケットクローズ
    close(fd);
    // 受信データの出力
    fprintf( stdout, "%s\n", buf );
    return 0;
}
