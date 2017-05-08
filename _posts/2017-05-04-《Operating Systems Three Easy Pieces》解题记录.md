---
layout: post
title: 《Operating Systems Three Easy Pieces》解题记录
tags: 《OSTEP》
categories: OS
date: 2017-05-04 22:47:00 +0800
---

**不断更新**    
最近在看**《Operating Systems: Three Easy Pieces》**，一本公认比较适合自学的一本关于操作系统的书籍，课后有一些需要编码实现的习题，由于都是自己编码解答、没有能对照纠正的答案，为方便之后温习纠正，故在此记录。    
**环境:操作系统：Centos7，编译器gcc**


### Process API

#### Question 1

>Write a programthat calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to something
(e.g., 100). What value is the variable in the child process?
What happens to the variablewhen both the child and parent change
the value of x?

代码：      

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("Solving Question 1\n");
	int val = 10;
	int rc = fork();
	if(rc < 0){
		fprintf(stderr, "fork failed\n");
		exit(1);
	}else if(rc == 0){
		printf("Val now is %d\n",val);
		val=9;
		printf("Val changed to %d\n",val);
	}else {
		printf("parient's val is %d\n",val);
		val=5;
		printf("parient change val to %d\n",val);
	}
	return 0;
}
```

输出：

```
Solving Question 1
parient's val is 10
parient change val to 5
Val now is 10
Val changed to 9
```

子进程有与父进程不同的地址空间，所以修改的是各自的变量，互不影响。

#### Question 2

>Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?

代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	printf("Solving Question 2\n");
	int fd=open("./q2.output",O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
	write(fd,"I'm parrent.\n",13);
	int rc = fork();
	if(rc<0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	} else if (rc == 0) {
		write(fd,"I'm child\n",10);
		write(fd,"child test.\n",12);
	} else {
		write(fd,"Parrent test.\n",14);
	}		
	return 0;
}
```

输出：

```
$ ./q2
Solving Question 2
$ cat q2.output
I'm parrent.
Parrent test.
I'm child
child test.
```

父、子进程都能够访问打开的文件，并发的向文件中写入内容时都能够写入。

#### Question 3

>3. Write another program using fork(). The child process should
print “hello”; the parent process should print “goodbye”. You should
try to ensure that the child process always prints first; can you do
this without calling wait() in the parent?

代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	printf("Solving Question 3\n");
	int rc = fork();
	int par=getpid();
	if (rc<0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		printf("hello\n");
	} else {
		waitpid(rc,NULL,WUNTRACED);
		printf("goodbye\n");
	}
	return 0;
}
```

输出：

```
$ ./q3
Solving Question 3
hello
goodbye
```

#### Question 4

>Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including execl(), execle(), execlp(),
execv(), execvp(), and execvpe(). Why do you think there
are so many variants of the same basic call?

代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	printf("Solving Question 4\n");
	int rc = fork();
	if (rc < 0) {
		fprintf(stderr,"fork failed\n");
		exit(1);
	} else if (rc == 0) {
		//execl("/bin/ls","/bin/ls",(char *)0);
		//execlp("/bin/ls","/bin/ls",(char *)0);
		//execle("/bin/ls","/bin/ls",(char *)0);
		char *args[] = {"/bin/ls",(char *)0 };
		//execv("/bin/ls",args);
		//execvp("/bin/ls",args);
		execvpe("/bin/ls",args);
		
	} else {
		printf("I'm parrent.\n");
	}
	return 0;
}
```

