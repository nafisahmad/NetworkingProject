# Client Server Communication Using TCP/IP protocol

## PROJECT (Commom Communication Portal) 

1. ### **Introduction**


   >TCP/IP or “Transmission Control Protocol / Internet Protocol” is basically a network protocol that defines the details of how data is sent and received in a network.

   >This project allows a common server-client(s) communication portal using TCP IP Protocol. In layman's term , just like any social media application (say, Whatsapp), it allows a common group chat among all the clients. At the same time server also acts as a history tracker of all the communication happened along with the connection status of each client. 


2. ### **How to run**

   * Cloning The Repository using Terminal
     
     * Open Github:

       *  On GitHub, navigate to the main page of the repository.
      
       *  Above the list of files, click Code.
     
       * To clone the repository using HTTPS/SSH, copy the corresponding url.
     
     * Open Terminal:
      
       * Change the current working directory to the location where you want the     cloned directory.
       
       
       * git clone : 
       
         * ```c 
            git clone https://github.com/ujjwal4384/ACM-Network-Project 
        
       * Compile the Project:
          1. Method-1 compiling Makefile directly    
        
         * ```c
           cd common_communication_portal 
           make Makefile compile
            
           ```
       
         2. Method-2  (trivial)
        
         * ```c
           cd common_communication_portal 
           gcc server_tcp.c -o server
           gcc client_tcp.c -o client
            
           ```
        * Run the Project:  
          
            * Run Server 
              
              ```c
              ./server 4444
              ```
           * Run Client 1 (in another terminal)
             
              ```c
               ./client 4444      

               Please Enter Your Name : <Type your name>
              ```
           
           * Run Client 2 (in another terminal)
               ```c
               ./client 4444      
             
                Please Enter Your Name : <Type your name>
               ```


          Similarly, many clients can be connected with server and each other as 
          well and at the same time communication can be started. 
          
           Communication among server and 3 clients is shown in the gif below :

 3. ### **Tech Stack Used**
      
      * *OS NAME* -- *Ubuntu 20.04.1 LTS*

      * *Text Editor* -- *Visual Studio Code 1.53.2*

      * *Programming Language* -- *C 17*

      * *Protocol* -- ** CP/IP**

    

4. ### **Important Logic Used**
      

     1. >This particular project  is based on TCP/IP Protocol which utilizes       fundamental concepts of **Socket 
       Programming** as well as some advanced concepts like **Threading**. All of 
       which has been briefly explained below :-
       ![OSI Model vs TCP/IP Model](https://www.imperva.com/learn/wp-content/uploads/sites/13/2020/02/OSI-vs.-TCPIP-models.jpg.webp)    



    2. >As can be seen from the above image, TCP/IP Model is much simpler than OSI  Model in terms of layering. OSI Model uses 7 layers while TCP/IP Model uses just  4 layers by cobining various layers.


    3. >Each logic has been represented in the code using separate functions to make it more understandable.  
           
    4. >Sequential logical working of the program can be sub divided into two  categories :- 
      
        * ### Server-end Working:
          
          
          1. **Creation of socket** in the provided port number
               ```c
               listenfd = socket(AF_INET, SOCK_STREAM, 0);
               ```  
               
              *AF_INET* is an address family that is used to designate the type of   addresses that our socket can communicate with.

             *SOCK_STREAM* means it is a TCP socket.
      
          
          2. **Binding of socket**
             ```c
             bind(listenfd, (struct sockaddr*)&address_server, sizeof(address_server)
             ```
             *Bind()* function in socket programming is used to associate the socket with local address i.e. IP Address(Here, 127.0.0.1), port and address family.

          3. **Listening by server (waiting)**
              ```c
              listen(listenfd, 10)
             ```

             After successful creation and binding of socket, *listen()* comes into picture, where server waits for client's connection using listenfd which is a listening socket file descriptor.

          4. **Accepting the request**

              ```c
              connfd = accept(listenfd, (struct sockaddr*)&ip_client, &ip_client_size); 
              ```
              Server *accepts* the data packet from client and is all set for communication

          ## *New Features Added* :

          1. ```c
              signal(SIGPIPE, SIG_IGN);
              ```
              In order to ignore the PIPE Signals ( SIGPIPE ), *SIG_IGN*  handler is passed.
          
          2. ```c
              #define CLIENTS_MAX_CAPACITY 90
              #define BUFFER_MAX_CAPACITY 4096
              #define MAX_NAME_LENGTH 50
             ```   
          
          Number of clients which can enter the communication portal,
          Maximum buffer capacity and Client's name length - all are controlled by macro defined globally whose values can be controlled by just changing the values of MACRO. 

          By doing so, it makes the code more compatible and simpler to handle.

         3. ```c
              static int client_id = 10;
             ```

            Each client has been differentiated by its unique client id.

            This client id helps in two ways :-
             
             * It helps in keeping the track on which client has joined or left the meeting
             
             * Client id facilitates in distinguishing client's with same name.
             

         4. ```c
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
             ```
           
             The logic used for adding new client to the queue (on connection request)
             is implemented using the for loop and only  if following conditions are met :-
               * CLIENTS MAXIMUM CAPACITY is not reached

               * That CLIENT already not exist in the meeting
               



         5.  ```c
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

             ```
           
             Similar logic is used for removing new client from the queue (on connection request).This loop keeps track of which client left the meeting.
             is implemented using the for loop and only  if following conditions are met :-
               * CLIENTS MAXIMUM CAPACITY is not reached

               * That CLIENT was present in the meeting
               

        5.  ```c
               void sending_tweet(char *s, int client_id)
            ```
           
           
               Sending of message to all the clients( and server) except the sender 
               itself is done through the help of client_id which tells the sender'id.

             Similar logic is applied for receiving the messages. 
  
 
        5.  ```c
                void display_user_ip(struct sockaddr_in addr){
                   printf("%d.%d.%d.%d",
                  addr.sin_addr.s_addr & 0xff,
                  (addr.sin_addr.s_addr & 0xff00) >> 8,
                  (addr.sin_addr.s_addr & 0xff0000) >> 16,
                  (addr.sin_addr.s_addr & 0xff000000) >> 24);
               }

              ```
             Client's address is also displayed by the server usingsin_addr family and some mathematical manipulation of shifting of data. 

              
5. ### **Results Obtained**    
      
      Result's output has been shown as a gif above as well as in form of images in steps below :-



6. ### **Future goals of the project:**

   * As the 4-week-long  weekly lectures (by IIT-R ACM Student Chapter) were passing, my interest and knowledge in the domain of networking system also gradually increased. To be honest, before that , I had negligible knowledge in that domain.
   
   * But once I was introduced with the world of networking and client-server communication model, my interest in it started increasing exponentially.
   The concepts developed during the 4-week-long discussion period, made me confident in bringing up something new from it. This project is the product of all the knowlege I gained through discussions and references study maerial.

 
   * My future goals associated with the project includes :-
       
        * Making it more versatile for various purposes:-
              
            * Since in the project I have used threads, I would be incorporating *fork* feature also with it. I can be used in handling multiple clients in following way :-
            
            *  Just like any social media application( like whatsapp), where in a group chat there is an option of privately messaging the particular client at any point of time, in the same fashion I would be integrating this feature as well. It can be made possible using *fork* 

        * Since, in my current project , there is a server which basically tracks all the communication happening among the clients, so adding more features to server(basically giving *admin powers*) would make the project more powerful. Following are the features which I am currently looking forward to work on :-
            
            * Removing particular client with/without its concern
            
            * Saving the data tracked in a encrypted manner in a text file. Hence,if in future there comes the need to produce the data of what happened durin the past meetings, then it could be displayed by the server(admin).


        * In current project, each client enters the meeting through his/her name which is then assigned a unique client id. So, in order to increase the security of the system, proper *authentication* can be set-up wherein client is required to enter his/her Username as well as *password*   
  
       * Current project takes care of the sharing/transfer of messages/tweets and  history tracker. I would like to ensure sharing of files also. Like there can be transfer of files among clients which includes modification, concatenation of file data.

     * Above mentioned points are just the things which I currently feel would make it more versatile. I am sure there is much more scope on what extra can be integrated and I will continue working on them as well. 
      




















