#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "dotenv.h"

using namespace std;

// Función para conectar al servidor
int connectServer(const string& serverIP, int serverPort) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }
    // Configurar la dirección del servidor
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    // Conectar al servidor y verificar errores
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error al conectar al servidor");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    cout << "Conectado al servidor." << endl;
    // Retornar el socket del cliente al servidor
    return clientSocket;
}

// Función para enviar un mensaje al servidor
void sendMessage(int clientSocket, const string& message) {
    cout << "Enviando mensaje al servidor..." << endl;
    send(clientSocket, message.c_str(), message.length(), 0);
}

// Función para recibir y mostrar un mensaje del servidor
void receiveMessage(int clientSocket) {
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        perror("Error al recibir datos del servidor");
        close(clientSocket);
        exit(EXIT_FAILURE);
    } else if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        cout << "Respuesta del servidor:\n" << buffer << endl;
    }
}

int main() {
    // Cargar el archivo .env
    dotenv::init();
    // Cargar variables de .env
    const char* PORT_ENV = getenv("PORT");
    const char* IP_SERVER_ENV = getenv("IP_SERVER");

    if (PORT_ENV == nullptr) {
        std::cerr << "Error: No se encontró la variable de entorno PORT" << std::endl;
        return -1;
    }
    if (IP_SERVER_ENV == nullptr) {
        std::cerr << "Error: No se encontró la variable de entorno IP_SERVER" << std::endl;
        return -1;
    }

    int PORT =  std::stoi(PORT_ENV);
    const char* IP_SERVER = IP_SERVER_ENV;

    int clientSocket = connectServer(IP_SERVER, PORT); // Conectar al servidor
    bool continua = true;
    while(continua){
        // Ingreso de numero de opcion
        int message;
        cout << "Ingrese un mensaje: ";
        cin >> message; //Opciones del 1 al 3

        sendMessage(clientSocket, to_string(message));
        if (message == 3){ //Opcion 3 para salir
            continua = false;
        }
        receiveMessage(clientSocket);
    }
    cout << "Cerrando la conexión." << endl;
    close(clientSocket);
    return 0;
}