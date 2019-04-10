#ifndef INSTRUCCIONES_H_INCLUDED
#define INSTRUCCIONES_H_INCLUDED

int ADD(int *op1,int *op2);
int MOV(int *op1,int *op2);
int SUB(int *op1,int *op2);
int MUL(int *op1,int *op2);
int DIV(int *op1,int *op2);
int MOD(int *op1,int *op2);
int CMP(int *op1,int *op2);
int SWAP(int *op1,int *op2);
int RND(int *op1,int *op2);
int JMP(int *op1,int *op2);
int JE(int *op1,int *op2);
int JG(int *op1,int *op2);
int JL(int *op1,int *op2);
int JZ(int *op1,int *op2);
int JP(int *op1,int *op2);
int JN(int *op1,int *op2);
int JNZ(int *op1,int *op2);
int JNP(int *op1,int *op2);
int JNN(int *op1,int *op2);
int AND(int *op1,int *op2);
int OR(int *op1,int *op2);
int NOT(int *op1,int *op2);
int XOR(int *op1,int *op2);
int SHL(int *op1,int *op2);
int SHR(int *op1,int *op2);
int SYS(int *op1,int *op2);
int STOP(int *op1,int *op2);


#endif // INSTRUCCIONES_H_INCLUDED
