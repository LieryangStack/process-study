#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int
main (int argc, char *argv[]) {

  int fds[2];
  /**
   * @param fds[0] 用于管道读端
   * @param fds[1] 用于管道写端
   * @return 调用成功返回 0，否则返回 -1
  */
  int n = pipe (fds);
  assert (n == 0);

  printf ("fds[0]:%d\n", fds[0]);
  printf ("fds[1]:%d\n", fds[1]);

  return 0;
}