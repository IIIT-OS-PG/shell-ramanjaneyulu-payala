#include<iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include<map>
#define MAXLINE 100
void Redirect(char **);
void Pipe(char *);
using namespace std;
int pipe_Called=1; 
map<string,char *>mps;
void setEnv(){
  ofstream myfile;
  myfile.open ("mybashrc");
   char myhost[100];
    myhost[100]='\0';
    gethostname(myhost,100);
  myfile<<myhost<<endl;  
  myfile<<getenv("USER")<<endl;
  myfile<<getenv("PATH")<<endl;
  myfile <<getenv("HOME")<<endl;
  
  myfile.close();
}
void getEnv(){
    char myhost[100];
    myhost[100]='\0';
    gethostname(myhost,100);
   cout<<getenv("USER")<<"@"<<myhost<<"$";

}
void alias(char buf[]){
    char *l=(char *)malloc(10*sizeof(char));
     char *r=(char *)malloc(10*sizeof(char));
 char temp[MAXLINE];
    strcpy(temp,buf);
char *token = strtok(buf,"=");
char *par[10];
int i=0;
while (token)
    {
        par[i++]=token;
        token = strtok(NULL,"=");
    }
    strcpy(r,par[i-1]);   
char *mp[3];    
char *st=strtok(par[0]," ");
int j=0;    
while (st)
    {
        mp[j++]=st;
        st=strtok(NULL," "); 
    }      
    strcpy(l,mp[j-1]);

  string Str = string(l);  
   
 mps.insert(make_pair(Str,r));
cout<<"map size---"<<mps.size()<<endl;
for (auto itr = mps.begin(); itr != mps.end(); ++itr) { 
        cout << itr->first 
             << '\t' << itr->second << '\n'; 
    } 

}
// void General(){

// }
void replaceAlias(char *par[],char *l,char *r){
 int v=0;
 int count=0;  
    char *par1[10];
    while(par[v]!=NULL){
   // cout<<"l::"<<l<<endl;     
    if(strcmp(par[v],l)==0){
    //cout<<"par::"<<par[0]<<endl;
        break;
    }
    v++;
    }
    //par[v]=r;
    int i6=v;
   // cout<<"r---"<<par[0]<<endl;
    char *ali = strtok(r," ");
    char *buf4[10];
    int d=0;
    while (ali)
    {
        buf4[d++]=ali;
        ali = strtok(NULL," ");
    }
    //buf4[d]=NULL;
    cout<<"buf4---::"<<buf4[0]<<endl;
    int i1=0;
    while(i1!=v){
        par1[i1]=par[i1];
        count++;
        i1++;
    }  
    int i2=0;
    while(i2!=d){
        par1[v++]=buf4[i2++];
        count++;
    }
    i6++;
    while(par[i6]!=NULL){
     par1[v++]=par[i6];
     count++;
     i6++;
    } 
    par1[v]=NULL;
    cout<<"par::"<<par1[0];
    cout<<"count"<<count<<endl;
    int k=0;
    char bufr[MAXLINE];
    for(int i=0;i<count;i++){
        int q=0;
        while(par1[i][q]){
         bufr[k++]=par1[i][q++];
        }
        bufr[k++]=' ';
    }
    bufr[k]='\0';
    cout<<"bufr::"<<bufr<<endl;
    
    int k1=0;
int f1=0,f2=0;
while(par1[k1]!=NULL){
    if(strcmp(">",par1[k1])==0||strcmp(">>",par1[k1])==0)
    f1=1;
    if(strcmp("|",par1[k1])==0)
    f2=1;
k1++;
}
if(f1){
Redirect(par1);
return;
}
if(f2){
    pipe_Called=0;
Pipe(bufr);
return;
}
    int pid,status;
    if ((pid = fork()) < 0) {
    cout<<"fork error";
} else if (pid == 0) {

    execvp(par1[0],par1);
}
if ((pid = waitpid(pid, &status, 0)) < 0)
cout<<"wait"<<endl;
exit(0);
 }
void Pipe(char *buf){
    cout<<"buf:p:"<<buf;
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
            //close(pipefd[1]);
        } 
        else 
        {
            close(pipefd[1]);
             wait(NULL);
            fd = pipefd[0];
            com++;
        }
       
    }
}


void Redirect(char* par[]){
int k=0,f2=0,f1=0;
while(par[k]!=NULL){
    if(strcmp(">>",par[k])==0){
        f2=1;
        break;
    }
    if(strcmp(">",par[k])==0){
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

   int pid,status;
    if ((pid = fork()) < 0) {
    cout<<"fork error";
} else if (pid == 0) {

    execvp(par[0],par);
}

}


int main()
{
char buf[MAXLINE];
char *par[10];
char *par1[10];
pid_t pid;
int status;


while (1) {
    getEnv();
fgets(buf, MAXLINE, stdin);
if (buf[strlen(buf) - 1] == '\n')
buf[strlen(buf) - 1] = '\0';
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

if(!strcmp(par[0],"exit")) 
 exit(0);   

if(strcmp("cd",par[0])==0){
    if(!strcmp("~",par[1])){
     chdir(getenv("HOME"));
    }
    else if(chdir(par[1])<0)
      cout<<"no such directory found"<<endl;
    continue;
}

if ((pid = fork()) < 0) {
cout<<"fork error";
} else if (pid == 0) {
int j=0;
while(par[j]!=NULL){
    string Str = string(par[j]);
    auto it=mps.find(Str);
    char left[Str.size()+1];
    char *rigt;
    if(it!=mps.end()){
       // cout<<"entered"<<endl;
    strcpy(left,Str.c_str());  
    rigt=it->second;
    replaceAlias(par,left,rigt);
    }
    j++;
}    
char *ch=buf;
int k=0;
int f1=0,f2=0,f3=0;
while(par[k]!=NULL){
    if(strcmp(">",par[k])==0||strcmp(">>",par[k])==0)
    f1=1;
    if(strcmp("|",par[k])==0)
    f2=1;
    
    
k++;
}
if(strcmp("alias",par[0])==0){
    alias(buf1);
   // cout<<"l:"<<l<<" r:"<<r<<endl;
    }

if(f1){
Redirect(par);
continue;
}
if(f2&&pipe_Called){
Pipe(buf1);
continue;
}
if(!strcmp("setenv",par[0])){
    setEnv();
    continue;
}

//else
execvp(par[0],par);
//exit(1);
}
if ((pid = waitpid(pid, &status, 0)) < 0)
cout<<"wait";
}

}