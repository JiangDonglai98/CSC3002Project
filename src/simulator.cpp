#include<cstdio>
#include<iostream>
#include<cstring>
#include<map> 
#include<vector>
#include<cstdlib>
#include<fstream>
#include<malloc.h>
#include<vector>
#include "pj1.cpp"
using namespace std;


const int text_address = 0x00400000;
const int data_address = 0x00500000;
map<string,string> Rm,Im,Jm;
const int memory_size = 6 * 1024 * 1024 * sizeof(char);
const int reg_size = 34 * sizeof(int);

void *reg,*start;
int datanum;
vector<string> MIPS;
vector<string> dataset;

void pre()
{
	for(register int i=0;i<32;++i) Rmap.insert(pair<string,string>(Rcode[i],Rname[i])),Reg.insert(pair<string,string>(Regcode[i],Regname[i]));
	for(register int i=0;i<36;++i) Imap.insert(pair<string,string>(Icode[i],Iname[i]));
	for(register int i=0;i<2;++i) Jmap.insert(pair<string,string>(Jcode[i],Jname[i]));
}

int binToInt(string a)
{
	int ans=0;
    for(register int i=0;i<a.size();i++) ans=(ans<<1)+(int(a[i])-48);
    return ans;
}

void Rsolve(string func,int rs,int rt,int rd,string sa)
{
	if(func=="sll")
	{
		void* rtad=(void*)((long long)(reg)+rt*sizeof(int));
    	void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	*(int*)rdad=*(int*)rtad<<binToInt(sa);
	}
	else if(func=="srl")
	{
		void* rtad=(void*)((long long)(reg)+rt*sizeof(int));
    	void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	*(int*)rdad=abs(*(int*)rtad>>binToInt(sa));
	}
	else if(func=="sra")
	{
		void* rtad=(void*)((long long)(reg)+rt*sizeof(int));
    	void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	*(int*)rdad=*(int*)rtad>>binToInt(sa);
	}
	else if(func=="sllv")
	{
		void* rsad=(void*)((long long)(reg)+rs*sizeof(int));
    	void* rtad=(void*)((long long)(reg)+rt*sizeof(int));
    	void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	*(int*)rdad=*(int*)rtad<<*(int*)rsad;
	}
	else if(func=="srlv")
	{
		void* rsad=(void*)((long long)(reg)+rs*sizeof(int));
		void* rtad=(void*)((long long)(reg)+rt*sizeof(int));
    	void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	*(int*)rdad=*(int*)rtad<<*(int*)rsad;
	}
	else if(func=="srav")
	{
		void* rsad=(void*)((long long)(reg)+rs*sizeof(int));
    	void* rtad=(void*)((long long)(reg)+rt*sizeof(int));
    	void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	*(int*)rdad=*(int*)rtad>>*(int*)rsad;
	}
	else if(func=="jr")
	{
		
	}
	else if(func=="jalr")
	{
		
	}
	else if(func=="syscall")
	{
		
	}
	else if(func=="mfhi")
	{
		void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
		void* hi=(void*)((long long)(reg)+32*sizeof(int));
    	*(int*)rdad=*(int*)(hi);
	}
	else if(func=="mflo")
	{
		void* rdad=(void*)((long long)(reg)+rd*sizeof(int));
    	void* lo=(void*)((long long)(reg)+33*sizeof(int));
    	*(int*)rdad=*(int*)(lo);
	}
	else if(func=="mtlo")
	{
		void* rsad=(void*)((long long)(reg)+rs*sizeof(int));
    	void* lo=(void*)((long long)(reg)+33*sizeof(int));
    	*(int*)(lo)=*(int*)rsad;
	}
	else if(func=="mult")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* hi = (void*)((long long)(reg) + 32 * sizeof(int));
    	void* lo = (void*)((long long)(reg) + 33 * sizeof(int));
    	long long res = (long long)((*(int*)rsad)) * (long long)(*(int*)rtad);
    	*(unsigned int*)hi = (unsigned int)(abs(res >> 32));
    	*(unsigned int*)lo = (unsigned int)(res & 4294967295);
	}
	else if(func=="multu")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* hi = (void*)((long long)(reg) + 32 * sizeof(int));
    	void* lo = (void*)((long long)(reg) + 33 * sizeof(int));
    	unsigned long long res = (unsigned long long)((*(int*)rsad)) * (unsigned long long)(*(int*)rtad);
    	*(unsigned int*)hi = (unsigned int)(res >> 32);
    	*(unsigned int*)lo = (unsigned int)res & 4294967295;
	}
	else if(func=="div")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* hi = (void*)((long long)(reg) + 32 * sizeof(int));
    	void* lo = (void*)((long long)(reg) + 33 * sizeof(int));
    	*(unsigned int*)lo = *(unsigned int*)rsad / *(unsigned int*)rtad;
    	*(unsigned int*)hi = *(unsigned int*)rsad % *(unsigned int*)rtad;
	}
	else if(func=="divu")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* hi = (void*)((long long)(reg) + 32 * sizeof(int));
    	void* lo = (void*)((long long)(reg) + 33 * sizeof(int));
    	*(unsigned int*)lo = *(unsigned int*)rsad / *(unsigned int*)rtad;
    	*(unsigned int*)hi = *(unsigned int*)rsad % *(unsigned int*)rtad;
	}
	else if(func=="add")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	if ((*(int*)rsad > 0 && *(int*)rtad > 0 && *(int*)rsad + *(int*)rtad < 0) || (*(int*)rsad < 0 && *(int*)rtad < 0 && *(int*)rsad + *(int*)rtad > 0))
    	{
    	    cout << "Overflow Exception";
    	    exit(0);
    	}
    	*(int*)rdad = *(int*)rsad + *(int*)rtad;
	}
	else if(func=="addu")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	*(unsigned int*)rdad = *(unsigned int*)rsad + *(unsigned int*)rtad;
	}
	else if(func=="sub")
	{
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	if ((*(int*)rsad < 0 && *(int*)rtad > 0 && *(int*)rsad - *(int*)rtad > 0) || (*(int*)rsad > 0 && *(int*)rtad < 0 && *(int*)rsad - *(int*)rtad < 0))
    	{
    	    cout << "Overflow Exception";
    	    exit(0);
    	}
    	*(int*)rdad = *(int*)rsad - *(int*)rtad;
	}
	else if(func=="subu")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	*(unsigned int*)rdad = *(unsigned int*)rsad - *(unsigned int*)rtad;
	}
	else if(func=="and")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	*(int*)rdad = *(int*)rsad & *(int*)rtad;
	}
	else if(func=="or")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	*(int*)rdad = *(int*)rsad | *(int*)rtad;
	}
	else if(func=="xor")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	*(int*)rdad = *(int*)rsad ^ *(int*)rtad;
	}
	else if(func=="nor")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	*(int*)rdad = !(*(int*)rsad | *(int*)rtad);
	}
	else if(func=="slt")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	if (*(int*)rtad < *(int*)rsad) *(int*)rdad = 1;
    	else *(int*)rdad = 0;
	}
	else if(func=="sltu")
	{
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    	if (*(unsigned int*)rtad < *(unsigned int*)rsad) *(unsigned int*)rdad = 1;
    	else *(unsigned int*)rdad = 0;
	}
}

