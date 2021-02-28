#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <signal.h>
#include <arpa/inet.h>
#include <pthread.h>

#define CLIENTS_MAX_CAPACITY 90     
#define BUFFER_MAX_CAPACITY  4096
#define MAX_NAME_LENGTH 50


char client_name[MAX_NAME_LENGTH];
volatile sig_atomic_t flag = 0;
int sockfd = 0;       //socket descriptor

void str_overwrite_stdout() {
  printf("%s", "> ");
  fflush(stdout);
}

//trims/ truncates the required string accordingly
void trimmingString (char* str, int count) {
  
  for (int j = 0; j < count; j++) { // trim \n
    if (str[j] == '\n') {
      str[j] = '\0';
      break;
    }
  }
}
//setting flag
void catch_ctrl_c_and_exit() {
    flag = 1;
}

// sending -message/tweet handler function made separately to avoid confusion
void tweet_sending_handler() {
  char buffer[BUFFER_MAX_CAPACITY ] = {};
	char tweet[BUFFER_MAX_CAPACITY  + MAX_NAME_LENGTH+10] = {};  // 10 is added to ensure in line 51(sprintf) has enough storage capacity for tweet


 //infinite while loop with conditional break                                                                  
  while(1) {
  	str_overwrite_stdout();
    fgets(buffer, BUFFER_MAX_CAPACITY , stdin);
    trimmingString (buffer, BUFFER_MAX_CAPACITY );

    if (strcmp(buffer, "exit") == 0) {
			break;
    } else {
      sprintf(tweet, "%s: %s\n", client_name,buffer);
      send(sockfd, tweet, strlen(tweet), 0);
    }

		bzero(buffer, BUFFER_MAX_CAPACITY );
    bzero(tweet, BUFFER_MAX_CAPACITY  + MAX_NAME_LENGTH);
  }
  catch_ctrl_c_and_exit(2);
}

// similarly, receiving- message/tweet handler function made separately to avoid confusion
void tweet_receiving_handler() {
	char tweet[BUFFER_MAX_CAPACITY ] = {};
  while (1) {
		int receive = recv(sockfd, tweet, BUFFER_MAX_CAPACITY , 0);
    if (receive > 0) {
      printf("%s", tweet);
      str_overwrite_stdout();
    } else if (receive == 0) {
			break;
    } 
		bzero(tweet, BUFFER_MAX_CAPACITY );  //erases the data in n bytes of memory by placing '\0' there 
  }
}


// main function begins  two parametes required : (1) binary name     (2) port 
int main(int argc, char **argv){    
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);

	signal(SIGINT, catch_ctrl_c_and_exit);

	printf("Please enter your name: ");
  fgets(client_name, MAX_NAME_LENGTH, stdin);
  trimmingString (client_name, strlen(client_name));


	if (strlen(client_name) > MAX_NAME_LENGTH -1|| strlen(client_name) < 2){
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

	struct sockaddr_in address_server;

	/*Client Side Socket settings  */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  address_server.sin_family = AF_INET;
  address_server.sin_addr.s_addr = inet_addr(ip);
  address_server.sin_port = htons(port);


  //Client  Connecting to Server
  int err = connect(sockfd, (struct sockaddr *)&address_server, sizeof(address_server));
  if (err == -1) {
		printf("ERROR: connection failed\n");
		return EXIT_FAILURE;
	}

	// Sending the  client's name
	send(sockfd, client_name, MAX_NAME_LENGTH, 0);

	printf("*** GREETINGS TO OUR COMMON COMMUNICATION-PORTAL ***\n");
 
 //creating two threads : (1)for sending    (2)for receiving messages/tweets
 
 //sending mssg thread
	pthread_t send_msg_thread;
  if(pthread_create(&send_msg_thread, NULL, (void *) tweet_sending_handler, NULL) != 0){
		printf("ERROR: pthread\n");
    return EXIT_FAILURE;
	}
//receiving mssg thread
	pthread_t recv_msg_thread;
  if(pthread_create(&recv_msg_thread, NULL, (void *) tweet_receiving_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

    //infinite while loop with conditional break on disconnection
	while (1){
		if(flag){
			printf("\n Bye and have a great day\n");
			break;
    }
	}
//closing it

	close(sockfd);

	return EXIT_SUCCESS;
}
