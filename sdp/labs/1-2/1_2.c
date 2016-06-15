#include <signal.h>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

#define BUFLEN 256

int count=0;
int flag = 0;

void sigHandler1(){
	flag = 1;
	count++;
	printf("I caught signal SIGUSR1\n");
	signal(SIGUSR1,sigHandler1);
}

void sigHandler2(){
	printf("I terminate now!\n");
	exit(0);
}

int main(int argc,char *argv[]){
	
	if( argc<2 ){
		printf("usage:filename\n");
		exit(-1);
	}
	
	FILE *fp;
	int i=0;
	char buf[BUFLEN];
	time_t begin;
	srand(time(0));
	int t;
	pid_t pid  = getpid();;
	
	
	fp = fopen(argv[1],"r");
	
	if(fork()>0){
	
	while(1){
	
		signal(SIGUSR1,sigHandler1);
		signal(SIGUSR2,sigHandler2);
		
		while(fgets( buf,BUFLEN,fp )){//read line by line
			if(count%2 == 0 && flag == 1){
				pause();
				flag = 0;
			}
			else{
			if(flag == 1)
			{rewind(fp);
			i = 0;
			flag = 0;
			}
			fgets(buf,BUFLEN,fp);
			printf("line:%d,content:%s",++i,buf);
			
			//to slow down 
			sleep(3);
			}
			}
		rewind(fp);
		i = 0;
	
	}
	//how to release fp?
	}
	else{	
		begin = time(NULL);
		while(time(NULL)-begin<20){
		t = rand()%11 + 1;
		sleep(t);
		kill(pid,SIGUSR1);
		}		
		kill(pid,SIGUSR2);
		printf("I'm child,also terminates!\n");
		exit(0);
	}
	return 0;
}
