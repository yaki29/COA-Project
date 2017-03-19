//Program to conert assembly code into binary.
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define registerLength 3
#define labLength 3
#define dataLength 3
#define maxOpcodes 25
#define instructionLength 8

//Structure for storing various tables
struct Mnemonic
{
    char text[20];
    char binary[50];
};
typedef struct Mnemonic Mnemonic;

Mnemonic opcodesTable[50];
Mnemonic registerTable[50];
Mnemonic labelTable[50];

int numberOfOpcodes=0,numberOfRegister=0,numberOfLabel=0;
char code[10];

void generateRegisterTable(); //function to generate hash table for registers
void generateOpcodeTable(); //function to generate hash table for OP Codes
void stringReversal(char *s); //Function to reverse a given string
void binary(int num,int len,char *mem); //convert a decimal number into a binary string of length l
void checkingLabels(); //this is pass 1, all the labels are looked for and jump statements are assigned proper Instruction Locations
void generateLabelTable(); //function to generate hash table for labels
void printOpcodeTable(); //print Opcode Table
void printRegisterTable(); //print Register Table
void printLabelTable(); //print Label Table
void displayAssemblyCode(); //Print assembly Code
void displayMachineCode(); //Print Machine Code
void getBinary(char *s,int l,int h,char *mem); //function to convert a number in form of string to binary string
void machineCodeGenerator(char *s,int l,int h,int *flag,int in_c,FILE *out); //utility function to generate machine code
void convertAssemblyToMachine(); //main function to convert assembly code to machine code



int main()
{
    int choice=0;
    //generate hash tables from "opcodes.txt" and "registers.txt"
    generateOpcodeTable();
    generateRegisterTable();
    //Menu
    while(choice!=7)
    {
        printf("**********Menu*************\n");
        printf("1.Print Opcode Table\n");
        printf("2.Print Register Table\n");
        printf("3.Convert Assembly to binary\n");
        printf("4.Print Label Table\n");
        printf("5.Print Assembly Code\n");
        printf("6.Print Machine Code\n");
        printf("7.Exit\n");

        scanf("%d",&choice);
        printf("\n");
        switch(choice)
        {
        case 1:
            {
                printOpcodeTable();
                break;
            }
        case 2:
            {
                printRegisterTable();
                break;
            }
        case 3:
            {
                printf("Enter the file name\n");
                scanf("%s",code);
                checkingLabels();//this is pass 1
                generateLabelTable();
                convertAssemblyToMachine();//pass 2
                break;
            }
        case 4:
            {
                printLabelTable();
                break;
            }
        case 5:
            {
                displayAssemblyCode();
                break;
            }
        case 6:
            {
                displayMachineCode();
                break;
            }
        }
        printf("\n\n");
    }
    return 0;
}

void generateRegisterTable()
{
    FILE *op=fopen("registers.txt","r");
    if(op==NULL)
    {
        printf("Register Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",registerTable[numberOfRegister].text,registerTable[numberOfRegister].binary)!=EOF){numberOfRegister++;}
    }
    fclose(op);
}

void generateOpcodeTable()
{
    FILE *op=fopen("opcodes.txt","r");
    if(op==NULL)
    {
        printf("Opcode Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",opcodesTable[numberOfOpcodes].text,opcodesTable[numberOfOpcodes].binary)!=EOF){numberOfOpcodes++;}
    }
    fclose(op);
}

void stringReversal(char *s)
{
    int i,j,l=strlen(s);
    char c;
    for(i=0,j=l-1;i<j;i++,j--)
    {
        c=s[i];
        s[i]=s[j];
        s[j]=c;
    }
}

void binary(int num,int len,char *mem)
{
    int i;
    for(i=0;i<len;i++)
    {
        mem[i]=num%2+'0';
        num>>=1;
    }
    mem[i]='\0';
    stringReversal(mem);
}

void checkingLabels()
{
    int i;
    FILE *as=fopen(code,"r");
    FILE *lb=fopen("labels.txt","w");
    char s[50],lab[10],mem[10];
    int line_cnt=0,lab_cnt=0;
    while(fgets(s,50,as)!=NULL)
    {
        s[strlen(s)-1]='\0';
        line_cnt++;
        if(s[0]=='X')
        {
            lab_cnt++;
            lab[0]=s[0];
            lab[1]=s[1];
            if(s[2]==':')
            {
                lab[2]='\0';
            }
            else
            {
                lab[2]=s[2];
                lab[2]='\0';
            }
            binary(line_cnt-(lab_cnt-1),labLength,mem);
            fprintf(lb,"%s %s\n",lab,mem);
        }
    }
    fclose(as);
    fclose(lb);
}

