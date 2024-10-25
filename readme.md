## Descripcion
Este proyecto es un ejemplo de ejecucion por socket y servidor con threads en C++


## Requisitos
* Libreria dotenv (Incluida en la carpeta)

## Compilacion
Para compilar el proyecto se debe ejecutar el siguiente comando:
```bash
make all
```

## Ejecucion
Para ejecutar el proyecto se debe ejecutar el siguiente comando:
```bash 
./server # Para el servidor
./client # Para el cliente
```

## Variables de entorno
* Servidor
  * SERVER_PORT: Puerto del servidor

* Cliente
    * SERVER_IP: IP del servidor
    * SERVER_PORT: Puerto del servidor

## Servidor
El servidor se encarga de recibir los mensajes de los clientes y responder con un mensaje de confirmacion a las ordenes numericas del 1 al 3.

## Cliente
El cliente se encarga de enviar mensajes al servidor y recibir la respuesta del servidor, utilizando ordenes numericas del 1 al 3.