int Isolve(string func,int rs,int rt,string immediate)
{
	int s=0;
	if(func=="beq")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	if (*(int*)rsad == *(int*)rtad)
        	return offset;
    	else
        	return 0;
	}
	else if(func=="bne")
	{
		int offset = binToInt(immediate.substr(1, 15));
    	if (immediate[0] == '1')
        	offset *= -1;
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	if (*(int*)rsad != *(int*)rtad)
        	return offset;
    	else
        	return 0;
	}
	else if(func=="blez")
	{
		int offset = binToInt(immediate.substr(1, 15));
    	if (immediate[0] == '1')
        	offset *= -1;
        void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	if (*(int*)rsad <= 0)
    	    return offset;
    	else
        	return 0;
	}
	else if(func=="bgtz")
	{
		int offset = binToInt(immediate.substr(1, 15));
    	if (immediate[0] == '1')
        	offset *= -1;
		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	if (*(int*)rsad > 0)
        	return offset;
    	else
        	return 0;
	}
	else if(func=="addi")
	{
		int imm = binToInt(immediate.substr(1, 15));
    	if (immediate[0] == '1')
        	imm *= -1;
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	if ((*(int*)rsad > 0 && imm > 0 && *(int*)rsad + imm < 0) || (*(int*)rsad < 0 && imm < 0 && *(int*)rsad + imm > 0))
    	{
        	cout << "Overflow Exception";
        	exit(0);
    	}
    	*(int*)rtad = *(int*)rsad + imm;
	}
	else if(func=="addiu")
	{
		unsigned int imm = binToInt(immediate);
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(unsigned int*)rtad = *(unsigned int*)rsad + imm;
	}
	else if(func=="slti")
	{
		int imm = binToInt(immediate);
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	if (*(int*)rsad < imm)
        	*(int*)rtad = 1;
    	else
        	*(int*)rtad = 0;
	}
	else if(func=="sltiu")
	{
		int imm = binToInt(immediate);
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	if (*(unsigned int*)rsad < imm)
        	*(unsigned int*)rtad = 1;
    	else
        	*(unsigned int*)rtad = 0;
	}
	else if(func=="andi")
	{
		int imm = binToInt(immediate);
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)rtad = *(int*)rsad & imm;
	}
	else if(func=="ori")
	{
		int imm = binToInt(immediate);
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)rtad = *(int*)rsad | imm;
	}
	else if(func=="xori")
	{
		int imm = binToInt(immediate);
    	void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)rtad = *(int*)rsad ^ imm;
	}
	else if(func=="lui")
	{
		int imm = binToInt(immediate.substr(1, 15));
    	if (immediate[0] == '1')
        	imm *= -1;
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)rtad = imm << 16;
	}
	else if(func=="lb")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
    	void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	memcpy((void*)((long long)(rtad) + 3 * sizeof(char)), *rsad, sizeof(char));
	}
	else if(func=="lh")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
    	void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	memcpy((void*)((long long)(rtad) + 2 * sizeof(char)), *rsad, 2 * sizeof(char));
	}
	else if(func=="lw")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
        void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)rtad = *(int*)((long long)(*(int**)rsad) + (long long)(start)- text_address);
	}
	else if(func=="lbu")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
        void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)rtad = **(unsigned short**)rsad;
    	memcpy((void*)((long long)(rtad) + 3 * sizeof(char)), *rsad, sizeof(char));
	}
	else if(func=="lhu")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
		void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	memcpy((void*)((long long)(rtad) + 2 * sizeof(char)), *rsad, 2 * sizeof(char));
	}
	else if(func=="sb")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
		void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	memcpy((int*)((long long)(*(int**)rsad) + (long long)(start) - text_address), (void*)((long long)(rtad) + 3 * sizeof(char)), sizeof(char));
	}
	else if(func=="sw")
	{
		int offset = binToInt(immediate.substr(1,15));
    	if (immediate[0] == '1')
        	offset *= -1;
		void** rsad = (void**)((long long)(reg) + rs * sizeof(int) + offset * sizeof(char));
    	void* rtad = (void*)((long long)(reg) + rt * sizeof(int));
    	*(int*)((long long)(*(int**)rsad) + (long long)(start) - text_address) = *(int*)rtad;
	}
}

