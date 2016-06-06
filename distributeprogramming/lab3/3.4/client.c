#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <stdint.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include "errlib.h"
#include "sockwrap.h"
#include <rpc/xdr.h>
#include "type.h"

#define MAXLINE 4096
#define LISTENQ 15
#define MAXNAME 63
#define TIMEOUT 120
#ifdef TRACE
#define trace(x) x
#else
#define trace(x)
#endif

int sendName(int sfd,char *filename,char *ptr);
int readControl(int sfd);
int readSizeTime(int sfd,int *fileSize);//if int fileSize,it will only store a copy,not able to change value
int readContent(int sfd,char *filename,int fileSize);
int sendName_xdr(int sfd,char *filename,char *ptr);
int readControl_xdr(int sfd);
int readSizeTime_xdr(int sfd,int *fileSize);
char *prog_name;
//int daemon_proc=0;
#define SA struct sockaddr


int reply(int connfd){

	//char buf[MAXBUFL+1]
	return 0;
}

int main(int argc,char* argv[]){

//check arguments
	if(argc<3)
	{
	 printf("usage: %s <>protocol> <address> <port>\n where <protocol> is -x",prog_name);
	 exit(1);
	}
	if( argc==4 )
	{
	 if( (argv[1][0]!='-')||(argv[1][1]!='x') )
	 {
		printf("usage: %s <>protocol> <address> <port>\n where <protocol> is -x",prog_name);
		exit(1);
		}
	}
char *filename=malloc(MAXNAME*sizeof(char));
char *ptr1=malloc(MAXNAME*sizeof(char));
prog_name=argv[0];
int s,sfd,fileSize;
//ssize_t nread;
struct addrinfo hints;
struct addrinfo *result,*rp;

memset(&hints,0,sizeof(struct addrinfo));
hints.ai_family=AF_UNSPEC;
hints.ai_socktype=SOCK_STREAM;
hints.ai_protocol=IPPROTO_TCP;
hints.ai_flags=0;
if(argc==4)
s=getaddrinfo(argv[2],argv[3],&hints,&result);
else
s=getaddrinfo(argv[1],argv[2],&hints,&result);
if(s!=0)
{
    printf("getaddrinfo wrongly\n");
    exit(1);
}
for(rp=result;rp!=NULL;rp=rp->ai_next){
    sfd=socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);

    if(sfd==-1)
    continue;

    if(connect(sfd,rp->ai_addr,rp->ai_addrlen)!=-1){
    printf("connected!!!******\n");
    break;}

    close(sfd);}

    if(rp==NULL){
    trace(err_msg("(%s)---could not connect",prog_name));
    exit(1);
}
    freeaddrinfo(result);
    trace(err_msg("(%s)---start communication.......\n",prog_name));
    while(1){
    memset(ptr1,0,strlen(ptr1));//need to be cleared,ot if 1st file name is longer than 2nd,the previous tail part will be remained
    memset(filename,0,strlen(filename));
    trace(err_msg("(%s)---Please provide file names to be transfered or enter ctl+d TO EXIT :",prog_name));
    int i=0;   
          while(1){
		if(scanf("%c",&ptr1[i++])!=EOF){
                //printf("%d",&ptr1[i]);
		if((ptr1[i-1]=='\n')||(ptr1[i-1]=='\r'))
			break;
                	}
 else{
        ptr1[0]='\0';
        strcat(ptr1, "QUIT\r\n");
        puts(ptr1);	
	if(argc==4){
		message m;
		XDR xdrs_w;
		
		memset(&m,0,sizeof(message));
		m.tag = 2;//should be after memset
		//initialize xdr stream
    		FILE *stream_socket_w = fdopen(sfd,"w");
    		if(stream_socket_w ==NULL)
    			err_sys ("(%s)error - fdopen() failed",prog_name);
    		xdrstdio_create(&xdrs_w,stream_socket_w,XDR_ENCODE);
    
    		//send to the server
    		//if(!xdr_message(&xdrs_w,&m))
    		if (!xdr_tagtype (&xdrs_w, &m))
    		{
    			printf("Error in sending xdr quit messages!\n");
    			xdr_destroy(&xdrs_w);
    			fclose(stream_socket_w);
    			close(sfd);
    			return 0;
    		}
    		fflush(stream_socket_w);
    		printf("I have sent quit information to server!m.tag=%d\n",m.tag);
    		xdr_destroy(&xdrs_w);
    		fclose(stream_socket_w);
    		}
      else{
	int n=writen(sfd,ptr1,strlen(ptr1));
       if(n!=strlen(ptr1))
          printf("errrrrrrr1\n");
      }
          
printf("I'm going to close,strlen(ptr)=%lu",strlen(ptr1));
close(sfd);
return 0;}
  }

   
       trace(err_msg("(%s)----in process of sending file name to server.....",prog_name));
       if(argc==3){
       if(sendName(sfd,filename,ptr1)!=0)
         break;
       }
       else{
       if(sendName_xdr(sfd,filename,ptr1)!=0)
         break;
       }
       if(argc==3){
       if(readControl(sfd)<0)
         break;
        }
        else{
        if(readControl_xdr(sfd)<0)
         break;
        }
        if(argc==3){
       if(!(2==readSizeTime(sfd,&fileSize)))
         break;
         }
         else{
         if(!(2==readSizeTime_xdr(sfd,&fileSize)))
         break;
         }
       printf("ha,I'm in\n");
       if(!(readContent(sfd,filename,fileSize)==1)){
         
         break;}
       
   //memset(&ptr,0,strlen(ptr));???????

          
}
printf("I'm going to close");
close(sfd);
free(ptr1);
return 0;
}

