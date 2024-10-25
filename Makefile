all:
	make clean
	g++ -o servidor servidor.cpp
	g++ -o cliente cliente.cpp 
clean:
	rm -f servidor cliente
