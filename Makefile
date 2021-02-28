compile:
	gcc -Wall -g3 -fsanitize=address -pthread server_tcp.c -o server
	gcc -Wall -g3 -fsanitize=address -pthread client_tcp.c -o client
