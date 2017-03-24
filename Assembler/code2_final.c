//this C program reads machine instructions and executes it
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//various constants
#define ins_len 8
#define reg_len 3
#define lab_len 3
#define data_len 3
#define max_opcodes 25
//structure for storing various hash tables
struct mnemonic
{
    char text[20];
    unsigned int val;
};
typedef struct mnemonic mnemonic;

mnemonic hash_opcodes[50];
mnemonic hash_register[50];

unsigned int reg[8],instruction[50],songs[8][3];
int machine_code_len=0,num_op=0,num_reg=0,num_lab=0;
int programCounter=0;
//program to convert a binary string to an integer value
unsigned int get_decimal(char *s)
{
    unsigned int num=0;
    int i,l=strlen(s);
    for(i=0;i<l;i++)
    {
        num+=(s[i]-'0')*(1<<(l-i-1));
    }
    return num;
}
//utility function to count the length of machine code
void count_machine_code_length()
{
    FILE *p=fopen("machine.txt","r");
    char s[50];
    if(p==NULL)
    {
        printf("Machine code not found\n");
        return;
    }
    while(fscanf(p,"%s",s)!=EOF){machine_code_len++;}
    fclose(p);
}
//read and convert binary code to integer and store into instruction table
void store_machine_code()
{
    count_machine_code_length();
    FILE *p=fopen("machine.txt","r");
    char s[50];
    if(p==NULL)
    {
        return;
    }
    int i;
    for(i=0;i<machine_code_len;i++)
    {
        fscanf(p,"%s",s);
        instruction[i]=get_decimal(s);
    }
    fclose(p);
}
//generate hash table of opcodes
void gen_hash_table_opcodes()
{
    FILE *op=fopen("opcodes.txt","r");
    char s[50];
    if(op==NULL)
    {
        printf("Opcode Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",hash_opcodes[num_op].text,s)!=EOF)
        {
            hash_opcodes[num_op].val=get_decimal(s);
            num_op++;
        }
    }
    fclose(op);
}
//print hash table of opcodes
void print_opcode_hash_table()
{
    int i=0;
    for(i=0;i<num_op;i++)
    {
        printf("%s %u\n",hash_opcodes[i].text,hash_opcodes[i].val);
    }
}
//print the final value stored in the register
void final_register_staus()
	{
	    int i;
	    printf("R00 %d\n\n",reg[0]);
	    for (i=1;i<8;i++)
	    {
	        printf("%s %u\n\n",hash_register[i].text,reg[i]);
	    }
	    printf("\n\n\n");
	}
