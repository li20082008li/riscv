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
uint i,j,k,m,n,s,t;
uint a[40],pc;
char ch[101000];        
uint c[1001000],b[101000];
int bo1,bo2[10];
int jsq;
struct dat{
	uint rd,rs1,rs2,I_im,S_im,B_im,U_im,J_im,opc,fc3,fc7;
    void pre(uint now)
	{
	  opc=now&0x7f;
	  fc3=(now>>12)&0x7;
	  fc7=(now>>25)&0x7f;
      rd=(now>>7)&0x1f;
      rs1=(now>>15)&0x1f;
      rs2=(now>>20)&0x1f;
      I_im=((now>>20)&0x7ff)|(((int)now<0)?0xfffff800:0);
      S_im=((now>>20)&0x7e0)|((now>>7)&0x1f)|(((int)now<0)?0xfffff800:0);
      B_im=((now>>20)&0x7e0)|((now>>7)&0x1e)|((now<<4)&0x800)|(((int)now<0)?0xfffff000:0);
      U_im=(now&0xfffff000);
      J_im=((now>>20)&0x7fe)|((now>>9)&0x800)|(now&0xff000)|(((int)now<0)?(0xfff00000):0);	
	}
};
struct IFID{
	uint IR,NPC;
}IFID;
struct IDEX{
	uint IR,NPC,A,B,IMM,JP;
	dat p;
}IDEX;
struct EXMEM{
	uint IR,NPC,B,cond,ALU,cnt;
	dat p;
}EXMEM;
struct MEMWB{
	uint IR,NPC,ALU,LMD;
	dat p;
}MEMWB;
uint zh(char c)
{
	if (c>='0'&&c<='9') return c-'0';
	return c-'A'+10;
}
void branches()
{
	dat p=IDEX.p;
	uint fc3=p.fc3,fc7=p.fc7;
	EXMEM.ALU=IDEX.p.B_im+IDEX.NPC-4;
    switch(fc3)
	{
    case 0b000:if ((uint)IDEX.A==(uint)IDEX.B) EXMEM.cond=1; else EXMEM.cond=0;break;
    case 0b001:if ((uint)IDEX.A!=(uint)IDEX.B) EXMEM.cond=1; else EXMEM.cond=0;break;
    case 0b100:if ((int)IDEX.A<(int)IDEX.B) EXMEM.cond=1; else EXMEM.cond=0;break;
    case 0b101:if ((int)IDEX.A>=(int)IDEX.B) EXMEM.cond=1; else EXMEM.cond=0;break;
    case 0b110:if ((uint)IDEX.A<(uint)IDEX.B) EXMEM.cond=1; else EXMEM.cond=0;break;
    case 0b111:if ((uint)IDEX.A>=(uint)IDEX.B) EXMEM.cond=1; else EXMEM.cond=0;break;
    }	
}
void cal()                            
{
	dat p=IDEX.p;
	uint fc3=p.fc3,fc7=p.fc7;
    if (fc3==0b000) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A+(uint)IDEX.B;       
    if (fc3==0b000) if (fc7==0b0100000) EXMEM.ALU=(uint)IDEX.A-(uint)IDEX.B;        
    if (fc3==0b001) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A<<(uint)(IDEX.B&0x1f); 
	if (fc3==0b010) if (fc7==0b0000000) EXMEM.ALU=(int)IDEX.A<(int)IDEX.B;        
	if (fc3==0b011) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A<(uint)IDEX.B;        
	if (fc3==0b100) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A^(uint)IDEX.B;        
	if (fc3==0b101) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A>>(uint)(IDEX.B&0x1f);
	if (fc3==0b101) if (fc7==0b0100000) EXMEM.ALU=(int)IDEX.A>>(uint)(IDEX.B&0x1f); 
	if (fc3==0b110) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A|(uint)IDEX.B;        
	if (fc3==0b111) if (fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A&(uint)IDEX.B;       
}
void cali()
{
	dat p=IDEX.p;
	uint fc3=p.fc3,fc7=p.fc7;
    if(fc3==0b000) EXMEM.ALU=(uint)IDEX.A+(uint)p.I_im;                                          
    if(fc3==0b010) EXMEM.ALU=(int)IDEX.A<(int)p.I_im;                                          
    if(fc3==0b011) EXMEM.ALU=(uint)IDEX.A<(uint)p.I_im;                                           
    if(fc3==0b100) EXMEM.ALU=(uint)IDEX.A^(uint)p.I_im;                                           
    if(fc3==0b110) EXMEM.ALU=(uint)IDEX.A|(uint)p.I_im;                                           
    if(fc3==0b111) EXMEM.ALU=(uint)IDEX.A&(uint)p.I_im;                                         
    if(fc3==0b001) if(fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A<<(uint)(p.I_im&0x1f); 
    if(fc3==0b101) if(fc7==0b0000000) EXMEM.ALU=(uint)IDEX.A>>(uint)(p.I_im&0x1f); 
    if(fc3==0b101) if(fc7==0b0100000) EXMEM.ALU=(int)IDEX.A>>(uint)(p.I_im&0x1f); 
}
void lui()
{
    EXMEM.ALU=IDEX.p.U_im;
}
void auipc()
{
    EXMEM.ALU=IDEX.NPC-4+IDEX.p.U_im;	
}
void jal()
{
	EXMEM.ALU=IDEX.p.J_im+IDEX.NPC-4;
	EXMEM.cond=1;           
}
void jalr()
{
    EXMEM.ALU=IDEX.A+IDEX.p.I_im;
	EXMEM.cond=1;  
}
void load1()
{
	EXMEM.ALU=IDEX.A+IDEX.p.I_im;
	EXMEM.B=IDEX.B;
}
void store1()
{
	EXMEM.ALU=IDEX.A+IDEX.p.S_im;
	EXMEM.B=IDEX.B;	
}
void load()
{
	uint s;
    switch(EXMEM.p.fc3)
	{
    case 0b000:MEMWB.LMD=(c[EXMEM.ALU]&0xff)|((c[EXMEM.ALU]&0x80)?~0xff:0);break;
    case 0b001:s=c[EXMEM.ALU]+c[EXMEM.ALU+1]*0x100;MEMWB.LMD=(s&0xffff)|((s&0x8000)?~0xffff:0);break;
    case 0b010:MEMWB.LMD=c[EXMEM.ALU]+c[EXMEM.ALU+1]*0x100+c[EXMEM.ALU+2]*0x10000+c[EXMEM.ALU+3]*0x1000000;break;
    case 0b100:MEMWB.LMD=c[EXMEM.ALU];break;
    case 0b101:MEMWB.LMD=c[EXMEM.ALU]+c[EXMEM.ALU+1]*0x100;break;
    }
}
void store()
{
    switch(EXMEM.p.fc3)
	{
    case 0b000:c[EXMEM.ALU]=(EXMEM.B&0xff);break;
    case 0b001:c[EXMEM.ALU]=(EXMEM.B&0xff);c[EXMEM.ALU+1]=(EXMEM.B&0xff00)>>8;break;
    case 0b010:c[EXMEM.ALU]=(EXMEM.B&0xff);c[EXMEM.ALU+1]=(EXMEM.B&0xff00)>>8;c[EXMEM.ALU+2]=(EXMEM.B&0xff0000)>>16;c[EXMEM.ALU+3]=(EXMEM.B&0xff000000)>>24;break;
    }
}
void print()
{
	jsq++;
    ///if (jsq>=38110&&jsq<=38120)
	{
	printf("%u\n",MEMWB.IR);
	//printf("rs1 %u rs2 %u pc %u\n",p.rs1,p.rs2,pc-4);
	for (int i=0;i<=31;i++)
	  printf("%u ",a[i]);
	puts("");
}
}
bool ck(uint now)
{
	if (now==0b1101111||now==0b1100111||now==0b1100011) return 1;
	return 0;
}
bool ck2(uint now)
{
	if (now==0b0110111||now==0b0010111||now==0b0010011||now==0b0110011) return 1;
	return 0;
}
bool predict(uint pc)
{
	pc=pc&0xfff;
	if (b[pc]>=0b10) return 1;
	else return 0;
}
void update(uint pc,int s)
{
	pc=pc&0xfff;
	if (s)
	{
		switch (b[pc])
		{
			case 0b00:b[pc]=0b01;break;
			case 0b01:b[pc]=0b11;break;
			case 0b10:b[pc]=0b11;break;
			case 0b11:b[pc]=0b11;break;
		}
	} else
	{
		switch (b[pc])
		{
			case 0b00:b[pc]=0b00;break;
			case 0b01:b[pc]=0b00;break;
			case 0b10:b[pc]=0b00;break;
			case 0b11:b[pc]=0b10;break;
		}
	}
}
void IF()
{
	if (bo1) return;
	if (bo2[0]) return;
	if (EXMEM.cnt!=0) return;
	uint now=0;
	for (int i=1;i<=4;i++)
		now=now*256+c[pc+4-i];		
	IFID.IR=now;
    if (IFID.IR==0xc68223) {bo2[0]=1;return;}
    if (EXMEM.IR&&EXMEM.p.opc==0b1100011&&EXMEM.cond!=predict(EXMEM.NPC-4))
	{
		if (EXMEM.cond==1)
		  pc=EXMEM.ALU;
		else
		  pc=EXMEM.NPC;
    	IFID.IR=0;
    	IDEX.IR=0;		
	} 
	else if (IDEX.IR&&(IDEX.p.opc==0b1101111||IDEX.p.opc==0b1100111||(IDEX.p.opc==0b1100011&&predict(IDEX.NPC-4))))
	{
		pc=IDEX.JP;
		IFID.IR=0;
    } else
	    pc+=4;
    if (EXMEM.IR&&EXMEM.p.opc==0b1100011)
      	update(EXMEM.NPC-4,EXMEM.cond);
	IFID.NPC=pc;
}
void ID()
{
	if (bo2[1]) return;
	if (EXMEM.cnt!=0) return;
	IDEX.IR=IFID.IR;
    if (IFID.IR==0xc68223) {bo2[1]=1;return;}
    if (IFID.IR==0) return;
	IDEX.p.pre(IDEX.IR);
    IDEX.NPC=IFID.NPC;
    if (EXMEM.IR&&ck2(EXMEM.p.opc)&&EXMEM.p.rd==IDEX.p.rs1)
      IDEX.A=EXMEM.ALU;
    else if (EXMEM.IR&&EXMEM.p.opc==0b0000011&&EXMEM.p.rd==IDEX.p.rs1)
    {
    	IDEX.IR=0;
    	bo1=1;
    	return;
    } 
    else if (MEMWB.IR&&ck2(MEMWB.p.opc)&&MEMWB.p.rd==IDEX.p.rs1)
      IDEX.A=MEMWB.ALU;
    else if (MEMWB.IR&&MEMWB.p.opc==0b0000011&&MEMWB.p.rd==IDEX.p.rs1)
      IDEX.A=MEMWB.LMD;
    else
      IDEX.A=a[IDEX.p.rs1];      
    if (EXMEM.IR&&ck2(EXMEM.p.opc)&&EXMEM.p.rd==IDEX.p.rs2)
      IDEX.B=EXMEM.ALU;
    else if (EXMEM.IR&&EXMEM.p.opc==0b0000011&&EXMEM.p.rd==IDEX.p.rs2)
    {
    	IDEX.IR=0;
    	bo1=1;
    	return;
    } 
    else if (MEMWB.IR&&ck2(MEMWB.p.opc)&&MEMWB.p.rd==IDEX.p.rs2)
      IDEX.B=MEMWB.ALU;
    else if (MEMWB.IR&&MEMWB.p.opc==0b0000011&&MEMWB.p.rd==IDEX.p.rs2)
      IDEX.B=MEMWB.LMD;
    else
      IDEX.B=a[IDEX.p.rs2];  
    if (IDEX.p.opc==0b1101111)
      IDEX.JP=IDEX.p.J_im+IDEX.NPC-4;
    else if (IDEX.p.opc==0b1100111)
	  IDEX.JP=IDEX.A+IDEX.p.I_im;
	else IDEX.JP=IDEX.p.B_im+IDEX.NPC-4;
    IFID.IR=0;
}
void EX()
{
	if (bo2[2]) return;
	if (EXMEM.cnt!=0) return;
	EXMEM.IR=IDEX.IR;
	EXMEM.p=IDEX.p;
    if (IDEX.IR==0xc68223) {bo2[2]=1;return;}
    if (IDEX.IR==0) return;
	switch(IDEX.p.opc)
	{
		case 0b1100011:branches();break;
        case 0b0110111:lui();break;
        case 0b0010111:auipc();break;
        case 0b1101111:jal();break;
        case 0b1100111:if (IDEX.p.fc3==0b000) jalr();break;
        case 0b0110011:cal();break;
        case 0b0010011:cali();break;
        case 0b0000011:load1();break;
        case 0b0100011:store1();break;
	}
	EXMEM.NPC=IDEX.NPC;
	IDEX.IR=0;
}
void MEM()
{
	if (bo2[3]) return;
	MEMWB.IR=EXMEM.IR;
	MEMWB.p=EXMEM.p;
    if (EXMEM.IR==0xc68223) {bo2[3]=1;return;}
    if (EXMEM.IR==0) return;
	if (EXMEM.p.opc==0b0000011||EXMEM.p.opc==0b0100011)
	{
		EXMEM.cnt--;
		if (EXMEM.cnt!=0)
		{
			MEMWB.IR=0;
			if ((int)EXMEM.cnt<0)
			  EXMEM.cnt=2;
			return;
		}
    	if (EXMEM.p.opc==0b0000011) load();
    	if (EXMEM.p.opc==0b0100011) store();
    }
    MEMWB.ALU=EXMEM.ALU; 
    MEMWB.NPC=EXMEM.NPC;
    EXMEM.IR=0;
}
void WB()
{
	if (bo2[4]) return;
    if (MEMWB.IR==0xc68223) {bo2[4]=1;return;}
    if (MEMWB.IR==0) return;
    if (ck2(MEMWB.p.opc))
      a[MEMWB.p.rd]=MEMWB.ALU;      
    if (MEMWB.p.opc==0b0000011)
      a[MEMWB.p.rd]=MEMWB.LMD;
    if (MEMWB.p.opc==0b1101111||MEMWB.p.opc==0b1100111)
      a[MEMWB.p.rd]=MEMWB.NPC;
	a[0]=0;
	//print();
    MEMWB.IR=0;
}
int main()
{
	//freopen("a.data","r",stdin);
	//freopen("1.out","w",stdout);
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
		bo1=0;
		WB(); if (MEMWB.IR==0xc68223) {cout<<((uint)a[10]&255u);return 0;}
		MEM();
		EX();
		ID();
		IF();
	}
}
