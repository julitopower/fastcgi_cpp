#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

#include <fcgio.h>

int getSocket(int port);
void diff(timespec start, timespec end);

int main(int argc, char** argv){
  timespec init, end;

  std::cout << "[INFO] Starting  Server on port 9999" << std::endl;
  int serverSocket = getSocket(9999);

  std::cout << "[INFO] Got socket number " << serverSocket << std::endl; 

  FCGX_Request request;
  FCGX_Init();
  FCGX_InitRequest(&request, serverSocket, 0);

  int status = FCGX_Accept_r(&request);
  std::cout << "[INFO] Accept status is " << status << std::endl;

  while(status == 0) {
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &init);
    
    std::cout << "[INFO] Have a request" << std::endl;

    std::stringstream ss;
    ss << "Centent-type: text/html\r\n\r\n";
    ss << "<html><body>Hi there<p>" << std::endl;
    
    char **env = request.envp;

    int contentSize = 0;
    while (*(++env)) {
      char cpy[strlen(*env)+1];
      strcpy(cpy, *env);

      char * key = strtok(cpy, "=");
      char * value = strtok(0, "");
      if (value == 0) value = "";

      const char * cl = "CONTENT_LENGTH";
      const char * empty = "";
      if (strcmp(key, cl ) == 0 && strcmp(value, empty) != 0) {
	std::cout << "[DEBUG] Content Length found" << std::endl;
	contentSize = atoi(value);
      }

      ss <<  key << " <----> " << value << "<br>" << std::endl;
    }

    char data[contentSize + 1];
    data[contentSize] = '\0';
    int n = FCGX_GetStr(data, contentSize, request.in);
    ss << "<p> " << contentSize  << "Read: " << n << " Data: " << data << "<p>";
    ss << "</body></html>" << std::endl;
    FCGX_PutS(ss.str().c_str(), request.out);
    FCGX_FFlush(request.out);
    FCGX_FClose(request.out);
    FCGX_Finish_r(&request);

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



