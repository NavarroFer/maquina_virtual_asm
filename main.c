#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include "instrucciones.h"
#define DS REG[2]
#define IP REG[8]
#define CC REG[9]
#define AX REG[10]
#define BX REG[11]
#define CX REG[12]
#define DX REG[13]
#define EX REG[14]
#define FX REG[15]

#define argc 2

typedef int (*f)(int *op1, int *op2);
int REG[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int RAM[2000];




//int main(int argc,char** argv)
int main()
{
    // USAR EL DEFINE const int argc=2;
    char argv[argc][500];
    strcpy(argv[0],"mv.exe");
    strcpy(argv[1],"E8.asm");
    //strcpy(argv[2],"test.asm");
    //strcpy(argv[3],"salida.img"); // REVISAR Q ESTE EXISTA!!!! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    //strcpy(argv[2],"porotin.img");
    //FIN variables crotas p poder ejecutarlo desde el debugger*/
    FILE *Fasm=NULL;
    FILE *Fimg=NULL;
    int cantRotulos=0,error=0;
    rotulo* rotulos = NULL;

    int arg = abrirArchivos(argc,argv,&Fasm,&Fimg);
    /* arg 1= solo ejecutar
        arg 2 = solo traducir
        arg 3 = todo*/

    if (arg!=-1){
        if (arg & 0b10)//este es el if traductor (arg == 2 || arg == 3)
        {
           /*inicio del interprete*/
            rotuladora(Fasm,&cantRotulos,&rotulos);
            traductora(Fasm,cantRotulos,rotulos,&error);
            printf("\n");
            fclose(Fasm);
           /*fin del interprete*/
            //aca ya esta todo traducido y catgado en los vectores
            if (!(arg & 0b1) && !error)//lo meto en le archivo si no hay que ejecutar
            {
                fwrite(RAM, 4, 2000, Fimg); //es mas lindo
                fwrite(REG, 4, 16, Fimg); // 16*4
                fclose(Fimg);
            }
        }

        if(arg & 0b01 && !error) //este es el if ejecutor (arg == 1 || arg == 3)
        {
            if((arg & 0b10) == 0) //archivo pasado a bin si no hubo q interpretar
            {
                fread(RAM, 4, 2000, Fimg);
                fread(REG, 4, 16, Fimg);
                fclose(Fimg);
            }
            //parte q ejecuta
            ejecucion();
        }
    if(error)
        printf("ERROR DE TRADUCCION\n");
    }
    else
        printf("La ejecucion no puede continuar por que hubo errores al abrir los archivos");


    printf("\n\n");
    printf("*****************************************************\n");
    printf("*                                                   *\n");
    printf("*  Ejecucion terminada, presione enter para cerrar  *\n");
    printf("*                                                   *\n");
    printf("*****************************************************\n");


    getchar();
    return 0;
}




void iniciaf(f func[])
{
    func[0x01] = MOV;    func[0x13] = CMP;    func[0x24] = JZ;    func[0x32] = OR;
    func[0x02] = ADD;    func[0x17] = SWAP;   func[0x25] = JP;    func[0x33] = NOT;
    func[0x03] = SUB;    func[0x19] = RND;    func[0x26] = JN;    func[0x34] = XOR;
    func[0x04] = MUL;    func[0x20] = JMP;    func[0x27] = JNZ;   func[0x37] = SHL;
    func[0x05] = DIV;    func[0x21] = JE;     func[0x28] = JNP;   func[0x38] = SHR;
    func[0x06] = MOD;    func[0x22] = JG;     func[0x29] = JNN;   func[0x81] = SYS;
    /*lalalaAAAAAAA*/;    func[0x23] = JL;     func[0x31] = AND;   func[0x8F] = STOP;
}

void OP(int top1,int top2,int **op1,int **op2)
{
	if(top1 == 0b00)
		*op1 = &RAM[IP+1];
    else if(top1 == 0b01)
		*op1 = &REG[RAM[IP+1]];
	else
		*op1 = &RAM[DS + RAM[IP+1]]   ;

	if(top2 == 0b00)
		*op2 = &RAM[IP+2];
	else if(top2 == 0b01)
		*op2 = &REG[RAM[IP+2]];
	else
		*op2 = &RAM[DS + RAM[IP+2]];
}
void ejecucion()
{
    f func[0x90];
    iniciaf(func);
	int ins,top1,top2,*op1,*op2,error = 0;
	IP = 0;
	while(IP>= 0 && IP < DS && error == 0)
	{
		ins = (RAM[IP] & 0xFFFF0000) >> 16;
		top1 = (RAM[IP] & 0b110000) >> 4;
		top2 = (RAM[IP] & 11);
		OP(top1,top2,&op1,&op2);
		IP+=3;
		error = func[ins](op1,op2);
	}
}
