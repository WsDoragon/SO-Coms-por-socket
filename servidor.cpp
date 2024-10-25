#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <thread>

#include "dotenv.h"

using namespace std;

// Funcion de menu con sus respuestas
string menu(int opcion) {
    switch (opcion) {
        case 1:
            return "Opción 1: Ver información del servidor";
        case 2:
            return "Opción 2: Ver estadísticas de conexión";
        case 3:
            return "Opción 3: Salir";
        default:
            return "Opción no válida - Intente de nuevo (1 - 3)";
    }
}

// Funcion para manejar cada cliente entrante
void handleClient(int clientSocket) {
    char buffer[1024];
    ssize_t bytesRead;
    string respuesta;
    bool continua = true;
    // Bucle para manejar las opciones del cliente
    while (continua) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        cout << buffer << endl;
        respuesta = menu(atoi(buffer));
        send(clientSocket, respuesta.c_str(), respuesta.length(), 0);
        if (atoi(buffer) == 3) {
            continua = false;
        }
    }
    cout << "Cliente desconectado " << endl;
    close(clientSocket); // Cerrar el socket del cliente al recibir la opción de salida
}

int main(){
    // Cargar el archivo .env
    dotenv::init();
    const char* PORT_ENV = getenv("PORT");
    if (PORT_ENV == nullptr) {
        std::cerr << "Error: No se encontró la variable de entorno PORT" << std::endl;
        return -1;
    }
    int PORT = std::stoi(PORT_ENV);
    
    // Crear socket de servidor
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); // Puerto del servidor
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Enlazamos socket a dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error al enlazar el socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == -1) {
        perror("Error al escuchar");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    cout << "Esperando conexiones entrantes..." << endl;

    while (true) {
        // Aceptar conexion entrante
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Error al aceptar la conexión");
            continue; // Continuar esperando conexiones
        }

        cout << "Cliente conectado" << endl;
        // Crear un nuevo hilo para manejar la conexión del cliente (solucion a desconexiones automaticas y caidas)
        thread(handleClient, clientSocket).detach(); 
        
    }

    return 0;
}