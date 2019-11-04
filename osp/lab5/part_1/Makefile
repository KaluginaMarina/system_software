server: main.c server.c server.h
	gcc -o server -std=gnu99 -Wall -Werror main.c server.c server.h

client: client_main.c client.c client.h server.h 
	gcc -o client -std=gnu99 -Wall -Werror client_main.c client.c client.h server.h

clear:
	@rm client server
