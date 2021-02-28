# Client Server Communication Using TCP/IP protocol

## Table of contents :
 * [Client Server Communication Using TCP/IP protocol](#client-server-communication-using-tcpip-protocol)
       
      * [<strong>Introduction</strong>](#introduction)
     
      * [<strong>How to run</strong>](#how-to-run)
      
      * [<strong>Tech Stack Used</strong>](#tech-stack-used)
     
      * [<strong>Important Logic Used</strong>](#important-logic-used)
     
      * [Server-end Working:](#server-end-working)
     
        * [<em>New Features Added in server-side</em> :](#new-features-added-in-server-side-)
     
      * [Client-end Working:](#client-end-working)
     
        * [<em>New Features Added in Client-side</em> :](#new-features-added-in-client-side-)
     
      * [<strong>Results &amp; Outputs Obtained</strong>](#results--outputs-obtained)
     
         * [gif](#gif)
     
         * [Images:-](#images-)
     
      * [<strong>Future goals of the project:</strong>](#future-goals-of-the-project)

## PROJECT (Commom Communication Portal) 



1. ### **Introduction**


   >TCP/IP or “Transmission Control Protocol / Internet Protocol” is basically a network protocol that defines the details of how data is sent and received in a network.

   >This project creates a common server-client(s) communication portal using TCP/IP Protocol. In layman's term , just like any social media application (say, Whatsapp), it allows a common group chat among all the clients. At the same time server also acts as a history tracker of all the communication happened along with the connection status of each client. 


2. ### **How to run**
     <a name="cloning"></a>
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
           
           gcc -Wall -g3 -fsanitize=address -pthread server_tcp.c -o server
	         
           gcc -Wall -g3 -fsanitize=address -pthread client_tcp.c -o client
            
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
          
           Communication among server and 3 clients is shown in the gif below in Results section :

 3. ### **Tech Stack Used**
      
      * *OS NAME* -- *Ubuntu 20.04.1 LTS*

      * *Text Editor* -- *Visual Studio Code 1.53.2*

      * *Programming Language* -- *C 17*

      * *Protocol* -- *TCP/IP*
      
    

4. ### **Important Logic Used**
      

     1. >This particular project  is based on TCP/IP Protocol which utilizes       fundamental concepts of **Socket 
       Programming** as well as some advanced concepts like **Threading**. All of 
       which has been briefly explained below :-
       ![OSI Model vs TCP/IP Model](https://www.imperva.com/learn/wp-content/uploads/sites/13/2020/02/OSI-vs.-TCPIP-models.jpg.webp)    



    2. >As can be seen from the above image, TCP/IP Model is much simpler than OSI  Model in terms of layering. OSI Model uses 7 layers while TCP/IP Model uses just  4 layers by combining various layers.


    3. >Each logic has been represented in the code using separate functions to make it more understandable.  
           
    4. >Sequential logical working of the program can be sub divided into two  categories :- 
      
        * ### Server-end Working:
          
          
          1. **Creation of socket** 
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

             After successful creation and binding of socket, *listen()* comes into picture, where server waits for client's connection using listenfd which is a listening socket descriptor.

          4. **Accepting the request**

              ```c
              connfd = accept(listenfd, (struct sockaddr*)&ip_client, &ip_client_size); 
              ```
              Server *accepts* the data packet from client and is all set for communication

          ## *New Features Added in server-side* :

          1. **Signal() function**
             ```c
              signal(SIGPIPE, SIG_IGN);
              ```
              In order to ignore the PIPE Signals ( SIGPIPE ), *SIG_IGN*  handler is passed.
          
          2. **MACRO**
             ```c
              #define CLIENTS_MAX_CAPACITY 90
              #define BUFFER_MAX_CAPACITY 4096
              #define MAX_NAME_LENGTH 50
             ```   
          
          Number of clients which can enter the communication portal,
          Maximum buffer capacity and Client's name length - all are controlled by macro defined globally whose values can be changed by just changing the values of MACRO. 

          By doing so, it makes the code more compatible and simpler to handle.

         3. **Using Client_id**
            ```c
              static int client_id = 10;
             ```

            Each client has been differentiated by its unique client id.

            This client id helps in two ways :-
             
             * It helps in keeping the track on which client has joined or left the meeting
             
             * Client id facilitates in distinguishing clients with same name.
             

         4. **Addition of new client**
         
            ```c
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
             is implemented using the for loop and client is added only  if following conditions are met :-
               * CLIENTS MAXIMUM CAPACITY is not reached

               * That CLIENT already not exist in the meeting
               



         5. **Removal of client** 
            ```c
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
             It is implemented using the for loop and client is added only  if following conditions are met :-
               * CLIENTS MAXIMUM CAPACITY is not reached

               * That CLIENT was present in the meeting
               

        5.  **Handling messages**
            ```c
               void sending_tweet(char *s, int client_id)
            ```
           
           
               Sending of message to all the clients( and server) except the sender 
               itself is done through the help of client_id which provides the sender's id.

             Similar logic is applied for receiving the messages. 
  
 
        5. **Client's Address** 
           ```c
                void display_user_ip(struct sockaddr_in addr){
                   printf("%d.%d.%d.%d",
                  addr.sin_addr.s_addr & 0xff,
                  (addr.sin_addr.s_addr & 0xff00) >> 8,
                  (addr.sin_addr.s_addr & 0xff0000) >> 16,
                  (addr.sin_addr.s_addr & 0xff000000) >> 24);
               }

              ```
             Client's address is also displayed by the server usingsin_addr family and  mathematical implementation of shifting of data. 
        
       9.  **Threading**
           ```c
            pthread_create(&tid, NULL, &handle_client, (void*)client);
              ```
             the pthread_create() function starts a new thread in the callng process. It starts by envoking start_routine();arg is passed as the only argument of start_routine() 
              
       10. ```c
             pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

           ```      
           In *multi-threading* Variables of type pthread_mutex_t is initialized statically, using the constants PTHREAD_MUTEX_INITIALIZER (for fast mutexes).

        10. ```c
             pthread_mutex_lock(&clients_mutex); pthread_mutex_unlock(&clients_mutex);
              ```      
              pthread_mutex_lock locks the given mutex. If the mutex is currently unlocked, it becomes locked and owned by the calling thread, and pthread_mutex_lock returns immediately. If the mutex is already locked by another thread, pthread_mutex_lock suspends the calling thread until the mutex is unlocked.    


   

   * ### Client-end Working:
          
          
       1. **Creation of socket** 
               ```c
               sockfd = socket(AF_INET, SOCK_STREAM, 0);
               ```  
               
           *AF_INET* is an address family that is used to designate the type of   addresses that our socket can communicate with.

             *SOCK_STREAM* means it is a TCP socket.
      
          
       2. **Connecting to servert**
             ```c
             connect(sockfd, (struct sockaddr *)&address_server, sizeof(address_server))
             ```
             *connect()* function in socket programming is used by client to set-up connection establishment with the server.

     ## *New Features Added in Client-side* :

        1.  **Sending Client's Name To Server**
          
             ```c
              send(sockfd, client_name, MAX_NAME_LENGTH, 0);
            ```
            
            Client shares it's name with the server and gets connected to the server and other clients.
          
        2.  **Sending & Receiving Threads**
              ```c
                pthread_t send_msg_thread;
                 if(pthread_create(&send_msg_thread, NULL, (void *) tweet_sending_handler, NULL) != 0){
                  printf("ERROR: pthread\n");
                  return EXIT_FAILURE;
                  }
              ```  
         
            Two threads are used : one for sending the message and other for receiving the message. 'Sending-thread' logic has been shown above.


          
            Other logics are same as that mentioned in server side working. 

        

5. ### **Results & Outputs Obtained**    
      Result and output for 3-clients 1-server communication system has been depicted below.
      It has been shown both as a gif as well as in form of images :-
      
      Port number taken in gif is 4444, and port number taken in images is 9999.
     
    ### gif

      ![Project_Demonstration](https://user-images.githubusercontent.com/73752812/109420952-39e51a00-79fb-11eb-9d91-825f90a98c2f.gif)

    ### Images:-

       STEP-1

     ![step-1](https://user-images.githubusercontent.com/73752812/109421313-c5ab7600-79fc-11eb-87e3-cfa2b0819bb8.png)

        STEP-2
      ![step-2](https://user-images.githubusercontent.com/73752812/109421315-c6dca300-79fc-11eb-9206-1a893dbac660.png)

        STEP-3
      ![step-3](https://user-images.githubusercontent.com/73752812/109421317-c80dd000-79fc-11eb-800a-4c49418f7cf7.png)

        STEP-4
      ![step-4](https://user-images.githubusercontent.com/73752812/109421320-c93efd00-79fc-11eb-9319-988e83ed9a35.png)

        STEP-5
      ![step-5](https://user-images.githubusercontent.com/73752812/109421321-ca702a00-79fc-11eb-9e04-b04bcb14195d.png)

        STEP-6
      ![step-6](https://user-images.githubusercontent.com/73752812/109421324-cc39ed80-79fc-11eb-9373-9caecf6477cc.png)

        STEP-7
      ![step-7](https://user-images.githubusercontent.com/73752812/109421328-cd6b1a80-79fc-11eb-9b92-0354b675ca8f.png)

        STEP-8
      ![step-8](https://user-images.githubusercontent.com/73752812/109421329-cf34de00-79fc-11eb-87ea-1633559e0705.png)

        STEP-9
      ![step-9](https://user-images.githubusercontent.com/73752812/109421331-d0660b00-79fc-11eb-8b40-a208b0597ac5.png)

        STEP-10
      ![step-10](https://user-images.githubusercontent.com/73752812/109421333-d1973800-79fc-11eb-9359-4de00ff0615f.png)

        STEP-11
      ![step-11](https://user-images.githubusercontent.com/73752812/109421335-d2c86500-79fc-11eb-8b44-96084de9163f.png)


6. ### **Future goals of the project:**

   * As the 4-week-long  weekly lectures conducted by 'IIT-R ACM Student Chapter' passed, my interest and knowledge in the domain of networking system also gradually increased. To be honest, before that , I had negligible knowledge in that domain.
   
   * But once I was introduced with the world of networking and client-server communication model, my interest in it started increasing exponentially.
   The concepts developed during the 4-week-long discussion period, made me confident in bringing up something new from it. This project is the product of all the knowlege I gained through discussions and references study maerial.

 
   * My future goals associated with the project includes :-
       
        1. Making it more versatile for various purposes:-
              
            * Since in the project I have used threads, I would be incorporating *fork* feature also with it in more productive manner. It can be used in handling multiple clients in following way :-
            
            *  Just like any social media application( like whatsapp), where in a group chat there is an option of privately messaging the particular client at any point of time, in the same fashion I would be integrating this feature as well. It can be made possible using *fork* 

        2.  Since, in my current project , there is a server which basically tracks all the communication happening among the clients, so adding more features to server(basically giving *admin powers*) would make the project more powerful. Following are the features which I am currently looking forward to work on :-
            
            * Removing particular client with/without its concern
            
            * Saving the communication-data in an encrypted manner in a text file. Hence,if in future there comes the need to produce the data of what happened during the past meetings, then it could be displayed by the server(admin).


        3.   In current project, each client enters the meeting through his/her name which is then assigned a unique client id. So, in order to increase the security of the system, proper *authentication* portal can be set-up where-in client is required to enter his/her username as well as *password*   
  
       4.  Current project takes care of the sharing/transfer of messages/tweets and  history tracker. Last but not the least , I would like to ensure sharing of files also. Like there can be transfer of files among clients which includes modification, concatenation of file data.
     




















