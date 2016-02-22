//
//  main.c
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
#define TCP_PORT 8002
void imprimeMinimo(int *);
int main(int argc, const char * argv[]) {
    
    //BLAB Bind,Listen,Accept,Begin to talk

    
    struct sockaddr_in direccion;
    double * windSrv = (double *)malloc(sizeof(double)*100);
    int servidor, cliente;
    ssize_t leidos, escritos;
    int continuar = 1;
    pid_t pid;

    if (argc != 2) {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }
    
    // Crear el socket
    servidor = socket(AF_UNIX, SOCK_STREAM, 0);
    
    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_UNIX;
    
    int a = bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    if (a !=0) {
        printf("El puerto %d no esta disponible\n",TCP_PORT);
        exit(-1);
    }
    
    // Escuhar
    
    listen(servidor, 10);
    
    escritos = sizeof(direccion);
    
    while (continuar)
    {
        cliente = accept(servidor, (struct sockaddr *) &direccion, &escritos);
        
        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
    
        pid = fork();
        if (pid == 0) continuar = 0;//Para evitar que los hijos creen mas conexiones.
    }
    
    if (pid == 0) {
        
        close(servidor);
        
        if (cliente >= 0) {
            double minimo = 1000;
            double maximo = 0;
            while ((leidos = read(cliente, windSrv, (sizeof(double)*100)))) {
                //printf("Leyendo...%f",*windSrv);
                for (int j = 0; j < 100 ; j++) {
                    if (minimo > *(windSrv+j))
                        minimo = *(windSrv+j);
                    if (maximo < *(windSrv+j)) {
                        maximo = *(windSrv+j);
                    }
                    if( *(windSrv+j) == 0) break;

                }
                printf("El mínimo de la tanda es %f\n",minimo);
                printf("El maximo de la tanda es %f\n",maximo);
                printf("El promedio de la tanda es %f\n",(maximo-minimo)/100);
            }
        }
        
        close(cliente);
    }
    
    else if (pid > 0) //Es el padre
    {
        while (wait(NULL) != -1);
        // Cerrar sockets
        close(servidor);
        
    }
    
    
    free(windSrv);    
    return 0;
}
