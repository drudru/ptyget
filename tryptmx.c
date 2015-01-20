#include <sys/types.h>
#include <sys/stat.h>

void main()
{
  struct stat st;

  if (stat("/dev/ptmx",&st) == -1) exit(1);
  exit(0);
}
