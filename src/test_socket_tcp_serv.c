#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define TEST_IP     "127.0.0.1"
#define TEST_PORT   (12345)



int main(int argc, char** argv)
{
    int fd;
    int fd2;
    struct servent *serv;
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    struct sockaddr_in from_addr;
    char buf[2048];

    // 受信バッファを初期化する
    memset(buf, 0, sizeof(buf));
    // ソケットを作成する
    printf("socket\n");
    if (( fd = socket(AF_INET, SOCK_STREAM, 0 )) < 0 ) {
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

    // IPアドレス、ポート番号を設定
    addr.sin_family = AF_INET;
//    addr.sin_port = serv->s_port;
    addr.sin_port = TEST_PORT;
    addr.sin_addr.s_addr = INADDR_ANY;
    // バインドする
    printf("bind\n");
    if ( bind( fd, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
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
