#include <stdio.h>
#include <config.h>
int main() {
printf("%s_%s\r\n", GATEWAY_SOFTWARE_NAME, GATEWAY_SOFTWARE_VERSION);
return 0;
}
