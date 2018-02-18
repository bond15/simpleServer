#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


void processRequest( int socket );

int main( int argc, char ** argv )
{
  
  int port = 8000;
  
  // Set the IP address and port for this server
  struct sockaddr_in serverIPAddress; 
  memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
  serverIPAddress.sin_family = AF_INET;
  serverIPAddress.sin_addr.s_addr = INADDR_ANY;
  serverIPAddress.sin_port = htons((u_short) port);
  
  // Allocate a socket
  int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
  if ( masterSocket < 0) {
    perror("socket");
    exit( -1 );
  }

  // Set socket options to reuse port. Otherwise we will
  int optval = 1; 
  int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
		       (char *) &optval, sizeof( int ) );
   
  // Bind the socket to the IP address and port
  int error = bind( masterSocket,
		    (struct sockaddr *)&serverIPAddress,
		    sizeof(serverIPAddress) );
  if ( error ) {
    perror("bind");
    exit( -1 );
  }
  
  // Put socket in listening mode and set the 
  // size of the queue of unprocessed connections
  error = listen( masterSocket, 5);
  if ( error ) {
    perror("listen");
    exit( -1 );
  }

  while ( 1 ) {

    // Accept incoming connections
    struct sockaddr_in clientIPAddress;
    int alen = sizeof( clientIPAddress );
    int slaveSocket = accept( masterSocket,
			      (struct sockaddr *)&clientIPAddress,
			      (socklen_t*)&alen);

    if ( slaveSocket < 0 ) {
      perror( "accept" );
      exit( -1 );
    }

    // Process request.
    processRequest( slaveSocket );

    // Close socket
    close( slaveSocket );
  }
  
}

void processRequest( int fd )
{
  //char  * data = "hello you piece of shit";
  	const char * html = 
		"HTTP/1.0 200 documentfollows \r\n"
		"Server: hello\r\n"
		"Content-type: text/html\r\n\r\n";
	const char * data = 
		"<html> <h1> Hello World"
		"</h1> </html>";
	
	
	char * response = (char * ) malloc(strlen(html) + strlen(data) + 1);
	strcpy(response, html);
	strcat(response, data);
	write(fd, response, strlen(response));
}
