#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

string get_line(string message);
string get_host(string message);
string body_file(string bodyfile);

int
main(int argc, char *argv[])
{
    std::string message;
    //check if only single argument
    if(argc == 1){
        std::cerr << "Missing URL input"<<std::endl;
        return 1;
    }
    
    
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << sockfd << std::endl;
    



  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(40000);     // short, network byte order
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  // connect to the server
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
    perror("connect");
    return 2;
  }

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
    perror("getsockname");
    return 3;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
  std::cout << "Set up a connection from: " << ipstr << ":" <<
    ntohs(clientAddr.sin_port) << std::endl;

    /* A HTTP request looks like:
     GET /index.html HTTP/1.0\r\n
     User-Agent: "Mozilla/5.0 (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0"\r\n
     Accept: \r\n
     Host: localhost:4000\r\n
     Connection: Keep-Alive\r\n
     \r\n
     */
    message = argv[1];
    string getline= get_line(message);
    string user_agent= "User agent: Mozilla/5.0 (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0\r\n";
    string accept = "Accept: text/html \r\n";
    string gethost= get_host(message);
    string language = "Language: en\r\n";
    string connection = "Connection: Keep-Alive\r\n";
    
    string sendm = getline + user_agent + accept +gethost + language + connection;
    //cout<< sendm << endl;
    
    
    
    
    
    
    
  // send/receive data to/from connection
    string answer;
    int n = sendm.length();
    char buf[n+1];
    char buf1[200];
    std::stringstream ss;
    n = sendm.length();
    strcpy(buf, sendm.c_str());
    int y=0;
  while (y==0) {
    //memset(buf, '\0', sizeof(buf));
      //cout << sendm<< endl;
    
   
    if (send(sockfd, buf, 2000, 0) == -1) {
      perror("send");
      return 4;
    }
     
      cout << "Request sent" << endl;
      cout << endl;


    if (recv(sockfd, buf1, 2000, 0) == -1) {
      perror("recv");
      return 5;
    }
    ss << buf1 << std::endl;
    std::cout << buf1 << std::endl;
      
        //now need to search for words in body text and print to file;
      string bodyfile= body_file(buf1);
      //cout<< bodyfile<< endl;
      ofstream outputFile;
      outputFile.open("writeto.html");
      outputFile << bodyfile << endl;
      outputFile.close();
      
      
      
      cout<< "Any thing else to send"<<endl;
      cin >> answer;
      cout<<endl;
      
      if(answer=="yes"){
          memset(buf, '\0', sizeof(buf));
          cout<< "Enter new url" <<endl;
          cin >> message;
          string getline= get_line(message);
          string user_agent= "User agent: Mozilla/5.0 (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0\r\n";
          string accept = "Accept: text/html \r\n";
          string gethost= get_host(message);
          string language = "Language: en\r\n";
          string connection = "Connection: Keep-Alive\r\n";
          
          string sendm = getline + user_agent + accept +gethost + language + connection;
          
          //cout << sendm<< endl;
          n = sendm.length();
          strcpy(buf, sendm.c_str());
        
      }
      else if (answer=="no") {
          sendm = answer;
          n = sendm.length();
          strcpy(buf, sendm.c_str());
          
          if (send(sockfd, buf, 2000, 0) == -1) {
              perror("send");
              return 4;
          }
          cout << "connection closed\n";
          
          break;
      }
      
    
  }
    

  close(sockfd);

  return 0;
}


string get_line (string message){
    int x=0;
    x=message.find('/');
    message = message.substr(x,message.length());
    message = "GET " + message + " HTTP/1.0\r\n";
    return message;
}

string get_host (string message){
    int x=0;
    x=message.find('/');
    message = message.substr(0,x);
    message = "Host: " + message + " \r\n";
    return message;
}

string body_file(string bodyfile){
    
    int x=bodyfile.find("<html");
    bodyfile = bodyfile.substr(x,bodyfile.length());
    return bodyfile;

    
    
}


