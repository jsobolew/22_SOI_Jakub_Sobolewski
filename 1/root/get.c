#include <stdio.h>
#include <lib.h>
int main()
{
message m;
int ret = _syscall( PM_PROC_NR, PM_GETVAR, & m );
printf( "syscall return: %d\n", ret );
return 0;
}