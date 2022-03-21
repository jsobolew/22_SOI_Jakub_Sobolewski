#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
int main( int argc, char ** argv )
{
if( argc != 2)
return 0;
int value;
value = atoi(argv[1]);
printf( "set variable %d\n", value);
message m;
m.m1_i1 = value;
int ret = _syscall( PM_PROC_NR, PM_SETVAR, & m );
printf( "syscall return: %d\n", ret );
return 0;
}