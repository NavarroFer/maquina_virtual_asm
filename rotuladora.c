#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "apodos.h"
extern int REG[16];
extern int RAM[2000];


int rotulovalido(char* posiblerotulo){

    char reservados[50][27];
    strcpy(reservados[0],"STOP");
    strcpy(reservados[0x01],"MOV");    strcpy(reservados[7],"CMP");    strcpy(reservados[14],"JZ");    strcpy(reservados[21],"OR");
    strcpy(reservados[0x02],"ADD");    strcpy(reservados[8],"SWAP");   strcpy(reservados[15],"JP");    strcpy(reservados[22],"NOT");
    strcpy(reservados[0x03],"SUB");    strcpy(reservados[9],"RND");    strcpy(reservados[16],"JN");    strcpy(reservados[23],"XOR");
    strcpy(reservados[0x04],"MUL");    strcpy(reservados[10],"JMP");    strcpy(reservados[17],"JNZ");   strcpy(reservados[24],"SHL");
    strcpy(reservados[0x05],"DIV");    strcpy(reservados[11],"JE");     strcpy(reservados[18],"JNP");   strcpy(reservados[25],"SHR");
    strcpy(reservados[0x06],"MOD");    strcpy(reservados[12],"JG");     strcpy(reservados[19],"JNN");   strcpy(reservados[26],"SYS");
    /*lalalaAAAAAAAAAAAAAA*/    	     strcpy(reservados[13],"JL");     strcpy(reservados[20],"AND");
    int i=1;
    int valido=1;
    while(i<=27 && valido){
        valido=abs(strcmp(reservados[i],posiblerotulo));
        i++;
    }
    return valido;

}


void rotuladora(FILE* Fasm,int* cantOut,rotulo** rotulosOut)
{
    int cantRotulos=0;
    rotulo* rotulos = (rotulo*)malloc(RMax*sizeof(rotulo));


    char linea[990] = "";
    int lineaactual=0;
    int pos=0;
    int posI=0;
    while(!feof(Fasm)){

        pos=0; posI=0;
        fgets(linea,990,Fasm);
        while (linea[pos]==' ' || (int)linea[pos] == 9){ //SALTEADOR DE ESPACIOS
            pos++;
            posI++;
            }
        if(linea[pos]!='*')//si no es un comentario
        {
            lineaactual++;
            //printf("Linea actual nro %i: %s",lineaactual,linea); //P DEBUG NO MAS

            while(linea[pos]!=':' && linea[pos]!='\n' && linea[pos]!='\0' && linea[pos]!=' ' &&(int) linea[pos] !=9)
                pos++;
            if(linea[pos] == ':'){
                char posiblerotulo[50];
                int largo=pos-posI;
                strncpy(posiblerotulo,(&linea[posI]),largo);
                posiblerotulo[largo]='\0'; //jaja
                if(rotulovalido(posiblerotulo)){
                    char* nombre = (char*)malloc(50);
                    strcpy(nombre,posiblerotulo);

                    rotulos[cantRotulos].nombre=nombre;
                    rotulos[cantRotulos].linea=lineaactual;
                    cantRotulos++;
                }
                else{
                    printf("Se encontro un rotulo invalido:%s \n",posiblerotulo);
                }
            }
        }
    }
    //printf("Se encontraron %i rotulos:\n",cantRotulos);
    //for(int i=0;i<cantRotulos;i++){
       // printf("el rotulo numero %i es %s y esta en la linea numero %i\n",i,rotulos[i].nombre,rotulos[i].linea);
    *rotulosOut=rotulos;
    *cantOut=cantRotulos;
    }

    //aca mando todo bien a donde tiene que ir

//}
