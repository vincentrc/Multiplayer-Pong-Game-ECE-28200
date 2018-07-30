1:
	gcc -lcurses -o serv1 server.c serverfunctions.c 
	gcc -lcurses -o client client.c clientfunctions.c 

run1:
	./serv1 `hostname` 1112 &

run2:
	./client `hostname` 1112

clean:
	rm -rf serv1
	rm -rf client
	pkill serv1

