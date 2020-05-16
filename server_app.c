
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

void simple_server(int new_sockfd1, int new_sockfd2);

int main()
{

  int i;
  int sockfd;
  int new_sockfd, new_sockfd1, new_sockfd2;
  int writer_len;

  struct sockaddr_in reader_addr;
  struct sockaddr_in writer_addr;
  struct timeval tv;

  /* ソケット生成 */
  if((sockfd = socket(PF_INET, SOCK_STREAM,0)) < 0){
	  perror("reader:socket");
	  exit(1);
  }

  /* 通信ポート・アドレスの設定 */
  bzero((char *) &reader_addr, sizeof(reader_addr));
  reader_addr.sin_family = PF_INET;
  reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  reader_addr.sin_port = htons(8001);

  /* ソケットにアドレスを結びつける */
  if(bind(sockfd, (struct sockaddr *)&reader_addr, sizeof(reader_addr)) <0){
	  perror("reader:bind");
	  exit(1);
  }

  /* コネクト要求をいくつまで待つかを設定 */
  if(listen(sockfd, 5) <0){
	  perror("reader:listen");
	  close(sockfd);
	  exit(1);
  }

  /* コネクト要求を待つ */
  /* 他のソケットを割り当てる */

  if((new_sockfd1 = accept(sockfd,(struct sockaddr *)&writer_addr, &writer_len)) < 0){
	  perror("reader:accept");
	  exit(1);
  }
  if((new_sockfd2 = accept(sockfd,(struct sockaddr *)&writer_addr, &writer_len)) < 0){
	  perror("reader:accept");
	  exit(1);
  }

  tv.tv_sec = 0;
  tv.tv_usec = 1000;
  setsockopt(new_sockfd1, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
  setsockopt(new_sockfd2, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));

  /* 読み取り 書き込み */
  simple_server(new_sockfd1, new_sockfd2);

  /* ソケット閉鎖 */
  close(sockfd);
}


void simple_server(int new_sockfd1, int new_sockfd2){
	char buf[1];
	int buf_len;
	while(1){
		if((buf_len = read(new_sockfd1, buf, 1)) > 0){
			write(new_sockfd1,buf,buf_len);
		}
		if((buf_len = read(new_sockfd2, buf, 1)) > 0){
			write(new_sockfd2,buf,buf_len);
		}
	}
}

