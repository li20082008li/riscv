#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cstdlib>
#include<cmath>
#include<ctime>
#include<queue>
#include<map>
#define ll long long
#define ull unsigned long long
#define uint unsigned int
using namespace std;
int i,j,k,m,n,s,t;
uint a[40],pc,opc,fc3,fc7;
uint mem[401000];
char ch[101000];          //////shuzudaxiao!!!!!
uint now;
uint c[1001000];
struct dat{
	uint rd,rs1,rs2,rd_d,rs1_d,rs2_d,I_im,S_im,B_im,U_im,J_im;
}p;
uint zh(char c)
{
	if (c>='0'&&c<='9') return c-'0';
	return c-'A'+10;
}
uint getbit(uint now,uint l,uint r)
{
	r=r-l+1;
	now>>=l;
	return now&((1<<r)-1);
}
void pre(uint now)
{
	opc=getbit(now,0,6);
	fc3=getbit(now,12,14);
	fc7=getbit(now,25,31);
    p.rd=(now>>7)&0x1f;
    p.rs1=(now>>15)&0x1f;
    p.rs2=(now>>20)&0x1f;
    p.rd_d=a[p.rd];
    p.rs1_d=a[p.rs1];
    p.rs2_d=a[p.rs2];
    p.I_im=((now>>20)&0x7ff)|(((int)now<0)?0xfffff800:0);
    p.S_im=((now>>20)&0x7e0)|((now>>7)&0x1f)|(((int)now<0)?0xfffff800:0);
    p.B_im=((now>>20)&0x7e0)|((now>>7)&0x1e)|((now<<4)&0x800)|(((int)now<0)?0xfffff000:0);
    p.U_im=(now&0xfffff000);
    p.J_im=((now>>20)&0x7fe)|((now>>9)&0x800)|(now&0xff000)|(((int)now<0)?(0xfff00000):0);	
}
void branches()
{
    switch(fc3){
    case 0b000:if ((uint)p.rs1_d==(uint)p.rs2_d) pc+=p.B_im-4;break;
    case 0b001:if ((uint)p.rs1_d!=(uint)p.rs2_d) pc+=p.B_im-4;break;
    case 0b100:if ((int)p.rs1_d<(int)p.rs2_d) pc+=p.B_im-4;break;
    case 0b101:if ((int)p.rs1_d>=(int)p.rs2_d) pc+=p.B_im-4;break;
    case 0b110:if ((uint)p.rs1_d<(uint)p.rs2_d) pc+=p.B_im-4;break;
    case 0b111:if ((uint)p.rs1_d>=(uint)p.rs2_d) pc+=p.B_im-4;break;
    }	
}
void cal()                              /////1:0x3f??  2:sltiu??int???
{
    if (fc3==0b000) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d+(uint)p.rs2_d;        //op_ADD
    if (fc3==0b000) if (fc7==0b0100000) a[p.rd]=(uint)p.rs1_d-(uint)p.rs2_d;        //op_SUB
    if (fc3==0b001) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d<<(uint)(p.rs2_d&0x1f); //op_SLL
	if (fc3==0b010) if (fc7==0b0000000) a[p.rd]=(int)p.rs1_d<(int)p.rs2_d;        //op_SLT
	if (fc3==0b011) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d<(uint)p.rs2_d;        //op_SLTU
	if (fc3==0b100) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d^(uint)p.rs2_d;        //op_XOR
	if (fc3==0b101) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d>>(uint)(p.rs2_d&0x1f); //op_SRL
	if (fc3==0b101) if (fc7==0b0100000) a[p.rd]=(int)p.rs1_d>>(uint)(p.rs2_d&0x1f); //op_SRA
	if (fc3==0b110) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d|(uint)p.rs2_d;        //op_OR
	if (fc3==0b111) if (fc7==0b0000000) a[p.rd]=(uint)p.rs1_d&(uint)p.rs2_d;        //op_AND
}
void cali()
{
    if(fc3==0b000) a[p.rd]=(uint)p.rs1_d+(uint)p.I_im;                                           //op_ADDI
    if(fc3==0b010) a[p.rd]=(int)p.rs1_d<(int)p.I_im;                                           //op_SLTI
    if(fc3==0b011) a[p.rd]=(uint)p.rs1_d<(uint)p.I_im;                                           //op_SLTIU
    if(fc3==0b100) a[p.rd]=(uint)p.rs1_d^(uint)p.I_im;                                           //op_XORI
    if(fc3==0b110) a[p.rd]=(uint)p.rs1_d|(uint)p.I_im;                                           //op_ORI
    if(fc3==0b111) a[p.rd]=(uint)p.rs1_d&(uint)p.I_im;                                           //op_ANDI
    if(fc3==0b001) if(fc7==0b0000000) a[p.rd]=(uint)p.rs1_d<<(uint)(p.I_im&0x1f); //op_SLLI
    if(fc3==0b101) if(fc7==0b0000000) a[p.rd]=(uint)p.rs1_d>>(uint)(p.I_im&0x1f); //op_SRLI
    if(fc3==0b101) if(fc7==0b0100000) a[p.rd]=(int)p.rs1_d>>(uint)(p.I_im&0x1f); //op_SRAI
}
void lui()
{
    a[p.rd]=p.U_im;
}
void auipc()
{
    a[p.rd]=pc-4+p.U_im;	
}
void jal()
{
    a[p.rd]=pc;
    pc+=p.J_im-4;            
}
void jalr()
{
    a[p.rd]=pc;
    pc=p.rs1_d+p.I_im;
}
void load()
{
    switch(fc3){
    case 0b000:a[p.rd]=(mem[p.rs1_d+p.I_im]&0xff)|((mem[p.rs1_d+p.I_im]&0x80)?~0xff:0);break;
    case 0b001:a[p.rd]=(mem[p.rs1_d+p.I_im]&0xffff)|((mem[p.rs1_d+p.I_im]&0x8000)?~0xffff:0);break;
    case 0b010:a[p.rd]=mem[p.rs1_d+p.I_im];break;
    case 0b100:a[p.rd]=(mem[p.rs1_d+p.I_im]&0xff);break;
    case 0b101:a[p.rd]=(mem[p.rs1_d+p.I_im]&0xffff);break;
    }
    //cout<<p.rs1_d+p.I_im<<endl;  ////
}
void store()
{
    switch(fc3){
    case 0b000:mem[p.rs1_d+p.S_im]=(p.rs2_d&0xff);break;
    case 0b001:mem[p.rs1_d+p.S_im]=(p.rs2_d&0xffff);break;
    case 0b010:mem[p.rs1_d+p.S_im]=p.rs2_d;break;
    }
    //cout<<p.rs1_d+p.S_im<<" "<<mem[p.rs1_d+p.S_im]<<endl; /////
}
void print()
{
	printf("rd %u pc %u\n",p.rd,pc);
	for (int i=0;i<=31;i++)
	  printf("%u ",a[i]);
	puts("");
}
int main()
{
	//freopen("a.data","r",stdin);
	int tp=0;
	while (scanf("%s",ch+1)!=EOF)
	{
		if (ch[1]=='@')
		{
			s=strlen(ch+1);
			tp=0;
			for (i=2;i<=s;i++)
			  tp=tp*16+zh(ch[i]);
			continue;
		}
		c[tp]=zh(ch[1])*16+zh(ch[2]);
		tp++;
	}
	while (1)
	{
		now=0;
		for (int i=1;i<=4;i++)
		    now=now*256+c[pc+4-i];		
		pc+=4;
		if (now==0xc68223) {cout<<((uint)a[10]&255u);return 0;}
		//cout<<((uint)a[10]&255u)<<endl;
		//printf("%lld\n",now);
		pre(now);
		switch(opc)
		{
			case 0b1100011:branches();break;
            case 0b0110111:lui();break;
            case 0b0010111:auipc();break;
            case 0b1101111:jal();break;
            case 0b1100111:if (fc3==0b000) jalr();break;
            case 0b0110011:cal();break;
            case 0b0010011:cali();break;
            case 0b0000011:load();break;
            case 0b0100011:store();break;
		}
		//print();
		a[0]=0;
	}
}
