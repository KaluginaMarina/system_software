server: main.c server.c server.h
	gcc -o server -std=gnu99 -Wall -Werror main.c server.c server.h

client: client.c server.h 
	gcc -o client -std=gnu99 -Wall -Werror client.c server.h

clear:
	@rm client server
