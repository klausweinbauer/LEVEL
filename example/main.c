#include <level.h>
#include <stdio.h>

int main(int argc, char** argv) {
  printf("Version %s\n", getVersion());
  CABasicServiceConfig conf = getCABasicServiceConfig();
  return 0;
}