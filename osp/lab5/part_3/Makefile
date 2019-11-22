server: main.c server.c server.h
	gcc -o server -std=gnu99 -Wall -Werror main.c server.c server.h

client: client.c
	gcc -o client -std=gnu99 -Wall -Werror client.c

clear:
	@rm client server
