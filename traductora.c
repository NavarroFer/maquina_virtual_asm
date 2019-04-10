#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "funciones.h"
#include "apodos.h"
extern int REG[16];
extern int RAM[2000];



int buscaCODOP(char* codop){
    codop=strupr(codop);
    char reservados[0x90][50];
    strcpy(reservados[0x01],"MOV");    strcpy(reservados[0x13],"CMP");    strcpy(reservados[0x24],"JZ");    strcpy(reservados[0x32],"OR");
    strcpy(reservados[0x02],"ADD");    strcpy(reservados[0x17],"SWAP");   strcpy(reservados[0x25],"JP");    strcpy(reservados[0x33],"NOT");
    strcpy(reservados[0x03],"SUB");    strcpy(reservados[0x19],"RND");    strcpy(reservados[0x26],"JN");    strcpy(reservados[0x34],"XOR");
    strcpy(reservados[0x04],"MUL");    strcpy(reservados[0x20],"JMP");    strcpy(reservados[0x27],"JNZ");   strcpy(reservados[0x37],"SHL");
    strcpy(reservados[0x05],"DIV");    strcpy(reservados[0x21],"JE");     strcpy(reservados[0x28],"JNP");   strcpy(reservados[0x38],"SHR");
    strcpy(reservados[0x06],"MOD");    strcpy(reservados[0x22],"JG");     strcpy(reservados[0x29],"JNN");   strcpy(reservados[0x81],"SYS");
    /*lalalaAAAAAAAAAAAAAA*/    	   strcpy(reservados[0x23],"JL");     strcpy(reservados[0x31],"AND");   strcpy(reservados[0x8F],"STOP");
    int i=0;
    while(i<=0x8F && strcmp(codop,reservados[i]))//horrible
        i++;


    if (!strcmp(codop,reservados[i]))
        return i;
    else return -1;

}




int buscaRotulo(char* entrada,int cantRotulos,rotulo* rotulos){
    int i=0;
    while(i<cantRotulos && strcmp(entrada,rotulos[i].nombre)!=0)
        i++;
    if (strcmp(entrada,rotulos[i].nombre) == 0)
        return rotulos[i].linea;
    else return -1;

}


void purificadora(FILE* Fasm)
{
    fseek(Fasm, 0, SEEK_SET);
    char linea[990] ="";
    FILE* tmp = fopen("aaa.tmp","wt");
    int pos;
    while(!feof(Fasm))
    {
        char hola = fgetc(Fasm);


        if(hola==(char) -1) //FEOF
            linea[0]=hola;  //is
        else                //evil
            linea[0]=hola;  //no se enojen

        fgets(&linea[1],990,Fasm);
        pos =0;
        salteaespacios(linea,&pos);
        if(linea[pos]!='*')
            while(linea[pos]!='\n' && linea[pos]!='\0'){
                if((linea[pos] == ','  && linea[pos-1] != (char)39) || linea[pos]==(char)9)
                    linea[pos] = ' ';
                if(linea[pos-1] == (char)39 && linea[pos-3]==(char)39)
                    linea[pos] = ' ';
            pos++;
            }
        if (hola!=(char)-1)
            fputs(linea,tmp);
        else fputs("    ",tmp);


    }
    fclose(tmp);
    fclose(Fasm);
    Fasm = fopen("aaa.tmp","rt");

}

