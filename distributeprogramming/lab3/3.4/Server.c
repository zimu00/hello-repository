#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <stdint.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include<sys/stat.h>
#include "sockwrap.h"
#include "errlib.h"

#define TIMEOUT 10
#define MAXLINE 4095
#define LISTENQ 15
#define MAXNAME 63
#ifdef TRACE
#define trace(x) x
#else
#define trace(x)
#endif

char *prog_name;
char *name_xdr;
//int daemon_proc=0;
#define SA struct sockaddr
 
 //unsigned int size,t;

int reply(int connfd){

	//char buf[MAXBUFL+1]
	return 0;
}
int getRequest(int connfd,char *name);
int sendControl(int connfd,char *name);
int sendContent(int connfd,char *name);
int sendSizeTime(int connfd,char *name);

int getRequest_xdr(int connfd,FILE *stream_socket_r);
int sendControl_xdr(int connfd,FILE *stream_socket_w);
int sendContent_xdr(int connfd,FILE *stream_socket_w);
int sendSizeTime_xdr(int connfd,FILE *stream_socket_w);

int main(int argc,char *argv[]){

	int listenfd,connfd,err=0;
        uint16_t port;
        char name[MAXNAME];
	struct sockaddr_in servaddr,cliaddr;
        socklen_t cliaddrlen=sizeof(cliaddr);	
        //FILE *fp;
	prog_name = argv[0];
	port = atoi(argv[1]);
	//check arguments
	if(argc<2)
	{
	 printf("usage: %s <>protocol> <address> <port>\n where <protocol> is -x",prog_name);
	 exit(1);
	}
	if( argc==3 )
	{
	 if( (argv[1][0]!='-')||(argv[1][1]!='x') )
	 {
		printf("usage: %s <>protocol> <address> <port>\n where <protocol> is -x",prog_name);
		exit(1);
		}
	}
	
	//create the socket
	listenfd=socket(AF_INET,SOCK_STREAM,0);
    
	if(listenfd<0){
	printf("error in creating socket!");
	exit(1);	
	}
	//specify address to bind to
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

	int b=bind(listenfd,(SA*) &servaddr,sizeof(servaddr));
	if(b<0){
	printf("error in binding!\n");
	exit(1);
}
	trace(err_msg("(%s) socket created",prog_name));
	trace(err_msg("(%s) listending on %s:%u",prog_name,inet_ntoa(servaddr.sin_addr),ntohs(servaddr.sin_port)));
        int c=listen(listenfd,LISTENQ);
if(c<0){
	printf("error in listening!\n");
	exit(1);
}
	while(1){
		trace(err_msg("(%s)waiting for connection...",prog_name));
		int retry=0;



		do{
			connfd=accept(listenfd,(SA*)&cliaddr,&cliaddrlen);
			if(connfd<0){
			trace(err_msg("(%s)accept() failed!,prog_name"));	
			exit(1);		
			}
			else{
                        
			trace(err_msg("(%s)new connection from client %s:%u",prog_name,inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port)));
/* set the timeout for the message received from the connected client */
			
 			FILE *stream_socket_r = fdopen(sfd,"r");
    			if(stream_socket_r==NULL){
       			printf("error in creating stream\n");
    
   			 }
    			FILE *stream_socket_w = fdopen(sfd,"w");
    			if(stream_socket_w ==NULL){
    				err_sys ("(%s)error - fdopen() failed",prog_name);
    			}
                        while(1){
                        trace(err_msg("(%s)new wait for commands....",prog_name));
                        fd_set socket_set;
	                struct timeval timeout;
			FD_ZERO(&socket_set); 
			FD_SET(connfd, &socket_set);
			timeout.tv_sec = TIMEOUT;
			timeout.tv_usec = 0;
			int n;
			if( (n = Select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout)) == -1)
			{
				printf("select() failed\n");
				break;
			}
			if(n > 0)
			{
			 printf("go to receive name!");retry=1;
                        //clear buffer
                       // memset(buf,0,MAXLINE);
                        //memset(name,0,MAXNAME);
                        if(argc==2){
                        int r=getRequest(connfd,name);
                        //sprintf(name);
            /*Upon successful completion, fopen() shall return a pointer to the o bject controlling the stream. Otherwise, a null pointer shall be returned, [CX] [Option Start]  and errno shall be set to indicate the error. [Option End],The proper thing to do is check errno when fopen returns NULL*/
                        if(r==1){
		        if(!(0==sendControl(connfd,name)))
                        break;
                        //j=0;
                        if(!(0==sendSizeTime(connfd,name)))
                        break;
                        if(!(0==sendContent(connfd,name)))
                        break;
        }
        else              
	{trace(err_msg("(%s)-connection closed by %s",prog_name,(err==0)?"client":"server"));
        close(connfd);
        retry=0;
        break;
        }
}
/*******************xdr***********************/
 else{
                        int r=getRequest_xdr(connfd,stream_socket_r);
                        //sprintf(name);
            /*Upon successful completion, fopen() shall return a pointer to the o bject controlling the stream. Otherwise, a null pointer shall be returned, [CX] [Option Start]  and errno shall be set to indicate the error. [Option End],The proper thing to do is check errno when fopen returns NULL*/
                        if(r==1){
		        if(!(0==sendControl_xdr(connfd,stream_socket_w)))
                        break;
                        //j=0;
                        if(!(0==sendSizeTime_xdr(connfd,stream_socket_w)))
                        break;
                        if(!(0==sendContent_xdr(connfd,stream_socket_w)))
                        break;
        }
        else              
	{trace(err_msg("(%s)-connection closed by %s",prog_name,(err==0)?"client":"server"));
        close(connfd);
        retry=0;
        break;
        }
}
/*********************************************/
 }else{
        trace(err_msg("(%s)-Timeout!",prog_name));
        close(connfd);
        fclose(stream_socket_w);
        fclose(stream_socket_r);
        retry=0;
        }	
}retry=0;}	}while(retry);}
return 0;
}


