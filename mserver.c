#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"

#define PORT 4444
#define BUF_SIZE 2000
#define CLADDR_LEN 100

struct element{
   char each[20];
};  
  struct element message[4];
   char temp[20];
   
void main() {
 struct sockaddr_in addr, cl_addr;
 int sockfd, len, ret, newsockfd;
 char buffer[BUF_SIZE];
 pid_t childpid;
 char clientAddr[CLADDR_LEN];
 
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) {
  printf("Error creating socket!\n");
  exit(1);
 }
 printf("Socket created...\n");
 
 memset(&addr, 0, sizeof(addr));
 addr.sin_family = AF_INET;
 addr.sin_addr.s_addr = INADDR_ANY;
 addr.sin_port = PORT;
 
 ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
 if (ret < 0) {
  printf("Error binding!\n");
  exit(1);
 }
 printf("Binding done...\n");

 printf("Waiting for a connection...\n");
 listen(sockfd, 5);

 for (;;) { //infinite loop
  len = sizeof(cl_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
  if (newsockfd < 0) {
   printf("Error accepting connection!\n");
   exit(1);
  }
  printf("Connection accepted...\n");

  inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
  if ((childpid = fork()) == 0) { //creating a child process

   close(sockfd); 
//stop listening for new connections by the main process. 
//the child will continue to listen. 
//the main process now handles the connected client.

   for( ; ; ){
    memset(message, 0, sizeof(message));
    ret = recvfrom(newsockfd, &message, sizeof(message), 0, (struct sockaddr *) &cl_addr, &len);
    if(ret < 0) {
     printf("Error receiving data!\n");  
     exit(1);
    }
    printf("Received data from %s: \n", clientAddr);
     for(int i=0;i<4;i++)
  fputs(message[i].each, stdout);
   printf("\n");
     
    // sorting
    for(int i=0;i<3;i++)
     {
         for(int j=i+1;j<4;j++)
         {
             if(strcmp(message[i].each,message[j].each)>0)
             {
                 strcpy(temp,message[i].each); 
                 strcpy(message[i].each,message[j].each);
                 strcpy(message[j].each,temp);
             }
         }
     }

    ret = sendto(newsockfd, &message, sizeof(message), 0, (struct sockaddr *) &cl_addr, len);   
    if (ret < 0) {  
     printf("Error sending data!\n");  
     exit(1);  
    }  
    printf("Sent data to %s: \n", clientAddr);
     for(int i=0;i<4;i++)
   fputs(message[i].each, stdout);
   printf("\n");
    
  }
  }
  close(newsockfd);
 }
}

