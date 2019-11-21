/*
 * Programa que reproduce archivos .wav
 * padsp ./rep dancin.wav
*/
#include <sys/soundcard.h>      ///Se encuentran definicas las constantes que indican la configuracion de la tarjeta de audio
#include <sys/ioctl.h>          ///Permite usar la funcion ioctl();
#include <fcntl.h>              ///Se encunetran definidas las constantes que indican en que modo se abren los archivos
#include <errno.h>              ///Desplegan un mensaje de Error si ocurre alguno.
#include <stdlib.h>             ///Se encuentran declaradas las funciones malloc,exit,free
#include <stdio.h>               ///Permite el uso de entrada y salida del programa
#include <string.h>
#include <unistd.h>
#define tiempo 1500

//
//Main
//
int main(int argc,char *argv[]){
	char* buffer;      ///Buffer en donde se carga la pista de audio
	int pista;                  ///descriptor de pista de audio
	int tar_escritura;          ///descriptor de tajreta de audio
	int taza = 45000;           ///taza de muestreo
	int canales = 1;            /// canal estereo
	int formato = AFMT_U16_LE;  ///Formato para 16 bits definido en la libreria sys/soundcard.h
	char NombreArchivo[100];
	strcpy( NombreArchivo, argv[1] );
	printf("\n->Repoduciendo: %s \n",NombreArchivo);
	if( (tar_escritura = open("/dev/dsp",O_WRONLY)) ==-1){
		perror("Error al abrir tarjeta de Audio\n");  ///Abre Tarjeta de Audio
		return (-1);
	}
	if( (pista = open( NombreArchivo,O_RDONLY )) ==-1){
		perror("Error al Abrir pista de audio\n");        ///Abre pista de Audio
		exit(-1);
	}
	buffer = malloc(taza*tiempo*(canales+1));  ///Crea buffer de audio
	read(pista,buffer,taza*tiempo*(canales+1));         ///Carga el audio en el Buffer
	if(ioctl(tar_escritura, SNDCTL_DSP_STEREO,&canales)==-1){
		perror("Error al configurar canales de audio\n"); ///Configura los canales de Audio
		return errno;
	}
	if(ioctl(tar_escritura, SNDCTL_DSP_SETFMT,&formato)==-1){
		perror("Error al leer el formato de pista\n");     ///Configura el formato de la pista de Audio
		return errno;
	}
	if (ioctl(tar_escritura, SNDCTL_DSP_SPEED,&taza)==-1){
		perror("Error al configurar la taza de muestreo\n");   ///Configura la taza de muestreo en hz
		return errno;
	}
	write(tar_escritura,buffer,taza*tiempo*(canales));    ///Escribe la pista de audio en la tarjeta de audio
	if(ioctl(tar_escritura, SNDCTL_DSP_SYNC)==-1){
		perror("Error al enviar pista");                    ///Sincroniza y reproduce la pista de audio
		return errno;
	}
	free(buffer);           ///Libera el buffer
	close(pista);           ///Cierra el archivo de la pista de audio
	close(tar_escritura);   ///Cierra el archivo controlador de la tarjeta de audio
	return 0;
}