/*-----------------------get request of the File-------------------*/
int getRequest(int connfd,char name[MAXNAME]){
		
		char buf[MAXLINE];
               // memset(buf,0,MAXNAME);
                int j=readline_unbuffered(connfd,buf,MAXNAME);
                     if (j == 0) 
                {
			return 0;
		} else if (j < 0) 
		{
			err_ret ("(%s) error - readline() failed", prog_name);
			/* return to the caller to wait for a new connection */
			return 0;
		}
        //cz readline_unb checked ending through \n,so both \n,\r will be stored
//bu only need name,so 4->j-2-1
			int i=0;
                        printf("haha check,j=%d\n",j);
			for(;i<j;i++)
                        printf("%c %d+  ",buf[i],buf[i]);
			//store name
                  	if( ((0!=strcmp(buf,"QUIT\n"))) && ((0!=strcmp(buf,"QUIT\r\n"))) )
                  {    printf("client doesn't quit....\n");
                        char dest[5];
                        strncpy(dest,buf,4);
                        dest[4]='\0';
                        if(!strcmp(dest,"GET ")==0)
                           err_sys ("(%s) --- incorrect request format: sending ERR message", prog_name);
                        else{
                        trace(err_msg("(%s)--the name of the file I got is:",prog_name));
//cz not need \r\n,i-2
			
                        for(j=0;buf[j+4]!='\r';j++){
                        name[j]=buf[j+4];
                 
			}
			name[j]='\0';
                        //}   
                        char n[MAXNAME];
                        sprintf(n,"%s"); 
                        puts(n); 
printf("check name again!\n"); 
//name has problem
int k=0;
for(;k<strlen(name);k++)
                        printf("k=%c %d,"[k][k]);	

}return 1;}
else
 return 0;
}

