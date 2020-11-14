#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/sem.h>


int main()
{
    int shmid = shmget((key_t)1000, 512, IPC_CREAT | 0666);//创建一个512个字节大小的名为1000的共享内存空间
    if (shmid == -1)
        return -1;
        char* s = (char*)shmat(shmid, NULL, 0);//返回共享内存被映射的地址
   
    if (s== -1)return -1;

        int a[2] = { 1,0 }；
            sem_init(a, 2)；
   
    while (1)
    {
        printf("请输入数据\n");
        char input[512] = { 0 };
        
        sem_p(0);
        fgets(input, 512, stdin);//获取输入数据

        strcpy(s, input);
        sem_v(1);

        if (strncmp("end", input, 3) == 0)//先判断是否是退出指令，若不是则将数据放入共享内存
        {
            break;
        }
    }
    

    shmdt(s);

    exit(0);
}