int sendName(int sfd,char *filename,char *ptr){
	//char *ptr=malloc(MAXNAME*sizeof(char));
	int i=0;
        char *full=malloc(MAXNAME*sizeof(char));
       // memset(full,0,MAXNAME*sizeof(char));
        full[i++]='G';
        full[i++]='E';
        full[i++]='T';
        full[i++]=' ';
        full[i]='\0';
        //strncpy(ptr,"GET ",ptr);
        strcat(full,ptr);
        
        //to find the position where there's no \r or \n,to add \0 for simplicity to add \r\n together
       i=strlen(full)-1;//needs to -1,otw it is \0,of course not ='\n' and '\r'
	while(1){
             if(full[i]!='\n'&&full[i]!='\r')
               break;
             --i;
             
                }	
	full[++i]='\0';
	strcat(full, "\r\n"); /* this adds \r\n to the message that have to be sent  */
        printf("full is:%s",full);
   
        //char *filename=(char*)malloc((strlen(full)-5)*sizeof(char));//including '\0'
        strncpy(filename,full+4,strlen(full)-6);
        printf("Filename: %s", filename);
    printf("TVB :-)\n\n");
    
    //send to the server
    if(write(sfd,full,strlen(full))!=strlen(full)){
    perror("partial/failed write\n");
    exit(-1);
   
    }
    trace(err_msg("(%s)---has sent to the server!",prog_name));
    free(full);
    //free(ptr);cz it will delete ptr1 in main
return 0;
}

