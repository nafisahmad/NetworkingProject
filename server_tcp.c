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
#define BUFFER_MAX_CAPACITY 4096
#define MAX_NAME_LENGTH 50
static _Atomic unsigned int Num_Of_Clients = 0;
static int client_id = 10;

/* Client structure */
typedef struct{
	struct sockaddr_in address;
	int client_id;
	char name[MAX_NAME_LENGTH];
    int sockfd;
} user_type;

user_type *clients[CLIENTS_MAX_CAPACITY];

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

/* Send message to all clients except sender */
void sending_tweet(char *s, int client_id){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i<CLIENTS_MAX_CAPACITY; ++i){
		if(clients[i]){
			if(clients[i]->client_id != client_id){
				if(write(clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}


void trimmingString (char* str, int count) {
  
  for (int j = 0; j < count; j++) { // trim \n
    if (str[j] == '\n') {
      str[j] = '\0';
      break;
    }
  }
}


/* Add clients to queue */
void new_client_connect(user_type *clien){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < CLIENTS_MAX_CAPACITY; ++i){
		if(!clients[i]){
			clients[i] = clien;
			break;
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}

void display_user_ip(struct sockaddr_in addr){
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr & 0xff00) >> 8,
        (addr.sin_addr.s_addr & 0xff0000) >> 16,
        (addr.sin_addr.s_addr & 0xff000000) >> 24);
}
/* Remove clients to queue */

void existing_client_disconnect(int client_id){
	pthread_mutex_lock(&clients_mutex);

	for(int i=0; i < CLIENTS_MAX_CAPACITY; ++i){
		if(clients[i]){
			if(clients[i]->client_id == client_id){
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clients_mutex);
}


/* Handle all communication with the client */
void *handle_client(void *arg){
	char buffer[BUFFER_MAX_CAPACITY];
	char name[MAX_NAME_LENGTH];
	int disconnected_flag = 0;

	Num_Of_Clients++;
	user_type *client = (user_type *)arg;

	// Name
	if(recv(client->sockfd, name, MAX_NAME_LENGTH, 0) <= 0 || strlen(name) <  2 || strlen(name) >= MAX_NAME_LENGTH-1){
		printf("Please enter the valid name  .\n");
		disconnected_flag = 1;
	} else{
		strcpy(client->name, name);
		sprintf(buffer, "%s has joined\n", client->name);
		printf("%s", buffer);
		sending_tweet(buffer, client->client_id);
	}

	bzero(buffer, BUFFER_MAX_CAPACITY);

	while(1){
		if (disconnected_flag) {
			break;
		}

		int tweet_receive = recv(client->sockfd, buffer, BUFFER_MAX_CAPACITY, 0);
		if (tweet_receive > 0){
			if(strlen(buffer) > 0){
				sending_tweet(buffer, client->client_id);

				trimmingString(buffer, strlen(buffer));
				printf("%s \n", buffer);
			}
		} else if (tweet_receive == 0 || strcmp(buffer, "exit") == 0){
			sprintf(buffer, "%s has left\n", client->name);
			printf("%s", buffer);
			sending_tweet(buffer, client->client_id);
			disconnected_flag = 1;
		} else {
			printf("ERROR: -1\n");
			disconnected_flag = 1;
		}

		bzero(buffer, BUFFER_MAX_CAPACITY);
	}

  /* Delete client from queue and yield thread */
	close(client->sockfd);
  existing_client_disconnect(client->client_id);
  free(client);
  Num_Of_Clients--;
  pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	int way = 1;
	int listenfd = 0, connfd = 0;    //listening file descriptor    //connection file descriptor
  struct sockaddr_in address_server;
  struct sockaddr_in ip_client;
  pthread_t tid;

  /* Socket settings */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  address_server.sin_family = AF_INET;
  address_server.sin_addr.s_addr = inet_addr(ip);
  address_server.sin_port = htons(port);

  /* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);

	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&way,sizeof(way)) < 0){
		perror("ERROR: setsockopt failed");
    return EXIT_FAILURE;
	}

	/* Bind */
  if(bind(listenfd, (struct sockaddr*)&address_server, sizeof(address_server)) < 0) {
    perror("ERROR: Socket binding failed");
    return EXIT_FAILURE;
  }

  /* Listen */
  if (listen(listenfd, 10) < 0) {
    perror("ERROR: Socket listening failed");
    return EXIT_FAILURE;
	}

	printf("*** GREETINGS TO OUR COMMON COMMUNICATION-PORTAL ***\n");

	while(1){
		socklen_t ip_client_size = sizeof(ip_client);
		//accept
		connfd = accept(listenfd, (struct sockaddr*)&ip_client, &ip_client_size);

		/* Check if max clients is reached */
		if((Num_Of_Clients + 1) == CLIENTS_MAX_CAPACITY){
			printf("ERROR: Clients Maximum Capacity Reached. Rejected: ");
			display_user_ip(ip_client);
			printf(":%d\n", ip_client.sin_port);
			close(connfd);
			continue;
		}

		/* Client-side settings */
		user_type *client = (user_type *)malloc(sizeof(user_type));
		client->address = ip_client;
		client->sockfd = connfd;
		client->client_id = client_id++;

		/* Adding client to the queue and fork thread */
		new_client_connect(client);
		pthread_create(&tid, NULL, &handle_client, (void*)client);

		/* Reduces CPU usage */
		sleep(1);
	}

	return EXIT_SUCCESS;
}
