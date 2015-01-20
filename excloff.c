#include <sys/ioctl.h>
#include <termios.h>

void main()
{
  ioctl(1,TIOCNXCL,(char *) 0);
}