int sendName_xdr(int sfd,char *filename,char *ptr){
	
				int i=0;
				XDR xdrs_w;
				message m;
				memset(&m,0,sizeof(message));
				/* char *full=malloc(MAXNAME*sizeof(char));
       // memset(full,0,MAXNAME*sizeof(char));
        full[i++]='G';
        full[i++]='E';
        full[i++]='T';
        full[i++]=' ';
        full[i]='\0';
        //strncpy(ptr,"GET ",ptr);
        strcat(full,ptr);*/
        
        //to find the position where there's no \r or \n,to add \0 for simplicity to add \r\n together
       
	printf(":sizeof(m)=%d,sizeof(message)=%d\n",sizeof(m),sizeof(message));
	m.tag = GET;//GET = 0
				
        filename[i]='\0';
        strcat(filename,ptr);
        
        //to find the position where there's no \r or \n,to add \0 for simplicity to add \r\n together
       i=strlen(filename)-1;//needs to -1,otw it is \0,of course not ='\n' and '\r'
	while(1){
             if(filename[i]!='\n'&&filename[i]!='\r')
               break;
             --i;
             
                }	
	strcat(filename, "\n"); /* this adds \r\n to the message that have to be sent  */
   
        //char *filename=(char*)malloc((strlen(full)-5)*sizeof(char));//including '\0'
        //strncpy(filename,full+4,strlen(full)-6);
        printf("Filename: %s", filename);
    printf("TVB :-)\n\n");
    
    
    //initialize xdr stream
    FILE *stream_socket_w = fdopen(sfd,"w");
    if(stream_socket_w ==NULL)
    	err_sys ("(%s)error - fdopen() failed",prog_name);
    xdrstdio_create(&xdrs_w,stream_socket_w,XDR_ENCODE);
    
    //send to the server
    m.message_u.filename = strdup(filename);
    xdr_message(&xdrs_w,&m);
    fflush(stream_socket_w);

    trace(err_msg("(%s)---has sent to the server!",prog_name));
    xdr_destroy(&xdrs_w);
    fclose(stream_socket_w);
    //free(ptr);cz it will delete ptr1 in main
    //free(full);
return 0;
}

int readControl(int sfd){
	char *tbuf=malloc(MAXNAME*sizeof(char));
        fd_set socket_set;
	struct timeval timeout;
	
	/* set connection timeout */
	FD_ZERO(&socket_set); 
	FD_SET(sfd, &socket_set);
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
        int n;
	
	if( (n = Select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout)) == -1)
	{
		printf("select() failed\n");
		return -1;
	}
	if(n > 0)
	{

//can't use readline
	    //Readline(sfd,tbuf,MAXNAME);
	   readline_unbuffered(sfd,tbuf,MAXNAME);
	    sprintf(tbuf,"%s",tbuf);
	    trace(err_msg("(%s)---received control message from server is:%s",prog_name,tbuf));
	    if(strcmp(tbuf,"+OK\r\n")==0){
	      free(tbuf);
	      return 1;}
	    else{
	      free(tbuf);
	      return -1;}}
       else{
           free(tbuf);
           trace(err_msg("(%s)---not receive response after %d seconds",prog_name,TIMEOUT));
           return -1;
      }
}

int readControl_xdr(int sfd){
	char *tbuf=malloc(MAXNAME*sizeof(char));
        fd_set socket_set;
	struct timeval timeout;
	
	/* set connection timeout */
	FD_ZERO(&socket_set); 
	FD_SET(sfd, &socket_set);
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
        int n;
	
	if( (n = Select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout)) == -1)
	{
		printf("select() failed\n");
		return -1;
	}
	if(n > 0)
	{

//can't use readline
	    //Readline(sfd,tbuf,MAXNAME);
	   readline_unbuffered(sfd,tbuf,MAXNAME);
	    sprintf(tbuf,"%s",tbuf);
	    trace(err_msg("(%s)---received control message from server is:%s",prog_name,tbuf));
	    if(strcmp(tbuf,"+OK\r\n")==0){
	      free(tbuf);
	      return 1;}
	    else{
	      free(tbuf);
	      return -1;}}
       else{
           free(tbuf);
           trace(err_msg("(%s)---not receive response after %d seconds",prog_name,TIMEOUT));
           return -1;
      }
}


int readSizeTime(int sfd,int *fileSize){
        fd_set socket_set;
	struct timeval timeout;
	uint32_t value,ts1,ts2;
	/* set connection timeout */
	FD_ZERO(&socket_set); 
	FD_SET(sfd, &socket_set);
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
        int nn;
	
	if( (nn = Select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout)) == -1)
	{
		printf("select() failed\n");
		return -1;
	}
	if(nn > 0)
	{

    int n=readn(sfd,&value,sizeof(uint32_t));
    if(n<0)
    trace(err_msg("(%s)---error in reading files",prog_name));
    ts1=ntohl(value);
    *fileSize=(int)ts1;
    //rbuf[n]='\0';  
    trace(err_msg("(%s)---The file size is:%d,*fileSize=%d",prog_name,(int)ts1,*fileSize));
    //*fileSize=ts1;
    //trace(err_msg("(%s)---The file size is:%u,ts1=%u",prog_name,ntohl(value1),ts1));  
    n=readn(sfd,&value,sizeof(uint32_t));
    ts2=ntohl(value);
    trace(err_msg("(%s)---The file modified time is:%d",prog_name,(int)ts2));
    //trace(err_msg("(%s)---The file modified time is:%u,ts2=%u",prog_name,ntohl(value2),ts2));
   // while(2){}
    return 2;}
    else{
           trace(err_msg("(%s)---not receive response after %d seconds",prog_name,TIMEOUT));
           return -1;
      }
    }
