#include <sys/ioctl.h>
#include <termios.h>

void main()
{
  ioctl(1,TIOCEXCL,(char *) 0);
}
