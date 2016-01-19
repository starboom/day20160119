#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

//tail 显示文件file 的 末尾 num行
// 默认为 10 行


#define SIZE 1024
#define off_set int
#define line_ int

/*	
	函数名：ReadLine
	功能：读取文件的总行数
	参数：int fd
		：文件描述符
	返回值：文件的总行数
*/
line_ ReadLine(int fd)
{
	char buf;
	int line;
	line = 1;
	while(read(fd,&buf,1))
	{
		if(buf == '\n')
		{
			line++;
			buf = '\0';
		}	
	}
	return line;
}
/*	
	函数名：ReadFile
	功能：获取文件想要的偏移值
	参数：int fd       int NumLine  int FileLine
		：文件描述符       目标行数		文件总行数
	返回值：文件相对于目标行数位置的偏移量
*/
off_set ReadFile(int fd,int NumLine,int FileLine)
{
	char buf;
	int line;
	int OffLen;
	int i;

	i = 0;
	OffLen = 0;
	line = 0;


	while(read(fd,&buf,1))
	{
			if(buf == '\n')
			{

				OffLen = i;
				line++;
				if(line == FileLine - NumLine)
				{
					return OffLen;
				}
				
				buf = '\0';
			}
			i++;	
		}
}



int main(int argc, char const *argv[])
{
	int fd;
	int _argv;
	char buf[SIZE];
	bzero(buf,0);

	if(strstr(argv[1],"-") != NULL)
	{
		_argv = atoi(argv[1]);		

		/*读取文件*/
		fd = open(argv[2],O_CREAT|O_RDWR,0777);
		if(fd == -1)
		{
			perror("file open ");		
		}
	} 
	else
	{
		_argv = 10;		
		/*读取文件*/
			fd = open(argv[1],O_CREAT|O_RDWR,0777);
			if(fd == -1)
			{
				perror("file open ");		
			}

	}
	int offset;
	int FileLine;
/*读取行数*/
	FileLine = ReadLine(fd);
	lseek(fd,0,SEEK_SET); // 复位文件指针 让其重新指在文件开头



/*读取偏移值*/
	offset = ReadFile(fd,abs(_argv),FileLine);

/*设置文件指针的偏移起始位置*/
	lseek(fd,offset,SEEK_SET); //不加1 会读一个‘\n’
	read(fd,buf,1);
	if(buf[0] == '\n')
	{
		lseek(fd,offset + 1,SEEK_SET); //不加1 会读一个‘\n’
	}
	else
	{
		lseek(fd,offset,SEEK_SET);
	}
/*读文件操作*/	
	read(fd,buf,SIZE);

	buf[SIZE - 1] = '\0';
	printf("%s\n",buf );

	return 0;
}

