#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/sem.h>

union semun
{
    int val;
};

void sem_init(int a[], int n);
void sem_p(int index);
void sem_v(int index);
void sem_del();

