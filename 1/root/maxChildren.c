#include <stdio.h>
#include <lib.h>
int main()
{
message m;
int ret = _syscall( PM_PROC_NR, MAXCHILDREN, & m ); //MAXDESCENDANTSEXCEPTPID MAXCHILDREN
printf( "Max children: %d\n", ret );
return 0;
}
