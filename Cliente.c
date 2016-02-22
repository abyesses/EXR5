//
//  Cliente.c
//  EXR5
//
//  Created by Abraham Esses on 2/22/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TCP_PORT 8002
double * leerViento();
int * num;
int main(int argc, const char * argv[])
{
    struct sockaddr_in direccion;
    double *wind = (double *)malloc(sizeof(double)*100);
    
    int cliente;
    
    ssize_t writing;
    
    if (argc != 2) {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }
    
    // Crear el socket
    cliente = socket(AF_UNIX, SOCK_STREAM, 0);
    
    // Establecer conexión
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_UNIX;
    
    writing = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    
    if (writing == 0) {
        printf("Conectado a %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
        
        // Escribir datos en el socket
        while (1) { //(leidos = read(fileno(stdin), &buffer, sizeof(buffer)))
            wind = leerViento();
            printf("Enviando lecturas... %f\n",*wind);
            write(cliente, wind,sizeof(wind));
            printf("Enviado... \n");
            sleep(10);
        }
    }
    else
        printf("El servidor no está disponible\n");
    
    // Cerrar sockets
    close(cliente);
    free(wind); 
    return 0;
}
double * leerViento(){
    //return rand()%100;
    double * arrayViento = (double *)malloc(sizeof(double)*3000);
    int i = 0;
    time_t c = time(0);
    time_t d;
    int randTime = rand()%15;
    while (d-c < randTime && i < 100) {
        *(arrayViento+i) = rand()%10;
         d = time(0);
        i++;
        usleep(50);
    }
    //num = &i;
    free(arrayViento);
    return arrayViento;
}
