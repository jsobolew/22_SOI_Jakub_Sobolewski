#include <stdio.h>
#include <lib.h>
int main()
{
message m;
int ret = _syscall( PM_PROC_NR, WHOMAXCHILDREN, & m );
printf( "Max Children Parent: %d\n", ret );
return 0;
}