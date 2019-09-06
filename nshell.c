#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 100
void alias(char *buf){

}
void General(){

}

void Pipe(char *buf){
    int pipefd[2];
  char *par[10];
int c=1;
char *ch;  
int k=0;
ch=buf;
while(ch[k]!='\0'){
 if(ch[k]=='|')
 c++;
 k++;
}
int i=0;
char *token = strtok(buf,"|");
 while (token)
    {
        par[i++]=token;
        token = strtok(NULL,"|");
    }
    par[i]=NULL;    
    int com=0;
    int fd=0;
    while(com<=c)
    {
        int j=0;
        char *arg[10];
        char *parts = strtok(par[com]," ");
        while (parts)
        {
            arg[j++]=parts;
            parts = strtok(NULL," ");
        }

        arg[j]=NULL;    
        //printf("arg--^%s$\n",arg[0]);

        pipe(pipefd);
        if (fork() == 0) 
        {
            dup2(fd,0);
            close(pipefd[0]);
            if(com+1!=c)
            {     
                dup2(pipefd[1], 1);
            }
            execvp(arg[0], arg);
            close(pipefd[1]);
        } 
        else 
        {
            close(pipefd[1]);
            // wait(NULL);
            fd = pipefd[0];
            com++;
        }
       
    }
}


void Redirect(char* par[]){
    //printf("enterd\n");
int k=0,f2=0,f1=0;
char *a=">";
char *b=">>";
while(par[k]!=NULL){
    if(strcmp(b,par[k])==0){
        f2=1;
        break;
    }
    if(strcmp(a,par[k])==0){
        f1=1;
        break;
    }
    k++;
}
par[k]=NULL;
int fd2;
if(f1){
    if ((fd2 = open(par[k+1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
    perror("couldn't open output file.");
    exit(0);
    }
dup2(fd2, 1);
close(fd2);
}
if(f2){
    if ((fd2 = open(par[k+1], O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0) {
    perror("couldn't open output file.");
    exit(0);
}
dup2(fd2, 1);
close(fd2);
}

}


int main()
{
char buf[MAXLINE];
char *par[10];
pid_t pid;
int status;
printf("%%"); 

while (1) {
fgets(buf, MAXLINE, stdin);
if (buf[strlen(buf) - 1] == '\n')
buf[strlen(buf) - 1] = '\0';
char ali[MAXLINE];
strcpy(ali,buf);
//if(strcmp("alias",par[0])==0)

char buf1[MAXLINE];
strcpy(buf1, buf);
char *token = strtok(buf," ");
int i=0;
    while (token)
    {
        par[i++]=token;
        token = strtok(NULL," ");
    }
    par[i]=NULL;    


if ((pid = fork()) < 0) {
printf("fork error");
} else if (pid == 0) {
char *ch=buf;
int k=0;
int f1=0,f2=0,f3=0;
char *a=">";
char *b=">>";
char *c="|";
while(par[k]!=NULL){
    if(strcmp(a,par[k])==0||strcmp(b,par[k])==0)
    f1=1;
    if(strcmp(c,par[k])==0)
    f2=1;
    k++;
}
if(f1)
Redirect(par);
if(f2)
Pipe(buf1);
//else

exit(1);
}
if ((pid = waitpid(pid, &status, 0)) < 0)
printf("waitpid error");
printf("%% ");
}
exit(0);
}