/*-----------------------get request of the File-------------------*/
int getRequest_xdr(int connfd,FILE *stream_socket_r){
		
		char buf[MAXLINE];
		message m;
		memset(&m,0,sizeof(message));
		XDR xdrs_r;
				
		xdrstdio_create(&xdrs_r,stream_socket_r,XDR_DECODE);
		xdr_message(&xdrs_r,&m);
               // memset(buf,0,MAXNAME);
              name_xdr=strdup(m.message_u.filename);
                   
			//store name
                  	if( ((0!=strcmp(buf,"QUIT\n"))) && ((0!=strcmp(buf,"QUIT\r\n"))) )
                  {    printf("client doesn't quit....\n");
                        char dest[5];
                        strncpy(dest,buf,4);
                        dest[4]='\0';
                        if(!strcmp(dest,"GET ")==0)
                           err_sys ("(%s) --- incorrect request format: sending ERR message", prog_name);
                        else{
                        trace(err_msg("(%s)--the name of the file I got is:",prog_name));
//cz not need \r\n,i-2
			
                        for(j=0;buf[j+4]!='\r';j++){
                        name[j]=buf[j+4];
                 
			}
			name[j]='\0';
                        //}   
                        char n[MAXNAME];
                        sprintf(n,"%s"); 
                        puts(n); 
printf("check name again!\n"); 
//name has problem
int k=0;
for(;k<strlen(name);k++)
                        printf("k=%c %d,"[k][k]);	

}return 1;}
else
 return 0;
}

/*-----------------------send Control of the File-------------------*/
int sendControl(int connfd,char *name){
			 char begin[]={'+','O','K','\r','\n'};
char err[]={'-','E','R','R','\r','\n'};
                        FILE *fp=fopen(name,"rb");//rb??
                        int k=0;
			for(;k<strlen(name);k++)
                        printf("kk=%c %d,"[k][k]);
                        if(fp==NULL){
                        trace(err_msg("(%s)--open file failed!",prog_name));
                        int errnum=errno;
                        trace(err_msg("(%s)--value of errno is:%d",prog_name,errnum));
                        
                        Writen(connfd,err,strlen(err));
                        err_sys ("(%s) --- incorrect request format: sending ERR message", prog_name);
                        printf("fd\n");
                        return -1;
}                       
 //reply to the client  
                        Writen(connfd,begin,strlen(begin));
                       /* if(w==0){
                          printf("no ok data sent!\n");
                        }
                        if(w<0){
                          printf("error to send ok data\n");
                        }*/
                        printf("send ok successfully!\n");
                        fclose(fp);
return 0;
}
/*-----------------------send Size and time of the File-------------------*/
int sendSizeTime(int connfd,char *name){
                        struct stat st;
                        uint32_t ssize,tt;
		        time_t time;
 			int r=stat(name,&st);
                        if(r<0){
                        printf("stat unsuccessfully!");
                        }
                        uint32_t size,t;
                        //fstat(&fp,&st);
                        //size=(unsigned int)st.st_size;
                        size=(uint32_t)st.st_size;
                        ssize=htonl(size);
                        //ssize[1]='\n';
                        time=st.st_mtime;
                        //t=(unsigned int) time;
                        t=(uint32_t)time;
                        tt=htonl(t);
                        //tt[1]='\n';
                        trace(err_msg("(%s)---the size and the time of the file is :%u %u",prog_name,size,t));
			Writen(connfd,&ssize,sizeof(uint32_t));
                
                        printf("send size successfully!\n");
                        Writen(connfd,&tt,sizeof(uint32_t));
                        printf("send time successfully!\n");
return 0;
}
/*-----------------------send Content of the File-------------------*/

int sendContent(int connfd,char *name){
 int j=0,c,n;
 FILE *fp=fopen(name,"rb");
 char content[MAXLINE];
			//while(fread(content,sizeof(char),));
                      while(1){
                       while(1){
                        
                       // printf("j=%d,%c",j,content[j-1]);
                        if(j==(MAXLINE-1))
                        {//printf("The file size is too big!");
                         break;
                        }
                        content[j]=fgetc(fp);
                        if(feof(fp))
                          break;
                        j++;
                        }
                        
                        c=j;
                        if(c==0)
                        break;
                        n=writen(connfd,content,c);
                        if(n!=c)
                        err_sys ("(%s) error - writen() failed", prog_name);
                        memset(content,0,strlen(content));
                        j=0;
                        }
                        printf("send content successfully\n");
		        return 0;
}
