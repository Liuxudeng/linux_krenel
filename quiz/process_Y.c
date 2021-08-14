#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/sem.h>

int main()
{
    int shmid = shmget((key_t)1000, 512, IPC_CREAT | 0666);//获取共享内存空间
    if(shmid == -1) return -1;

    char* p = (char*)shmat(shmid, NULL, 0);
    if(p == (char*)-1) return -1;

    int a[2] = { 1,0 };
    sem_init(a, 2);//初始化或获取信号量id

    while (1)
    {   
        sem_p(1);
        if (strncmp(p, "end", 3) == 0)
        {
            break;
        }
        char m[512];
        strcpy(m, p);
        int i;
        for (i = 0; i < strlen(m); i++)
        {
            if (m[i] > 96 && m[i] < 123)
            {
                m[i] = m[i] - 32;            //将小写变为大写
                continue;
            }
        }
        printf("接收到的数据为：");
        for (i = 0; i < strlen(m); i++)
            printf("%c",m[i]);
        printf("\n");
       
        sem_v(0);
        
    }

    shmdt(p);
    sem_del();
    exit(0);
}
