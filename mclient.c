#include"stdio.h"  
#include"stdlib.h"  
#include"sys/types.h"  
#include"sys/socket.h"  
#include"string.h"  
#include"netinet/in.h"  
#include"netdb.h"
  
#define PORT 4444 
#define BUF_SIZE 2000 
 
 struct element{
   char each[20];
}; 
  struct element  server_reply[4]; 
    int  i;   
    struct element sort[4];
     
int main(int argc, char**argv) {  
 struct sockaddr_in addr, cl_addr;  
 int sockfd, ret;  
 char buffer[BUF_SIZE];  
 struct hostent * server;
 char * serverAddr;

 if (argc < 2) {
  printf("usage: client < ip address >\n");
  exit(1);  
 }

 serverAddr = argv[1]; 
 
 sockfd = socket(AF_INET, SOCK_STREAM, 0);  
 if (sockfd < 0) {  
  printf("Error creating socket!\n");  
  exit(1);  
 }  
 printf("Socket created...\n");   

 memset(&addr, 0, sizeof(addr));  
 addr.sin_family = AF_INET;  
 addr.sin_addr.s_addr = inet_addr(serverAddr);
 addr.sin_port = PORT;     

 ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
 if (ret < 0) {  
  printf("Error connecting to the server!\n");  
  exit(1);  
 }  
 printf("Connected to the server...\n");  

 memset(sort, 0, sizeof(sort));
 printf("Enter strings for sorting: \n");
 for(int k=0;k<4;k++)
 fgets(sort[k].each, sizeof(sort[k].each), stdin);
 
  ret = sendto(sockfd, sort, sizeof(sort), 0, (struct sockaddr *) &addr, sizeof(addr));  
  if (ret < 0) {  
   printf("Error sending data!\n\t");  
  }
  ret = recvfrom(sockfd, sort, sizeof(sort), 0, NULL, NULL);  
  if (ret < 0) {  
   printf("Error receiving data!\n");    
  } else {
   printf("Received: \n");
   for(i=0;i<4;i++)
   fputs(sort[i].each, stdout);
   printf("\n");
    
 }
 close(sockfd);
    
}  
