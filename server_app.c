
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>


main()
{

  int sockfd
  int new_sockfd;
  int writer_len;

  struct sockaddr_in reader_addr;
  struct sockaddr_in writer_addr;

  /* ソケット生成 */
  if((sockfd = socket(PF_INET, SOCK_STREAM,0)) < 0){
	  perror("reader:socket");
	  exit(1);
  }

  /* 通信ポート・アドレスの設定 */
  bzero((char *) &reader_addr, sizeof(reader_addr));
  reader_addr.sin_family = PF_INET;
  reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  rader_addr.sin_port = htons(8000);

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
  if((new_sockfd = accept(sockfd,(struct sockaddr *)&writer_addr, &writer_len)) < 0){
	  perror("reader:accept");
	  exit(1);
  }
  /* ソケット閉鎖 */
  close(sockfd);
}



void simpe_server(int sckfd){
	char buf[1];
	int buf_len;
	while((buf_len = read(new_sockfd, buf, 1)) > 0){
		write(1,buf,buf_len);
	}
	close(new_sockfd);
}