//generate hash table of registers
void gen_hash_table_regsiters()
{
    FILE *op=fopen("registers.txt","r");
    char s[50];
    if(op==NULL)
    {
        printf("Register Table not found\n");
    }
    else
    {
        while(fscanf(op,"%s %s",hash_register[num_reg].text,s)!=EOF)
        {
            hash_register[num_reg].val=get_decimal(s);
            num_reg++;
        }
    }
    fclose(op);
}
//print hash table of registers
void print_reg_hash_table()
{
    int i=0;
    for(i=0;i<num_reg;i++)
    {
        printf("%s %d\n",hash_register[i].text,hash_register[i].val);
    }
}
void set_song_table()
{
    int i,j;
    for(i=0;i<5;i++)
    {
        songs[i][0]=1;
        songs[i][1]=0;
        songs[i][2]=0;
    }
    for(i=5;i<8;i++)
    {
        for(j=0;j<2;j++)
        {
            songs[i][j]=0;
        }
        songs[i][3]=-1;
    }
}
//search opcode in hash table
int get_opcode(char *s,unsigned int num)
{
    int i;
    for(i=0;i<num_op;i++)
    {
        if(hash_opcodes[i].val==num)
        {
            strcpy(s,hash_opcodes[i].text);
            return 1;
        }
    }
    return 0;
}
//check if opcode is 2 bit using bit masking
int check_2_bit(unsigned int inst,char *s)
{
    unsigned int num=0;
    int i,found=0;
    for(i=6;i<8;i++)
    {
        num+=(1<<i);
    }
    inst=(inst&num);
    inst=inst>>6;
    found=get_opcode(s,inst);
    if(!found)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//check if opcode is 5 bit using bit masking
int check_5_bit(unsigned int inst,char *s)
{
    unsigned int num=0;
    int i,found=0;
    for(i=3;i<8;i++)
    {
        num+=(1<<i);
    }
    inst=(inst&num);
    inst=inst>>3;
    found=get_opcode(s,inst);
    if(!found)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//check if opcode is 8 bit using bit masking
int check_8_bit(unsigned int inst,char *s)
{
    unsigned int num=0;
    int i,found=0;
    for(i=0;i<8;i++)
    {
        num+=(1<<i);
    }
    inst=(inst&num);
    found=get_opcode(s,inst);
    if(!found)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//Various functions to perform OPCODE instructions

//NOTE:: Bit masking is used for extracting opcode and addresses. At each step the status of register, ALU, flags is displayed
//Status after each line is written in file "status.txt"


//Pause current song
void PAUSE(int i,FILE *op){
    programCounter+=1;
  fprintf(op,"Line %d\n",i+1);
  fprintf(op,"FLAG:PP FLAG IS SET TO 0\n");
  fprintf(op,"Register Status: EMPTY\n");
  fprintf(op,"ALU Status: No ALU Operations Performed\n");
  fprintf(op,"Data Movement: NONE\n");
  fprintf(op, "Program Counter: %d\n",programCounter );   
}
//Delete Current Song
void DCS(int i,FILE *op){
    programCounter+=1;
    reg[7] = reg[7] - 1;
    songs[reg[5]][0]=-1;
    songs[reg[5]][1]=0;
    songs[reg[5]][2]=-1;
  fprintf(op,"Line %d\n",i+1);
  fprintf(op,"FLAG: PP FLAG IS SET TO 0\n");
  fprintf(op,"Register Status: R07 is set to %d\n", reg[7]);
  fprintf(op,"ALU Status: Subtraction is performed\n");
  fprintf(op,"Data Movement: NONE\n");  
  fprintf(op, "Program Counter: %d\n",programCounter );   
}
//Get Most Played Song
void MPS(int i,FILE *op){
   unsigned int k,max= 0,maxIndex;
    
    programCounter+=1;
    for(k=0;k<8;k++)
    {   
        if(songs[k][1]>max)
        {
            max=songs[k][1];
            maxIndex=k;
        }
         
    }
    reg[0]=maxIndex;

  fprintf(op,"Line %d\n",i+1);
  fprintf(op,"FLAG: NONE\n");
  fprintf(op,"Register Status: R00 is set to %d\n", reg[0]);
  fprintf(op,"ALU Status: Logical Comparision operation is performed\n");
  fprintf(op,"Data Movement: Memory to register\n");
  fprintf(op, "Program Counter: %d\n",programCounter );   
  
}
//Shuffle all songs
void SFL(int i,FILE *op){
  programCounter+=1;
  int x = reg[7];
  reg[5]=rand()%x;
  fprintf(op,"Line %d\n",i+1);
  fprintf(op,"FLAG: SFL FLAG SET TO 1\n");
  fprintf(op,"Register Status: R05 is set to %d\n", reg[5]);
  fprintf(op,"ALU Status: Generate a random number\n");
  fprintf(op,"Data Movement: Register to Register\n");
  fprintf(op, "Program Counter: %d\n",programCounter );   
  
}
//Increase volume of songs in R04 register
void VOLU(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    
    if(reg[reg_in]<=9)
    {
        reg[reg_in]+=1;
    }
    else
    {
        printf("Volume is already at maximum.\n");
    }
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op,"ALU Status: Value of Volume R0%d incremented\n",reg_in);
    fprintf(op,"Data Movement: No Data movement operation performed\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");

}
//Reduce volume of songs in R04 register
void VOLL(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    
    if(reg[reg_in]>=0)
    {
        reg[reg_in]-=1;
    }
    else
    {
        printf("Volume is already at minimum.\n");
    }
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op,"ALU Status: Value of Volume R0%d incremented\n",reg_in);
    fprintf(op,"Data Movement: No Data movement operation performed\n");
    fprintf(op,"\n\n");

}
//Play next song in current playlist
void FRW(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    int k;
    for(k=reg[reg_in]+1;;(k++)%8)
    {
        if(songs[k][0]==1)
        {
            if(reg[6]==songs[reg[5]][2])
            {
            	reg[reg_in] = k;
            	break;
            }
        }
        if(k>=8)
            k=0;
    }
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op,"ALU Status: Value of Current song %d \n",reg_in);
    fprintf(op,"Data Movement: No Data movement operation performed\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");

}
//Play previous song in current playlist
void BKW(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    int k;
    for(k=reg[reg_in]-1;;(k--)%8)
    {   
        if(songs[k][0]==1)
        {
           if(reg[6]==songs[reg[5]][2])
            {
            	reg[reg_in] = k;
            	break;
            }
        }
        if(k<=0)
            k=7;
    }
    
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op,"ALU Status: Value of Current song %d \n",reg_in);
    fprintf(op,"Data Movement: No Data movement operation performed\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");

}
//Play ith() song
void PLYD(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    data=(1<<(data_len))-1;
    
    data=(inst&data);
    reg[5]=data;
    songs[data][1]++;
    reg_in=5;
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op, "FLAG PP IS on\n");
    fprintf(op,"ALU Status: NONE");
    fprintf(op,"Data Movement: Immediate addressing\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");
}
//Loop current song for i number of times.
void LPD(unsigned inst,int i,FILE *op)
{
    unsigned int data,reg_in,reg_in1,reg_in2;

    data=(1<<(data_len))-1;
    
    data=(inst&data);
    int k;
    reg_in=5;
    fprintf(op,"Line %d\n",i+1);
    for(k=0;k<data;k++)
    {
       songs[reg[reg_in]][1]++;
       fprintf(op,"Playing song %d for %dth time\n",reg[reg_in],k);  
       programCounter+=1;      
    } 
    
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op, "FLAG Status LP Flag is on\n");
    fprintf(op,"ALU Status: NONE");
    fprintf(op,"Data Movement: NONE\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");
}
// Play the current song
void PLYR(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    
    
    fprintf(op,"Line %d\n",i+1);
    fprintf(op, "The song which is played is no. %d \n", reg[reg_in]);
    fprintf(op, "Flag Status PP is set to 1\n");
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op,"ALU Status: Value of Current song %d \n",reg_in);
    fprintf(op,"Data Movement: No Data movement operation performed\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");

}

