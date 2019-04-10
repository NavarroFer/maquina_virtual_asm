#include "instrucciones.h"
#include <stdlib.h>
#include "funciones.h"
#include "apodos.h"
extern int REG[16];
extern int RAM[2000];

void setCC(int resultado) //el bit de la derecha del todo es 1 si el resultado es 0
{//el de la izq del todo es 1 si es negativo
    CC = 0;
    CC = (resultado==0) | ((resultado<0) <<31);
}


int ADD(int *op1,int *op2)
{
	*op1 += *op2;
    setCC(*op1);
    return 0;
}
int MOV(int *op1,int *op2)
{
	*op1 = *op2;
	return 0;
}
int SUB(int *op1,int *op2)
{
	*op1 -= *op2;
	setCC(*op1);
	return 0;
}
int MUL(int *op1,int *op2)
{
	*op1 *= *op2;
	setCC(*op1);
	return 0;
}
int DIV(int *op1,int *op2)
{
    if(*op2 == 0)
    {
        printf("\n\nDIVISION POR CERO!");
        return 1;
    }
    else
    {
        (*op1) /= (*op2);
        setCC(*op1);
        return 0;
    }
}
int MOD(int *op1,int *op2)
{
    if(*op2 == 0)
    {
        printf("\n\nDIVISION POR CERO!");
        return 1;
    }
    else
    {
        setCC(*op1/(*op2));//asi
        *op1 = (*op1) % (*op2);
        return 0;
    }

}
int CMP(int *op1,int *op2)
{
    setCC((*op1) - (*op2)); // jeje
    return 0;
}
int SWAP(int *op1,int *op2)
{
	int aux;
	aux = *op1;
	*op1 = *op2;
	*op2 = aux;
	return 0;
}
int RND(int *op1,int *op2)
{
    *op1 = rand() % (*op2) + 1;
    return 0;

}
int JMP(int *op1, int *op2)
{
    IP = (*op1-1)*3;
    return 0;
}
int JE(int *op1,int *op2)
{
    if(*op1 == AX)
        IP = (*op2-1)*3;
    return 0;
}
int JG(int *op1,int *op2)
{
    if(*op1 > AX)
        IP = (*op2-1)*3;
    return 0;
}
int JL(int *op1,int *op2)
{
    if(*op1 < AX)
        IP = (*op2-1)*3;
    return 0;
}
int JZ(int *op1,int *op2)
{
    if((CC & 0b1) == 1)
        IP = (*op1-1)*3;
    return 0;
}
int JP(int *op1,int *op2)
{
    if(CC==0)
        IP = (*op1-1)*3;
    return 0;
}
int JN(int *op1,int *op2)
{
    if (CC & (0b1<<31))
        IP = (*op1-1)*3;
    return 0;
}
int JNZ(int *op1,int *op2)
{

    if((CC & 0b1) == 0b0)
        IP = (*op1-1)*3;
    return 0;
}
int JNP(int *op1,int *op2)
{
    if(CC != 0)
        IP = (*op1-1)*3;
    return 0;
}
int JNN(int *op1,int *op2)
{
    if((CC & (0b1<<31)) != 1)
        IP = (*op1-1)*3;
    return 0;
}
int AND(int *op1,int *op2)
{
	*op1 = (*op1) & (*op2);
	setCC(*op1);
    return 0;
}
int OR(int *op1,int *op2)
{
	*op1 = (*op1) | (*op2);
	setCC(*op1);
	return 0;
}
int NOT(int *op1,int *op2)
{
	*op1= ~(*op1);
	setCC(*op1);
	return 0;
}
int XOR(int *op1,int *op2)
{
	*op1 = (*op1) ^ (*op2);
	setCC(*op1);
	return 0;
}
int SHL(int *op1,int *op2)
{
	*op1 = (*op1) << (*op2);
	setCC(*op1);
	return 0;
}
int SHR(int *op1,int *op2)
{
	*op1 = (*op1) >> (*op2);
	setCC(*op1);
	return 0;
}
int SYS(int *op1, int *op2)
{
    int i;
    switch(*op1){
    case 1:{//read AX=formato CX=largo DX=direccion RAM donde exribir
         //mostrar la direc

        if (AX & (1<<8)){//es un numerito!!
            char palabras[50][50];
            for(i=0;i<CX;i++)
            {
                if (AX & (1 <<12))
                    printf("[0000 %04d]:",DS+DX+i);
                fgets(palabras[i],50,stdin);
            }


            switch(AX & 0xF)
            {
            case 0b1:
                {
                    for(i=0;i<CX;i++)
                        RAM[DS+DX+i]=strtol(palabras[i],NULL,10);
                    break;
                }
            case 0b100:
                {
                    for(i=0;i<CX;i++)
                        RAM[DS+DX+i]=strtol(palabras[i],NULL,8);
                    break;
                }
            case 0b1000:
                {
                    for(i=0;i<CX;i++)
                        RAM[DS+DX+i]=strtol(palabras[i],NULL,16);
                    break;
                }
                default:{printf("aaa eror!!");}
            }
        }
        else
        {
            char palabra[50];
            fgets(palabra,50,stdin);
            for(i=0;i<=CX;i++)//gabar caracter a caracter el igual es para q llegue hasta el barra 0 jeje
                RAM[DS+DX+i]=palabra[i];
        }

        break;}
    case 2:
    case 3:{
            if(*op1 == 3)
            {
                printf("[DS]: %d\n",DS);
                printf("[IP]: %d\n",IP);
                printf("[CC]: %d\n",CC);
                printf("[AX]: %d\n",AX);
                printf("[BX]: %d\n",BX);
                printf("[CX]: %d\n",CX);
                printf("[DX]: %d\n",DX);
                printf("[EX]: %d\n",EX);
                printf("[FX]: %d\n",FX);
            }



            if((AX & 1) == 0b1) //DECIMAL
                for(i=0;i<CX;i++)
                {
                    printf("%d ",RAM[DS+DX+i]);
                    if(AX & (1<<8)) //Endline
                        printf("\n");
                }

            if((AX & 0b10) == 0b10) //BINARIO
                for(i=0;i<CX;i++)
                {
                    fwrite(&RAM[DS+DX+i],sizeof(int),1,stdout);
                }

            if((AX & 1<<2) ==1<<2) //OCTAL
                for(i=0;i<CX;i++)
                {
                    printf("%o ",RAM[DS+DX+i]);
                    if(AX & (1<<8)) //Endline
                        printf("\n");
                }
            if((AX & 1<<3) == 1<<3) //HEXA
                for(i=0;i<CX;i++)
                {
                    printf("%x ",RAM[DS+DX+i]);
                    if(AX & (1<<8)) //Endline
                        printf("\n");
                }
            if((AX & 1<<4) == 1<<4) //CARACTER
                for(i=0;i<CX;i++)
                {
                    printf("%c",(RAM[DS+DX+i]& 0xFF));
                    //printf("%c ",RAM[DS+DX+i]);
                    if(AX & (1<<8)) //Endline
                        printf("\n");
                }

        break;}
    default:{printf("esto no tendria que aparecer nunca");}
    }
    return 0;
}
int STOP(int *op1, int *op2)
{
    IP=-900;
    return 0;
}


