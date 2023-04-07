#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define K 1024

typedef struct dir {
unsigned int my_node;
unsigned short my_length;
unsigned char my_namelen;
}DIRPATH;



unsigned int getint(int fd, int block, int byte);
unsigned short int getshort(int fd, int block, int byte);


int main (int argc , char *argv[])
{
int i;
int nameleng;
char name[K];
DIRPATH dh;
int argument;
int fd;

argument = (argc > 1) ? atoi(argv[1]) : 0;

fd=open(argv[1],O_RDONLY);
fprintf(stderr,"fd=%d\n",fd);
if(lseek(fd,K,SEEK_SET)<0)
{
fprintf(stderr,"unable to seek");
exit(0);
}
int itab = getint(fd,2,8);
unsigned int first_block = getint(fd,itab,inode_size + 40);
int length=0;
for(i=0;i<6;i++)
{
lseek(fd,first_block * K + length,SEEK_SET);
read(fd,&dh,sizeof(DIRPATH));
length += dh.my_length;
printf("",dh.my_namelen);
read(fd,name,dh.my_namelen);
name[dh.my_namelen] = 0;
printf("%s\n",name);
}
close(fd);
}

unsigned int getint(int fd, int block, int byte)
{
unsigned int x;
int check;
lseek(fd,block * K + byte, SEEK_SET);
check = read(fd, &x, 4);
if(check != 4){
fprintf(stderr,"error to read %d %d \n",block, byte);
exit(0);
}
return(x);
}

unsigned short int getshort(int fd, int block, int byte)
{
unsigned short int x;
int check;
lseek(fd,block * K + byte, SEEK_SET);
check = read(fd, &x, 4);
if(check != 2){
fprintf(stderr,"error to read %d %d \n",block, byte);
exit(0);
}
return(x);
}