**execl、execle、execlp、execv、execvp、execvP的区别**   
参考于此[http://stackoverflow.com/questions/5769734/what-are-the-different-versions-of-exec-used-for-in-c](http://stackoverflow.com/questions/5769734/what-are-the-different-versions-of-exec-used-for-in-c)

- **L vs V**: whether you want to pass the parameters to the exec'ed program as
    - L: individual parameters in the call (variable argument list): execl(), execle(), execlp(), and execlpe()
    - V: as an array of char* execv(), execve(), execvp(), and execvpe()
    - The array format is useful when the number of parameters that are to be sent to the exec'ed process are variable -- as in not known in advance, so you can't put in a fixed number of parameters in a function call.
- **E**: The versions with an 'e' at the end let you additionally pass an array of char* that are a set of strings added to the spawned processes environment before the exec'ed program launches. Yet another way of passing parameters, really.
- **P**: The versions with 'p' in there use the environment path variable to search for the executable file named to execute. The versions without the 'p' require an absolute or relative file path to be prepended to the filename of the executable if it is not in the current working directory.

#### Question 5
>Nowwrite a programthat uses wait() towait for the child process
to finish in the parent. What does wait() return? What happens if
you use wait() in the child?

代码;

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	printf("Solving Question 5\n");
	int rc = fork();
	if(rc < 0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	} else if (rc == 0) {
		int cc=wait(NULL);
		printf("I'm child (pid:%d),wait() returned %d\n",(int)getpid(),cc);
	} else {
		int wc =wait(NULL);
		printf("I'm parrent of %d (wait returned:%d)\n",rc,wc);
	}
	return 0;
}
```

输出：

```
$ ./q5
Solving Question 5
I'm child (pid:6505),wait() returned -1
I'm parrent of 6505 (wait returned:6505)
```

wait(NULL) 等待任何一个子进程结束。   
wait()返回的是子进程的PID,在此例中子进程中没有子进程，调用wait()后直接返回。

#### Question 6

>Write a slight modification of the previous program, this time using
waitpid() instead of wait(). When would waitpid() be
useful?

代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	printf("Solving Question 5\n");
	int rc = fork();
	if(rc < 0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	} else if (rc == 0) {
		printf("I'm child (pid:%d)\n",(int)getpid());
	} else {
		int wc =waitpid(rc,NULL,WUNTRACED);
		printf("I'm parrent of %d (waitpid() returned:%d)\n",rc,wc);
	}
	return 0;
}
```

输出：

```
$ ./q6
Solving Question 5
I'm child (pid:7790)
I'm parrent of 7790 (waitpid() returned:7790)
```

waitpid()能够方便的等待哪一个进程改变状态。

#### Question 7

>Write a program that creates a child process, and then in the child
closes standard output (STDOUT FILENO).What happens if the child
calls printf() to print some output after closing the descriptor?

代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    printf("Solving question 7\n");
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
		close(STDOUT_FILENO);
        printf("Where is this message\n"); 
    }
    return 0;
}
```
输出：
```
$ ./q7
Solving question 7
```

在关闭标准输出流后掉用printf()不晓得会发生什么，可能回损坏程序，具体参考这个问答[How to close stdout and stderr in C?](http://stackoverflow.com/questions/4972994/how-to-close-stdout-and-stderr-in-c)

#### Question 8

>Write a program that creates two children, and connects the standard
output of one to the standard input of the other, using the
pipe() system call.

代码：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	printf("Solving Question 8\n");

	int pfd[2];

	if(pipe(pfd) == -1) {
		perror("pipe");
		exit(1);
	}

	int pidOne=fork();

	if(pidOne < 0){
		fprintf(stderr,"fork failed\n");
		exit(1);
	}else if(pidOne == 0){
		close(pfd[0]);
		dup2(pfd[1],STDOUT_FILENO);
		printf("Hello,I'm process(pid:%d)\n#",(int)getpid());
	}else ;
	
	int pidTwo=fork();
	
	if(pidTwo<0){	
		fprintf(stderr,"fork failed\n");
		exit(1);
	}else if(pidTwo==0){
		close(pfd[1]);
		printf("process(pid %d) get message:",(int)getpid());
		char c;
		dup2(pfd[0],STDIN_FILENO);
		c=getchar();
		while(c!='#'){
			putchar(c);
			c=getchar();
		}
	}else {}
}
```

输出：

```
$ ./q8
Solving Question 8
process(pid 11089) get message:Hello,I'm process(pid:11088)
```

还可以实现多个线程之间通过管道连接，具体参考[How do I chain stdout in one child process to stdin in another child in C?](http://stackoverflow.com/questions/7281894/how-do-i-chain-stdout-in-one-child-process-to-stdin-in-another-child-in-c)。