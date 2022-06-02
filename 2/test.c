#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main( int argc, char ** argv)
{	
	message m_c, m_b, m_1, m_2, m_3;
		
	m_1.m1_i1 = 0;
	m_1.m1_i2 = 2;
	_syscall(MM, SYS_SET_QUANT, &m_1);
	
	m_2.m1_i1 = 1;
	m_2.m1_i2 = 4;
	_syscall(MM, SYS_SET_QUANT, &m_2);
	
	m_3.m1_i1 = 2;
	m_3.m1_i2 = 6;
	_syscall(MM, SYS_SET_QUANT, &m_3);
	
	printf("PID: %d\n", getpid());
	
	
	if(fork() == 0)
	{	
		printf("PID with group A: %d\n", getpid());
		
		while(1){}
		return 0;
	}
	
	if(fork() == 0)
	{	
		printf("PID with group A: %d\n", getpid());
		
		while(1){}
		return 0;
	}
	
	
	if(fork() == 0 )
	{
		m_b.m1_i1 = getpid();
		m_b.m1_i2 = 1;
		_syscall(MM, SYS_SET_GROUP, &m_b);
		
		printf("PID with group B: %d\n", getpid());
		while(1){}
		return 0;
	}
	
	if(fork() == 0)
	{	
		m_c.m1_i1 = getpid();
		m_c.m1_i2 = 2;
		_syscall(MM, SYS_SET_GROUP, &m_c);
		
		printf("PID with group C: %d\n", getpid());
		while(1){}
		return 0;
	}
	
	while(1){}
	return 0;
}

