build:
	gcc -o server server.cc
	gcc -o client client.cc
clean:
	rm server
	rm client
