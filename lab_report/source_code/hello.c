#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
 
#define my_syscall_num 222 //新增系统调用的调用号
#define sys_call_table_address 0xc1672140//系统调用表的地址
 
static int counter = 0;
struct process
{
	int pid;
	int depth;
};
 
struct process a[1000];
 
unsigned int clear_and_return_cr0(void);
void setback_cr0(unsigned int val);
asmlinkage long sys_mycall(char __user *buf);
int orig_cr0;
unsigned long *sys_call_table = 0;
static int (*anything_saved)(void);


unsigned int clear_and_return_cr0(void)//寄存器权限修改函数
{
	unsigned int cr0 = 0;
	unsigned int ret;
	asm("movl %%cr0, %%eax":"=a"(cr0));//输入部分为空，即直接从cr0寄存器中取数；输出部分为cr0变量，a表示将cr0和eax相关联。这句话的作用是cr0寄存器中的值就赋给了变量cr0
	ret = cr0;
	cr0 &= 0xfffeffff;//增加写权限（第17位为0代表内核空间可写）
	asm("movl %%eax, %%cr0"::"a"(cr0));//输出部分为空，即直接将结果输出到cr0寄存器。输入部分为变量cr0，它和eax寄存器相关联。这句话的作用是变量cr0的值赋给了寄存器cr0
	return ret;
}
 


void processtree(struct task_struct * p,int b)//进程树图形结构函数
{
	struct list_head * l;
	a[counter].pid = p -> pid;
	a[counter].depth = b;
	counter ++;
	for(l = p -> children.next; l != &(p->children); l = l->next)
	{
		struct task_struct *t = list_entry(l,struct task_struct,sibling);
		processtree(t,b+1);
	}
}

asmlinkage long sys_mycall(char __user * buf)
{
    int b = 0;
	struct task_struct * p;
	printk("add_newkernel!\n");

	for(p = current; p != &init_task; p = p->parent );//遍历所有进程并调用进程树图函数得到树状图
		processtree(p,b);
		
	if(copy_to_user((struct process *)buf,a,1000*sizeof(struct process)))//将内核空间的数据拷贝到用户空间
		return -EFAULT;//(为什么不返回0？？)
	else
		return sizeof(a);//拷贝失败返回拷贝地址的字节数
}


void setback_cr0(unsigned int val)
{
	asm volatile("movl %%eax, %%cr0"::"a"(val));//读取val的值到eax寄存器，再将eax寄存器的值放入cr0中
}
 
static int __init init_addsyscall(void)
{
	printk("creat_newkernel\n");
	sys_call_table = (unsigned long *)sys_call_table_address;//获取系统调用服务首地址
	printk("%x\n",sys_call_table);
	anything_saved = (int(*)(void)) (sys_call_table[my_syscall_num]);//保存原始系统调用的地址
	orig_cr0 = clear_and_return_cr0();//调用上面的寄存器修改函数来对系统调用表的内存地址增加写权限
	sys_call_table[my_syscall_num]= (unsigned long)&sys_mycall;//更改原始的系统调用服务地址
	setback_cr0(orig_cr0);//设置为原始的只读cr0
	return 0;
}
 

static void __exit exit_addsyscall(void)//移除内核模块函数
{
	//设置cr0中对sys_call_table的更改权限。
	orig_cr0 = clear_and_return_cr0();//设置cr0可更改
	//恢复原有的中断向量表中的函数指针的值。
	sys_call_table[my_syscall_num]= (unsigned long)anything_saved;
	//恢复原有的cr0的值
	setback_cr0(orig_cr0);
	printk("remove new_kernel_module\n");
}
 
module_init(init_addsyscall);
module_exit(exit_addsyscall);
MODULE_LICENSE("GPL");