//Go to ith time in current song.
void REWD(unsigned inst,int i,FILE *op)
{
    programCounter+=1;
    unsigned int data,reg_in,reg_in1,reg_in2;
    data=(1<<(data_len))-1;
    data=data<<reg_len;
    data=(inst&data);
    data=(data>>reg_len);
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    reg[reg_in] = data;
    
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R0%d= %d\n",reg_in,reg[reg_in]);
    fprintf(op,"ALU Status: NONE");
    fprintf(op,"Data Movement: Using Immediate addressing to transfer into register\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");
}
//ADD how many songs 
void ADSD(unsigned inst,int i,FILE *op)
{

    programCounter+=1;
    int k,x;
    unsigned int data,reg_in,reg_in1,reg_in2;
    data=(1<<(data_len))-1;
    data=data<<reg_len;
    data=(inst&data);
    data=(data>>reg_len);
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
    reg[reg_in] = data;

    x = reg[7];
    reg[7] += data;
    if(reg[7]>=8)
    {
        fprintf(op,"Last %d songs are rejected to add\n", (reg[7]-8));
     reg[7] = 8;
    }
    for(k=x+1;k<reg[7];k++)
    {
        songs[k][0] = 1;
        songs[k][1] = 0;
        songs[k][2] = 0;
    }

    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R07= %d\n",reg[7]);
    fprintf(op,"ALU Status: Addition is performed\n");
    fprintf(op,"Data Movement: NONE\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");
}


