#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>

pthread_mutex_t wifi_mutex;
pthread_cond_t connection;
pthread_cond_t received;

char buffer_send[3];
char buffer_read[3];

int sockfd, portno = 51717, n;
char serverIp[] = "169.254.0.2";
struct sockaddr_in serv_addr;
struct hostent *server;
uint8_t running = 1;

void error(char *msg) {
    perror(msg);
    exit(0);
}

void *sendData( void *parameter ) {
  int n;

  pthread_mutex_lock(&wifi_mutex);
  pthread_cond_wait(&connection, &wifi_mutex);
  pthread_mutex_unlock(&wifi_mutex);
  
  while(running)
  {
    //sprintf( buffer, "%d\n", x );
    
    if(strlen(buffer_send) != 0)
    {
      pthread_mutex_lock(&wifi_mutex);
      n = write( sockfd, buffer_send, strlen(buffer_send) );
      pthread_mutex_unlock(&wifi_mutex);
   
      if ( n < 0 )
        error( (char*)( "ERROR writing to socket") );
    }
  }
  pthread_exit(NULL);
}

void *getData( void *parameter ) {
  
  int n;
  
  pthread_mutex_lock(&wifi_mutex);
  pthread_cond_wait(&connection, &wifi_mutex);
  pthread_mutex_unlock(&wifi_mutex);  

  while(running)
  {
    pthread_mutex_lock(&wifi_mutex);
    n = read(sockfd,buffer_read,2);
    pthread_cond_signal(&received);
    pthread_mutex_unlock(&wifi_mutex);

    if ( n < 0 )
         error( (char*)( "ERROR reading from socket") );
  }
  pthread_exit(NULL);
}

void *connectionThread( void *parameter )
{
    pthread_mutex_lock(&wifi_mutex);
 
    if ( ( sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 )
        error( (char*)( "ERROR opening socket") );

    if ( ( server = gethostbyname( serverIp ) ) == NULL ) 
        error( (char*)("ERROR, no such host\n") );
    
    bzero( (char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if ( connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error( (char *)( "ERROR connecting") );
    pthread_cond_broadcast(&connection);
    pthread_mutex_unlock(&wifi_mutex);
    pthread_exit(NULL);

}

void *control(void *parameter)
{
  pthread_mutex_lock(&wifi_mutex);
  pthread_cond_wait(&connection, &wifi_mutex);
  pthread_mutex_unlock(&wifi_mutex);  

  while(running)
  {
    pthread_mutex_lock(&wifi_mutex);
    pthread_cond_wait(&received, &wifi_mutex);
    printf ("%s\n", buffer_read);
    pthread_mutex_unlock(&wifi_mutex);
  }
  pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    
    
  pthread_t threads[4];
  pthread_attr_t attr;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&wifi_mutex, NULL);
  pthread_cond_init (&connection, NULL);
  pthread_cond_init (&received, NULL);
 
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  pthread_create(&threads[0], &attr, connectionThread, (void *)0);
  pthread_create(&threads[1], &attr, getData, (void *)1);
  pthread_create(&threads[2], &attr, sendData, (void *)2);
  pthread_create(&threads[3], &attr, control, (void *)3);

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  
  close( sockfd );
  
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&wifi_mutex);
  pthread_cond_destroy(&connection);
  pthread_cond_destroy(&received);
  pthread_exit (NULL);
    
    return 0;
}
