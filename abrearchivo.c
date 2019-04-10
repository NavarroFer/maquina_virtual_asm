#include <stdio.h>
#include <stdlib.h>
#include <string.h> //tiene strlen()
#include <ctype.h> //tiene a toupper
#include "apodos.h"
#include "funciones.h"
#include "instrucciones.h"
#include <math.h>
extern int REG[16];
extern int RAM[2000];





//int abrirArchivos(int argc,char** argv,FILE** Fasm,FILE** Fimg)
int abrirArchivos(int argc,char argv[][500],FILE** Fasm,FILE** Fimg)
{
    int hayerror = 0;
    //creo q esto se podria hacer mas lindo -mau
    int s = 38; //el bicho q arregla lo de los argumentos
	int arg = 91; //1 es -x, 2 es -t, 3 es -a o nada


    /*parte procesa aprametro*/
	if(argc>=2){
        if (strlen(argv[1])==2 && argv[1][0]=='-') //SI HAY UN PARAMETRO
        {
            s = 1;
            switch(toupper(argv[1][1]))
            {//
                case 'X':arg=1;
                        break;
                case 'T':arg=2;
                        break;
                case 'A':arg=3;
                          break;

                default:{
                        hayerror = hayerror | 0b1;
                        }
            }
        }
        else // no hay parametro!! va -A
        {
            s = 0;
            arg=3;
        }
    }
    else {
            hayerror= hayerror | 0b10;
         }
    /*FIN parte procesa aprametro*/

	/*parte arranca archivos*/

	if(!hayerror){
		switch(arg)
		{
            case 0b01:	{//solo execute mv.exe -x tal.img
								*Fimg = fopen(argv[1+s],"rb");
								hayerror = (Fimg==NULL)*0b100;
								}
								break;
            case 0b10:	{//solo traducir mv.exe -t in.asm out.img
								*Fasm=fopen(argv[1+s],"rt");
								hayerror = (argv[1+s]==NULL)*0b1000;
                                if (argv[2+s]==NULL)
                                    *Fimg=fopen("salida.img","wb");
                                else
                                    *Fimg=fopen(argv[2+s],"wb");
								}
								break;
            case 0b11:	{//ambas mv.exe in.asm O mv.exe -A in.asm
                                *Fasm=fopen(argv[1+s],"rt");
								hayerror = (Fasm==NULL)*0b1000;
								}
								break;
      }
	}

    char errores[200][10];
    strcpy(errores[0],"El argumento onda -X esta mal!!\n");
    strcpy(errores[1],"Muy pocos argumentos!!!\n");
    strcpy(errores[2],"El archivo .img se intento abrir y no pudo\n");
    strcpy(errores[3],"El archivo .asm no se pudo abrir\n");

	int aux=1;
	while (aux <= hayerror){
        if (hayerror & aux)
            printf(errores[(int) log2(aux)]);//aaaaaaaaaaaaaaaaaaaa
        aux = aux << 1;
	} //es tan compacto!!!!!!!!!

     /*FIN parte arranca archivos*/
    return hayerror?-1:arg; //-1 si hay error, y si no 0b01, 0b10 y 0b11

}
