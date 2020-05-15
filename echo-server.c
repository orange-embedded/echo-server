
#include <sys/fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <strings.h>

void simpe_server(int sockfd) ;

int main()

{

  int sockfd;

  int new_sockfd;

  int writer_len;

  struct sockaddr_in reader_addr;

  struct sockaddr_in writer_addr;

  pid_t pid_num = 1;

  /* ソケットの生成 */

  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {

    perror("reader: socket");

    exit(1);

  }



  /* 通信ポート・アドレスの設定 */

  bzero((char *) &reader_addr, sizeof(reader_addr));

  reader_addr.sin_family = PF_INET;

  reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  reader_addr.sin_port = htons(9000);

  /* ソケットにアドレスを結びつける */

  if (bind(sockfd, (struct sockaddr *)&reader_addr, sizeof(reader_addr)) < 0) {

    perror("reader: bind");

    exit(1);

  }



  /* コネクト要求をいくつまで待つかを設定 */

  if (listen(sockfd, 5) < 0) {

    perror("reader: listen");

    close(sockfd);

    exit(1);

  }



  /* コネクト要求を待つ */

  while(0 != pid_num){

    if ((new_sockfd = accept(sockfd,(struct sockaddr *)&writer_addr, (socklen_t *) &writer_len)) < 0) {

      perror("reader: accept");

      exit(1);

    }

    pid_num = fork();

  }

  simpe_server(new_sockfd);

  close(sockfd);  /* ソケットを閉鎖 */

  return 0;

}



void simpe_server(int sockfd) {

  char buf[1];

  int buf_len;

  while((buf_len = read(sockfd, buf, 1)) > 0) {

    write(sockfd, buf, buf_len);

  }

  close(sockfd);  /* ソケットを閉鎖 */

}