void generateLabelTable()
{
    numberOfLabel=0;
    FILE *op=fopen("labels.txt","r");
    if(op==NULL)
    {
        printf("Label Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",labelTable[numberOfLabel].text,labelTable[numberOfLabel].binary)!=EOF){numberOfLabel++;}
    }
    fclose(op);
}

void printOpcodeTable()
{
    int i=0;
    for(i=0;i<numberOfOpcodes;i++)
    {
        printf("%s %s\n",opcodesTable[i].text,opcodesTable[i].binary);
    }
}

void printRegisterTable()
{
    int i=0;
    for(i=0;i<numberOfRegister;i++)
    {
        printf("%s %s\n",registerTable[i].text,registerTable[i].binary);
    }
}

void printLabelTable()
{
    int i=0;
    for(i=0;i<numberOfLabel;i++)
    {
        printf("%s %s\n",labelTable[i].text,labelTable[i].binary);
    }
}

void displayAssemblyCode()
{
    FILE *as=fopen(code,"r");
    char s[50];
    while(fgets(s,50,as)!=NULL)
    {
        printf("%s",s);
    }
    fclose(as);
}

void displayMachineCode()
{
    FILE *as=fopen("machine.txt","r");
    char s[50];
    while(fgets(s,50,as)!=NULL)
    {
        printf("%s",s);
    }
    fclose(as);
}

void getBinary(char *s,int l,int h,char *mem)
{
    int num=0,i;
    for(i=l;i<h;i++)
    {
        num=num*10+(s[i]-'0');
    }
    for(i=0;i<dataLength;i++)
    {
        mem[i]=num%2+'0';
        num>>=1;
    }
    mem[i]='\0';
    stringReversal(mem);
}

void machineCodeGenerator(char *s,int l,int h,int *flag,int in_c,FILE *out)
{
    char mem[50];
    int i;
    if(in_c==1)
    {
        if(s[h-1]=='D')//Immediate addressing mode found, set flag to 1
        {
            (*flag)=1;
        }
        for(i=l;i<h;i++)
        {
            mem[i]=s[i];
        }
        mem[i]='\0';
        for(i=0;i<numberOfOpcodes;i++)
        {
            if(!strcmp(opcodesTable[i].text,mem))
            {
                fprintf(out,"%s",opcodesTable[i].binary);
                return;
            }
        }
    }
    else
    {
        if(*flag==1&&in_c==2)//constant value
        {
            getBinary(s,l,h,mem);
            fprintf(out,"%s",mem);
            return;
        }
        else if(in_c==2)//registers
        {
            for(i=l;i<h;i++)
            {
                mem[i-l]=s[i];
            }
            mem[i-l]='\0';
            for(i=0;i<numberOfRegister;i++)
            {
                if(!strcmp(registerTable[i].text,mem))
                {
                    fprintf(out,"%s",registerTable[i].binary);
                    return;
                }
            }
            for(i=0;i<numberOfLabel;i++)//if its not in register then its label
            {
                if(!strcmp(labelTable[i].text,mem))
                {
                    fprintf(out,"%s",labelTable[i].binary);
                    return;
                }
            }
        }
        else if(in_c==3)//3rd part of instruction if exists is always register
        {
            for(i=l;i<h;i++)
            {
                mem[i-l]=s[i];
            }
            mem[i-l]='\0';
            for(i=0;i<numberOfRegister;i++)
            {
                if(!strcmp(registerTable[i].text,mem))
                {
                    fprintf(out,"%s",registerTable[i].binary);
                    return;
                }
            }
        }
    }
}

void convertAssemblyToMachine()
{
    FILE *as=fopen(code,"r");
    FILE *out=fopen("machine.txt","w");
    char s[50];
    int i,j,flag,in_c;
    if(as==NULL)
    {
        printf("Assembly Code not found\n");
        return;
    }
    while(fgets(s,50,as)!=NULL)
    {
        s[strlen(s)-1]='\0';
        j=0;
        in_c=0;
        flag=0;
        int z=0;
        int fl=0;
        for(i=0;i<=strlen(s);i++)
        {
            if(s[i]==' '||s[i]=='\0')
            {

                in_c++;
               if(in_c==1&&s[j]=='X')
               {
                   fl=1;
                   break;
               }
                machineCodeGenerator(s,j,i,&flag,in_c,out);
                j=i+1;
            }

        }
       if(!fl)
            fprintf(out,"\n");
    }
    fclose(as);
    fclose(out);
    printf("Successfully Converted to Machine instruction\n");
}