void escribeRAM(char instruccion[][50],int cantPalabras,int cantRotulos, rotulo* rotulos, int *error)
{
    int i = 1;
    int ins, op, arg=0,arg1=0,arg2=0,top1=0,top2=0,errorLinea = 0;
    op=buscaCODOP(instruccion[0]);
    if(op==-1)
    {
        *error = 1;
        errorLinea = 1;
       // printf("FFFF FFFF FFFF FFFF\n");
    }
    while(i < cantPalabras && *error != 1 && errorLinea != 1)
    {
        ins = 0;
        if(instruccion[i][0] == '-' || instruccion[i][0] == '#' || instruccion[i][0] == '@' || instruccion[i][0] == '%' || instruccion[i][0] == '\'' || (instruccion[i][0]<='9' && '0'<=instruccion[i][0])) //INMEDIATO
        {
            switch(instruccion[i][0])
                        {
                            case '#': arg = atoi(&instruccion[i][1]) ;
                                break;
                            case '@': arg = strtol(&instruccion[i][1],NULL,8);
                                break;
                            case '%': arg = strtol(&instruccion[i][1],NULL,16);
                                break;
                            case '\'': arg = (int)instruccion[i][1];
                                break;
                            case '-': arg = -1*atoi(&instruccion[i][1]);
                                break;

                            default: arg = atoi(instruccion[i]);
                        }
            switch(i)
            {
            case 0: {*error = 1;
                    errorLinea = 1;
                     i = 10;}
                break;
            case 1:{arg1 = arg;
                    top1 = 0b00;}
                break;
            case 2: {arg2 = arg;
                     top2 = 0b00;}
                break;
            }
        }
        else
                if(toupper(instruccion[i][1]) == 'X')    //REGISTRO
                {
                    switch(i)
                    {
                    case 0x0:{*error = 1;
                            errorLinea = 1;
                              i = 10;}
                        break;
                    case 0x1:{arg1 = (toupper(instruccion[i][0]) - 55);    //Convierte string(num) en int
                              top1 = 0b01;}
                        break;
                    case 0x2:{arg2 = (toupper(instruccion[i][0]) - 55);
                              top2 = 0b01;}
                        break;

                    }

                }
                else
                {
                    if(instruccion[i][0] == '[' && instruccion[i][strlen(instruccion[i]) - 1] == ']') //DIRECTO
                    {
                        switch(i)
                        {
                        case 0x0: {*error = 1;
                                    errorLinea = 1;
                                   i = 10;}
                            break;
                        case 0x1:{arg1 = atoi(&instruccion[i][1]);
                                top1 = 0b10;}
                            break;
                        case 0x2:{arg2 = atoi(&instruccion[i][1]);
                                top2 = 0b10;}
                            break;
                        }
                    }
                    else //ROTULO
                    {
                        if( cantRotulos > 0)
                        {
                            int linea = buscaRotulo(instruccion[i],cantRotulos,rotulos);
                            if (linea!=-1)
                            {
                                switch(i)
                                {
                                    case 0:{*error = 1;
                                            errorLinea = 1;
                                            i = 10;}
                                    case 1:{arg1=linea;
                                            top1=0b00;
                                            break;
                                            }
                                    case 2:{arg2=linea;
                                            top2=0b00;
                                            break;
                                            }
                                }
                            }
                        }

                        else
                        {
                            *error = 1;
                            errorLinea = 1;
                            i = 10;
                        }

                    }
                }
            i++;
        }
    if(!errorLinea)
    {
        // PASAR A HEXA
        printf("[0000 %04x]: ",DS);
        printf("%04x ",op);
        op = op << 16;
        printf("00%d%d ",top1,top2);
        top1 = top1 << 4;
        ins = op | top1 | top2;
        int arg11 = (arg1 & 0xFFFF0000) >> 16;
        int arg12 = arg1 & 0x0000FFFF;
        int arg21 = (arg2 & 0xFFFF0000) >> 16;
        int arg22 = arg2 & 0x0000FFFF;
        printf("%04x ",arg11); printf("%04x ",arg12);
        printf("%04x ",arg21); printf("%04x ",arg22);
        printf("%d:", (int)DS/3 +1);
        switch(cantPalabras)
        {
            case 3: printf("%s %s,%s",instruccion[0],instruccion[1],instruccion[2]);
                break;
            case 2: printf("%s %s",instruccion[0],instruccion[1]);
                break;
            case 1: printf("%s ",instruccion[0]);
                break;

        }




        RAM[DS] = ins;
        RAM[DS+1] = arg1;
        RAM[DS+2] = arg2;
        DS+=3;
    }
    else {
        printf("FFFF FFFF FFFF FFFF");
        RAM[DS] = 0xFFFFFFFF;
        RAM[DS+1] = 0xFFFFFFFF;
        RAM[DS+2] = 0xFFFFFFFF;
        DS+=3;
          }
  }

void salteaespacios(char* linea, int* pos)
{
    while(linea[*pos]==' ')
        (*pos)++;
}

void traductora(FILE *Fasm,int cantRotulos, rotulo* rotulos, int *error)
{
    purificadora(Fasm);
    char linea[990] = " ";
    char instruccion[5][50] = {" "," "," "," "," "};
    fseek(Fasm, 0, SEEK_SET);
    int pos;
    int cantPalabras=0;
    while(!feof(Fasm))//while de linea en linea
    {
        strcpy(instruccion[0],"");strcpy(instruccion[1],"");strcpy(instruccion[2],"");
        pos = 0;
        fgets(linea,990,Fasm);
        cantPalabras=0;
        salteaespacios(linea,&pos);
        if(linea[pos]!='\n' && linea[pos]!='\0'){
            if (linea[pos]!='*' && linea[pos]!=';')
            { //if anti comentarios
                pos = 0;
                salteaespacios(linea,&pos);
                while(linea[pos] != '\n' && linea[pos]!='\0' && linea[pos]!=';') //while para la linea
                {
                    salteaespacios(linea,&pos);
                    if(linea[pos]=='\'')
                    {
                        instruccion[cantPalabras][0]=linea[pos++];
                        instruccion[cantPalabras][1]=linea[pos++];
                        instruccion[cantPalabras][2]='\0';
                        cantPalabras++;
                        if(linea[pos]=='\'')
                            pos++;
                    }
                    else
                    {
                    //ahora leo la palabrita
                    int minipos =0;
                    while(linea[pos]!='\n' && linea[pos]!='\0' && linea[pos]!=':' && linea[pos]!=';' && linea[pos]!=' ' && linea[pos]!='\t')
                    { //while por palabra
                            instruccion[cantPalabras][minipos]=linea[pos];
                            minipos++;//una mas en la palabra
                            pos++;// una mas en la linea
                    }
                    instruccion[cantPalabras][minipos]='\0';
                    if(linea[pos]!=':')
                    {
                        if(strcmp(instruccion[cantPalabras],""))
                            cantPalabras++;
                    }
                    else
                        pos++;
                    }
                }//fin de linea
               escribeRAM(instruccion,cantPalabras, cantRotulos, rotulos,error);
               if(linea[pos]==';'){//comentario corto
                    linea[strlen(linea)-1]='\0'; //jojo
                    printf("%s",&linea[pos]);
               }
                printf("\n");
            }//fin if anti comentarios
            else{ //comentario largo
                    linea[strlen(linea)-1]='\0'; //jojo
                    printf("%s\n",linea);
            }
            }
    }//fin linea en linea
}