int Jsolve(string mips, void* head)
{
    string func_code = mips.substr(0, 6);
    string func_code2 = mips.substr(26, 6);
    int s = 0, func = binToInt(func_code), func2 = binToInt(func_code2);
   
    if(func==0)
    {
        if (func2 == 9)
        {
        	int rs = binToInt(mips.substr(6, 5));
    		int rd = binToInt(mips.substr(16, 5));
    		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    		void* rdad = (void*)((long long)(reg) + rd * sizeof(int));
    		*(int*)rdad = (long long)(head) - (long long)(start) + text_address + sizeof(int);
    		s = (*(int*)(rsad) - text_address) / sizeof(int);
		}
        if (func2 == 8)
        {
        	int rs = binToInt(mips.substr(6, 5));
    		void* rsad = (void*)((long long)(reg) + rs * sizeof(int));
    		cout << "s:" << (*(int*)(rsad)-text_address) / sizeof(int) << "\n";;
    		s=(*(int*)(rsad)-text_address) / sizeof(int);
		}
	}
    return s;
}

string judge(string mips)
{
	int len=mips.length();
	if (mips=="00000000000000000000000000001100") return "Stype";
	if(mips.substr(len-6,6)=="000000")
	{
		string b=mips.substr(len-6,6);
		if(b=="001001"||b=="001000") return "Jtype";
		else return "Rtype";
	}
	else
	{
		if(mips.substr(len-6,6)=="000000"||mips.substr(len-6,6)=="000011") return "Jtype";
		else return "Itype";
	}
}



void work(void *head)
{
	string now,op,func,immediate,sa;
	int rs,rt,rd,s;
	for(register int i=0;i<number;++i)
	{
		//now=machine_code
		
		if(judge(now)=="Rtype")
		{
			rs=binToInt(now.substr(6,5));
			rt=binToInt(now.substr(11,5));
			rd=binToInt(now.substr(16,5));
			sa=now.substr(21,5);
			func=Rmap[now.substr(26,6)];
			Rsolve(func,rs,rt,rd,sa);
		}
		else if(judge(now)=="Itype")
		{
			op=Imap[now.substr(0,6)];
			rs=binToInt(now.substr(6,5));
			rt=binToInt(now.substr(11,5));
			immediate=now.substr(16,16);
			s = Isolve(op,rs,rt,immediate);
            head = (void*)((long long)(head) + s * sizeof(int));
		}
		else if(judge(now)=="Jtype")
		{
			s=Jsolve(now, head);
            head=(void*)((long long)(start) + (s - 1) * sizeof(int));
		}
	}
}

int main()
{
	void* head=malloc(memory_size);
    reg=malloc(reg_size);
    pre();
	work(head);
	free(reg);
	free(head);
	return 0;
}
