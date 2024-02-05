//编译:
//		gcc -g3 ./unix_sock_bind.c -o x

//本demo 展示如何处理unix socket 绑定的path 路径(unix socket bind() 之后, 可以 轻松进行udp/tcp 通信)

#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>



int main(void)
{
  int fd, size;
  struct sockaddr_un un;

  memset(&un, 0, sizeof(un));
  un.sun_family = AF_UNIX;
  strcpy(un.sun_path, "foo.socket");

  if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    return -1;
  }

  //取偏移量(宏), 求struct sockaddr_un -> sun_path 元素的字符串长度
  size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

  if (bind(fd, (struct sockaddr *) &un, size) < 0) {
    perror("bind()");
    return -1;
  }

  printf("UNIX domain socket bound\n");
  return 0;
}
