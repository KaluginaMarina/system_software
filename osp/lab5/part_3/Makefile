server: main.c server.c server.h
	gcc -o server -std=gnu99 -Wall main.c server.c server.h

client: main_client.c client.c client.h server.h 
	gcc -o client -std=gnu99 -Wall -Werror main_client.c client.c client.h server.h

clear:
	@rm client server
