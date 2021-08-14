#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include<string.h>

int main()
{
	int pid = 0;
	int status = 0;
	int len = 0;
	char buf[100] = { 0 };
	int i = 0;

	while (1) {
		printf("LGT@myshell:~$ ");
		scanf("%[^\n]%*c", buf);
		pid = fork();//生成子进程  
		if (pid < 0)
		{
			printf("创建进程失败\n");
			return 1;
		}

		if (pid == 0)
		{
			execlp(buf, buf, NULL); // 子进程调用execlp来进行进程指令的替换  

			printf("指令错误\n"); // 当输入的是不存在的命令或者内部命令时，输出此语句  
			return 1;
		}

		if (waitpid(pid, &status, 0) < 0) // 父进程等待子进程执行结束  
		{
			printf("等待子进程\n");
			return 1;
		}

	}
		    return 0;
	
}
