#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include "HTTP.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

string file_name(string message);
string getCT(string filename);

int
main(int argc, char *argv[]){
    
    DIR *dir;
    time_t now = time(0);
    string dt = ctime(&now);
    struct dirent *ent;
    string dir1, word, body, filename;
    ifstream inFile;
    int error, bodysize;
    string headermessage,time,server,repsonsemessage,LM,CL,CT, Conn;
    char buff[2000];
    
    if(argc == 1){
        std::cerr << "Nowhere to look"<<std::endl;
        return 1;
    }
    cout << endl;
    dir1=argv[3];
    //std::cout << dir1 << std::endl;
    
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // allow others to reuse the address
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }

  // bind address to socket
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(40000);     // short, network byte order
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 2;
  }

  // set socket to listen status
  if (listen(sockfd, 1) == -1) {
    perror("listen");
    return 3;
  }

  // accept a new connection
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

  if (clientSockfd == -1) {
    perror("accept");
    return 4;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Accept a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;
    std::cout << "connection is open"<< std:: endl;
    char response[] = "HTTP/1.1 200 OK\r\n\r\n!";
  // read/write data from/into the connection
  bool isEnd = false;
  char buf[2048] = {0};
  std::stringstream ss;
    int y=0;
    
  while (y==0) {
    memset(buf, '\0', sizeof(buf));
      
     

    if (recv(clientSockfd, buf, 2048, 0) == -1) {
        char response2[] = "HTTP/1.0 400 Bad Request\r\n\r\n";
        send(clientSockfd, response2, strlen(response2), 0);
        perror("recv");
      return 5;
    }
    else {
        if(buf[0]=='n' && buf[1]=='o'){
            close(clientSockfd);
            break;
        }
        cout << endl;
        std::cout << "HTTP request:" << std::endl;
        char response[] = "HTTP/1.0 200 OK\r\n\r\n";
    }

      //ss << buf <<  std::endl;
      cout << endl;
      std::cout << buf << std::endl;
      cout << endl;
      
      //in here is where we need to put all the data to get the http request
      std::string mess(buf);
      filename= file_name(mess);
      word = filename.substr(0,(filename.length())-1);
      
      // now search for file name! "/Users/davidneill/Desktop/1")
      inFile.open(dir1 + filename.substr(0,(filename.length())-1));
      if (!inFile) {
          error = 1;
          cerr << "Unable to open file, file may not exisit" << filename << endl;
          char response1[]="HTTP/1.0 404 Not Found\r\n\r\n";
          if (send(clientSockfd, response1, strlen(response1), 0) == -1) {
              perror("send");
              return 6;
          }
      }
      else{
          while (inFile >> word){
            body= body+word;
               error=0;
          }
          bodysize = body.length();
          inFile.close();
      }
     
      //Now we need to create a repsonse message
      if(error==0){
          headermessage="HTTP/1.1 200 OK\n";
          time= "Date: " + dt;
          server= "FireFox\n";
          LM = "Last Modified: 6/2/2019, 08:23:45\n";
          CL = "Content length: " + to_string(bodysize) + "\r\n";
          Conn = "Connection-Type: Keep Alive\n";
          CT =getCT(filename);
          
          repsonsemessage = headermessage + time + server +LM + CL + CT + Conn+ body + "             \n\r";
          int n = repsonsemessage.length();
          strcpy(buff, repsonsemessage.c_str());
          body="\n";
          
          
          
          
            if (send(clientSockfd, buff, strlen(buff), 0) == -1) {
                    perror("send");
                return 6;
            }

          cout << "Response sent:"<< endl;
          cout<<buff<< endl;
   
      }
      
      
      error=0;
      memset(buff, '\0', sizeof(buff));
  }
  std::cout << "connection is closed"<< std:: endl;
  close(clientSockfd);
    
    
    
   
   
  return 0;
}
string file_name(string message){
    string filename;
    int x=message.find('/');
    int y=message.find("HTTP");
    filename= message.substr(x,(y-x));
    filename=filename.substr(1,filename.length());
    filename = "/" + filename;
    return filename;
}

string getCT(string filename){
    int x=0;
    x=filename.find('.');
    filename = filename.substr(x,filename.length());
    filename = "Content Type: " + filename + "\n";
    
    return filename;

    
}


