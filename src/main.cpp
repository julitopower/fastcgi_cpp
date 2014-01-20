//#include <exception>
#include <iostream>
#include <unistd.h>
#include <ctime>
//#include <cstdlib>

#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

//#include <fastcgi.h>
#include <fcgio.h>

int getSocket(int port);
void diff(timespec start, timespec end);

int main(int argc, char** argv){
  timespec init, end;
  std::cout << "[INFO] Starting  Server on port 1234" << std::endl;
  int serverSocket = getSocket(9999);

  std::cout << "Got socket number " << serverSocket << std::endl; 

  std::streambuf * cin_streambuf  = std::cin.rdbuf();
  std::streambuf * cout_streambuf = std::cout.rdbuf();
  std::streambuf * cerr_streambuf = std::cerr.rdbuf();


  FCGX_Request request;
  FCGX_Init();
  FCGX_InitRequest(&request, serverSocket, 0);

  int status = FCGX_Accept_r(&request);
  std::cout << "Accept status is " << status << std::endl;
  while(status == 0) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &init);
    std::cout << "Have a request" << std::endl;

    fcgi_streambuf cin_fcgi_streambuf(request.in);
    fcgi_streambuf cout_fcgi_streambuf(request.out);
    fcgi_streambuf cerr_fcgi_streambuf(request.err);

    std::cin.rdbuf(&cin_fcgi_streambuf);
    std::cout.rdbuf(&cout_fcgi_streambuf);
    std::cerr.rdbuf(&cerr_fcgi_streambuf);

    std::cout << "Centent-type: text/html\r\n\r\n";
    std::cout << "<html><body>Hi there<p>" << std::endl;
    
    char **env = request.envp;
    while (*(++env))
      std::cout << *env << "<p>" << std::endl;

    std::cout << "</body></html>" << std::endl;
    FCGX_Finish_r(&request);

    std::cin.rdbuf(cin_streambuf);
    std::cout.rdbuf(cout_streambuf);
    std::cerr.rdbuf(cerr_streambuf);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    diff(init, end);
    status = FCGX_Accept_r(&request);
  }
}

int getSocket(int port) {
  int status  = 0;
  int socketNumber = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  std::cout << "Socket number " << socketNumber << std::endl;

  struct sockaddr_in server;
  memset(&server, 0 , sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(port);

  status = bind(socketNumber, (struct sockaddr *) & server, sizeof(server));

  std::cout << "Bind status " << status << std::endl;

  listen(socketNumber, 32);
  return socketNumber;
}

void diff(timespec start, timespec end)
{
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  std::cout << temp.tv_sec << ":" << temp.tv_nsec/1000000.0f << std::endl;
}



