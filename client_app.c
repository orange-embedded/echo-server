
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>


void send_input_data(int sockfd);

/* mainの引数：コマンドライン引数 */
/* argc:文字列数    **argv：文字列配列（関数名と引数を格納） */
int main(int argc, char **argv) {

    int sockfd;
    struct sockaddr_in client_addr;

    /* コマンド引数が一個であることを確認 */
    if (argc != 2) {
        fprintf(stderr, "usage: %s machine-name\n", argv[0]);
        exit(1);
    }

    /* ソケットを生成 */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("client: socket");
        exit(1);
    }

    /*
     * client_addr構造体に、接続するサーバのアドレス・ポート番号を設定
     */
    bzero((char *) &client_addr, sizeof(client_addr));
    client_addr.sin_family = PF_INET;
    client_addr.sin_addr.s_addr = inet_addr(argv[1]);
    client_addr.sin_port = htons(8001);

    /* ソケットをサーバに接続 */
    if (connect(sockfd, (struct sockaddr*) &client_addr, sizeof(client_addr))
            < 0) {
        perror("client: connect");
        close(sockfd);
        exit(1);
    }

    send_input_data(sockfd);

    /* ソケットをクローズ */
    close(sockfd);

    return 0;
}

void send_input_data(int sockfd) {

    char buf[1];
    int buf_len;

    while (1) {
    	//ソケットに文字を出力
    	if((buf_len = read(0, buf, 1)) > 0){
            write(sockfd, buf, buf_len);
    	}
        //ソケットから文字を受け取り標準出力
        if((buf_len = read(sockfd, buf, 1)) > 0){
            write(1, buf, buf_len);
        }
    }

}