// MOVE a value to register
void MOVD(unsigned inst,int i,FILE *op)
{

    programCounter+=1;
    int k,x;
    unsigned int data,reg_in,reg_in1,reg_in2;
    data=(1<<(data_len))-1;
    data=data<<reg_len;
    data=(inst&data);
    data=(data>>reg_len);
    reg_in=(1<<(reg_len))-1;
    reg_in=(inst&reg_in);
   
    reg[reg_in] = data;

   
    fprintf(op,"Line %d\n",i+1);
    fprintf(op,"Register Status: R%d changed to %d\n", reg_in, reg[reg_in]);
    fprintf(op,"ALU Status: NONE\n");
    fprintf(op,"Data Movement: Immediate addressing to Register\n");
    fprintf(op, "Program Counter: %d\n",programCounter );   
    fprintf(op,"\n\n");
}
void print_mem()
{

	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<3;j++)
		{
			printf("songs[%d][%d ] = %d\n", i,j,songs[i][j]);
		}
		printf("\n\n");
}
//function to interpret and execute machine code
void solve_machine_code()
{   
    FILE *op=fopen("status.txt","w");
    char s[50];
    int i=0;
    unsigned int inst;
    while(i<machine_code_len)
    {
        inst=instruction[i];
        
        if(check_2_bit(inst,s))
        {
            if(!strcmp(s,"REWD"))
            {	
                REWD(inst,i,op);
                i++;
            }
            else if(!strcmp(s,"ADSD"))
            {	
                ADSD(inst,i,op);
                i++;
            } 
            else if(!strcmp(s,"MOVD"))
            {	
                MOVD(inst,i,op);
                i++;
            }        
            
        }
        else if(check_5_bit(inst,s))
        {
        	
            if(!strcmp(s,"VOLU"))
            {
                
                VOLU(inst,i,op);
                i++;
            }
            else if(!strcmp(s,"VOLL"))
            {
                
                VOLL(inst,i,op);
                i++;
            }
            else if(!strcmp(s,"FRW"))
            {
                
                FRW(inst,i,op);
                i++;
            }
            else if(!strcmp(s,"BKW"))
            {
                BKW(inst,i,op);
                i++;
            }else if(!strcmp(s,"PLYD"))
            {
                
                PLYD(inst,i,op);
                i++;
            }else if(!strcmp(s,"LPD"))
            {
                
                LPD(inst,i,op);
                i++;
            }else if(!strcmp(s,"PLYR"))
            {
                
                PLYR(inst,i,op);
                i++;
            }
           
               
        }
        else if(check_8_bit(inst,s))
        {
            if(!strcmp(s,"PAUS"))
            {
              
              PAUSE(i,op);
                i++;
            }
            else if(!strcmp(s,"DCS"))
            {
              
              DCS(i,op);
                i++;
            }
            else if(!strcmp(s,"SFL"))
            {
              
              SFL(i,op);
                i++;
            }else if(!strcmp(s,"MPS"))
            {
              
              MPS(i,op);
                i++;
            }
        }
        else
        {
            printf("Error: Unrecognized Instruction Found!\n");
            i++;
        }
    }

    fclose(op);
}
void print_status_file()
{
    FILE *p=fopen("status.txt","r");
    char s[50];
    while(fgets(s,50,p)!=NULL){puts(s);}
    fclose(p);
}

int main()
{
    //generate hash table for opcodes and registers
    set_song_table();
    gen_hash_table_opcodes();
    gen_hash_table_regsiters();
    //read the machine code to be executed
    store_machine_code();
    //execute the machine code

    reg[7] = 5; // Register which holds total no. of songs
    reg[6] = 0; //Default playlist 
    reg[5] = 2; // Default song when starting first time
    reg[4] = 3; //Default volume is 3.
    int choice;
    while(choice!=5)
    {
        printf("**********Menu*************\n");
        printf("1.Execute the machine code\n");
        printf("2.Print Status of step wise execution\n");
        printf("3. Register Status\n");
        printf("4. print mem\n");
        printf("5.Exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            {
                //executes the code
                solve_machine_code();
                break;
            }
        case 2:
            {
                //print "status.txt"
                print_status_file();
                break;
            }
            case 3:
            {
            	final_register_staus();

				break;
				
            }
            case 4:
            {
                
                print_mem();
                break;
            }
        
        }
    }

    return 0;
}