int readSizeTime_xdr(int sfd,int *fileSize){
fd_set socket_set;
	struct timeval timeout;
	uint32_t value,ts1,ts2;
	/* set connection timeout */
	FD_ZERO(&socket_set); 
	FD_SET(sfd, &socket_set);
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
        int nn;
	
	if( (nn = Select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout)) == -1)
	{
		printf("select() failed\n");
		return -1;
	}
	if(nn > 0)
	{

    int n=readn(sfd,&value,sizeof(uint32_t));
    if(n<0)
    trace(err_msg("(%s)---error in reading files",prog_name));
    ts1=ntohl(value);
    *fileSize=(int)ts1;
    //rbuf[n]='\0';  
    trace(err_msg("(%s)---The file size is:%d,*fileSize=%d",prog_name,(int)ts1,*fileSize));
    //*fileSize=ts1;
    //trace(err_msg("(%s)---The file size is:%u,ts1=%u",prog_name,ntohl(value1),ts1));  
    n=readn(sfd,&value,sizeof(uint32_t));
    ts2=ntohl(value);
    trace(err_msg("(%s)---The file modified time is:%d",prog_name,(int)ts2));
    //trace(err_msg("(%s)---The file modified time is:%u,ts2=%u",prog_name,ntohl(value2),ts2));
   // while(2){}
    return 2;}
    else{
           trace(err_msg("(%s)---not receive response after %d seconds",prog_name,TIMEOUT));
           return -1;
      }
}

int readContent(int sfd,char *filename,int fileSize){
 //create a file where to store data
 	char *cbuf=malloc(MAXLINE*sizeof(char));
       printf("dasdaa\n");
        int n=1;
    FILE *fp=fopen(filename,"wb");
    if(NULL==fp){
      printf("Error opening file");
      exit(-1);
    }
        fd_set socket_set;
	struct timeval timeout;
	int nn;
	/* set connection timeout */
	FD_ZERO(&socket_set); 
	FD_SET(sfd, &socket_set);
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	
	if( (nn = Select(FD_SETSIZE, &socket_set, NULL, NULL, &timeout)) == -1)
	{
		printf("select() failed\n");
		return -1;
	}
	if(nn > 0)
	{
        trace(err_msg("(%s)----I'm going to download....fileSize=%d",prog_name,fileSize));
    //fileSize=55330;
    int bRemained=fileSize;
    int rBytes,i=0;
    while(bRemained>0)
    {i++;
    if(bRemained>=MAXLINE)
      {rBytes=MAXLINE;}
    else
      {rBytes=bRemained;}
    //printf("\ni=%d",i);
   

// printf("rBytes=%d,bRemained=%d\n",rBytes,bRemained);
    n=readn(sfd,cbuf,rBytes);//cz readline_un stopps when there's \n,so one line once
    //n=readline_unbuffered(sfd,cbuf,rBytes);
   // if(n!=rBytes)
     // trace (err_msg("(%s) n=%d error - readn() failed", prog_name,n));
    //cbuf[n]='\0';  

 // printf("Bytes received %d\n",n);
    int f=0;
    //while(f<n)
    f=fwrite(cbuf,1,n,fp);
     if(f<n)
     printf("error\n"); 
    //memset(cbuf,0,n);
    bRemained-=n;
    }trace(err_msg("(%s)---download finished!,times=%d,rBytes=%d",prog_name,i,rBytes));
return 1;}
else{
           trace(err_msg("(%s)---not receive response after %d seconds",prog_name,TIMEOUT));
           return -1;
      }
}
