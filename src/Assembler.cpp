
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cin;
using std::cout;

// R-type
#define add 0x20
#define addu 0x21
#define And 0x24
#define div 0x1a	// rd = 0
#define divu 0x1b	// rd = 0
#define mult 0x18	// rd = 0
#define multu 0x19	// rd = 0
#define nor 0x27
#define Or 0x25
#define sll 0		// shamt
#define sllv 4
#define sra 3		// shamt
#define srav 7
#define srl 2		// shamt
#define srlv 6
#define sub 0x22
#define subu 0x23
#define Xor 0x26
#define slt 0x2a
#define sltu 0x2b
#define jalr 9		// rt = 0
#define jr	8	    // rd = rt = 0
#define teq 0x34	// rd = 0
#define tne 0x36	// rd = 0
#define tge 0x30	// rd = 0
#define tgeu 0x31	// rd = 0
#define tlt 0x32	// rd = 0
#define tltu 0x33	// rd = 0

#define mfhi 0x10	// rd
#define mflo 0x12	// rd

#define mthi 0x11	// rs
#define mtlo 0x13	// rs

// I-type
#define addi 8
#define addiu 9
#define andi 0xc
#define ori 0xd
#define xori 0xe
#define lui 0xf		// rs=0
#define slti 0xa
#define sltiu 0xb
#define bclf 0x11	// rs=cc	rt=0
#define bclt 0x11	// rs=cc	rt=1
#define beq 4
#define bgez 1		// rt=1
#define bgezal 1		// rt=0x11
#define bgtz 7		//rt= 0
#define blez 6		//rt= 0
#define bltzal 1		//rt= 0x10
#define bltz 1		// rt=0
#define bne 5
#define teqi 1	    // rt = 0xc
#define tnei 1	    // rt = 0xe
#define tgei 1	    // rt = 8
#define tgeiu 1	    // rt = 9
#define tlti 1	    // rt = 0xa
#define tltiu 1	    // rt = 0xb

#define lb 0x20
#define lbu 0x24
#define lh 0x21
#define lhu 0x25
#define lw 0x23
#define lwl 0x22
#define lwr 0x26
#define ll 0x30
#define sb 0x28
#define sh 0x29
#define sw 0x2b
#define swl 0x2a
#define swr 0x2e
#define sc 0x38

// J-type
#define J	2
#define jal 3

namespace assembler {
    std::map<string, int> labels;
    std::map<string, int> RTypes;
    std::map<string, int> ITypes;
    std::map<string, int> JTypes;
    std::map<string, int> rts;
    std::map<string, int> Regs;
    void init() {
	    RTypes["add"] = add;
	    RTypes["addu"] = addu;
	    RTypes["and"] = And;
	    RTypes["div"] = div;
	    RTypes["divu"] = divu;
	    RTypes["mult"] = mult;
	    RTypes["multu"] = multu;
	    RTypes["nor"] = nor;
	    RTypes["or"] = Or;
	    RTypes["sll"] = sll;
	    RTypes["sllv"] = sllv;
	    RTypes["sra"] = sra;
	    RTypes["srav"] = srav;
	    RTypes["srl"] = srl;
	    RTypes["srlv"] = srlv;
	    RTypes["sub"] = sub;
	    RTypes["subu"] = subu;
	    RTypes["xor"] = Xor;
	    RTypes["slt"] = slt;
	    RTypes["sltu"] = sltu;
	    RTypes["jalr"] = jalr;
	    RTypes["jr"] = jr;
	    RTypes["teq"] = teq;
	    RTypes["tne"] = tne;
	    RTypes["tge"] = tge;
	    RTypes["tgeu"] = tgeu;
	    RTypes["tlt"] = tlt;
	    RTypes["tltu"] = tltu;
	    RTypes["mfhi"] = mfhi;
	    RTypes["mflo"] = mflo;
	    RTypes["mthi"] = mthi;
	    RTypes["mtlo"] = mtlo;

	    ITypes["addi"] = addi;
	    ITypes["addiu"] = addiu;
	    ITypes["andi"] = andi;
	    ITypes["ori"] = ori;
	    ITypes["xori"] = xori;
	    ITypes["lui"] = lui;
	    ITypes["slti"] = slti;
	    ITypes["sltiu"] = sltiu;
	    ITypes["bclf"] = bclf;
	    ITypes["bclt"] = bclt;
	    ITypes["beq"] = beq;
	    ITypes["bgez"] = bgez;
	    ITypes["bgezal"] = bgezal;
	    ITypes["bgtz"] = bgtz;
	    ITypes["blez"] = blez;
	    ITypes["bltzal"] = bltzal;
	    ITypes["bltz"] = bltz;
	    ITypes["bne"] = bne;
	    ITypes["teqi"] = teqi;
	    ITypes["tnei"] = tnei;
	    ITypes["tgei"] = tgei;
	    ITypes["tgeiu"] = tgeiu;
	    ITypes["tlti"] = tlti;
	    ITypes["tltiu"] = tltiu;
	    ITypes["lb"] = lb;
	    ITypes["lbu"] = lbu;
	    ITypes["lh"] = lh;
	    ITypes["lhu"] = lhu;
	    ITypes["lw"] = lw;
	    ITypes["lwl"] = lwl;
	    ITypes["lwr"] = lwr;
	    ITypes["ll"] = ll;
	    ITypes["sb"] = sb;
	    ITypes["sh"] = sh;
	    ITypes["sw"] = sw;
	    ITypes["swl"] = swl;
	    ITypes["swr"] = swr;
	    ITypes["sc"] = sc;

	    JTypes["j"] = J;
	    JTypes["jal"] = jal;

	    rts["bclf"] = 0;
	    rts["bclt"] = 1;
	    rts["bgez"] = 1;
	    rts["bgezal"] = 0x11;
	    rts["bltzal"] = 0x10;
	    rts["bltz"] = 0;
	    rts["teqi"] = 0xc;
	    rts["tnei"] = 0xe;
	    rts["tgei"] = 8;
	    rts["tgeiu"] = 9;
	    rts["tlti"] = 0xa;
	    rts["tltiu"] = 0xb;

	    Regs["$ze"] = 0;
	    Regs["$at"] = 1;
	    Regs["$v0"] = 2;
	    Regs["$v1"] = 3;
	    Regs["$a0"] = 4;
	    Regs["$a1"] = 5;
	    Regs["$a2"] = 6;
	    Regs["$a3"] = 7;
	    Regs["$t0"] = 8;
	    Regs["$t1"] = 9;
	    Regs["$t2"] = 10;
	    Regs["$t3"] = 11;
	    Regs["$t4"] = 12;
	    Regs["$t5"] = 13;
	    Regs["$t6"] = 14;
	    Regs["$t7"] = 15;
	    Regs["$s0"] = 16;
	    Regs["$s1"] = 17;
	    Regs["$s2"] = 18;
	    Regs["$s3"] = 19;
	    Regs["$s4"] = 20;
	    Regs["$s5"] = 21;
	    Regs["$s6"] = 22;
	    Regs["$s7"] = 23;
	    Regs["$t8"] = 24;
	    Regs["$t9"] = 25;
	    Regs["$k0"] = 26;
	    Regs["$k1"] = 27;
	    Regs["$gp"] = 28;
	    Regs["$sp"] = 29;
	    Regs["$fp"] = 30;
	    Regs["$ra"] = 31;
    }

    int getOp(const std::string &op) {
		if (RTypes.find(op) != RTypes.end()) {  //  prefer this method, since time complexity is logn and 
			return 0;
		}
		if (ITypes.find(op) != ITypes.end()) {
			return ITypes[op];
		}
		if (JTypes.find(op) != JTypes.end()) {
			return JTypes[op];
		}
		return -1;
	}

	int getFunc(const std::string &op) {
	if (RTypes.find(op) != RTypes.end()) {
			return RTypes[op];
		}
		return -1;
	}

	int getR(std::string &name) {
		name = name.substr(0, 3);
		if (Regs.find(name) == Regs.end()) {
            cout << "Unknown register " << name << std::endl;
			exit(1);
		}
		return Regs[name];
	}

	short getImm(std::string &word, int addr) {
		if (labels.find(word) == labels.end()) {
			std::istringstream ss(word);
			short val;
			ss >> val;
			return val;
		}
		int imm = (short) labels[word];
		imm = imm - (addr + 1);
		return imm;
	}
}

int main(int argc, char **argv) {
	if (argc != 3) {
		cout << "Usage: ./assembler <input file> <output file>" << std::endl;
		return 1;
	}

	ifstream is(argv[1]);
	if (!is) {
		cout << "Can't open " << argv[1] << std::endl;
		return 1;
	}

	ofstream os(argv[2]);
	if (!os) {
		cout << "Can't create " << argv[2] << std::endl;
		return 1;
	}

	assembler::init();

	string line;
	int addr = 0;
	while (getline(is, line)) {
		istringstream ss(line);
		string word;
		if (!(ss >> word)) {
			continue;
		}
		if (word[0] == '#') {
			continue;
		}
		if (word[word.length() - 1] == ':') {
			// label
			word[word.length() - 1] = '\0';
			assembler::labels[word.c_str()] = addr;
			if (!(ss >> word)) {
				continue;
			}
			if (word[0] == '#') {
				continue;
			}
		}
		addr++;
	}

	is.close();
	is.open(argv[1]);
	addr = 0;
	while (getline(is, line)) {
		istringstream ss(line);
		string word;
		if (!(ss >> word)) {
			continue;
		}
		if (word[0] == '#') {
			continue;
		}
//		cout << line << endl;
		if (word[word.length() - 1] == ':') {
			// label
			if (!(ss >> word)) {
				continue;
			}
			if (word[0] == '#') {
				continue;
			}
		}

		int op = assembler::getOp(word);
		if (op == -1) {
			cout << "unknown instruction " << word << std::endl;
			return 1;
		}
		unsigned int inst = op << 26;
		int rs = 0, rt = 0, rd = 0, shamt = 0;

		if (op == 0) {
			// R-Type
			int func = assembler::getFunc(word);
			inst |= func & 0x3f;
			switch (func) {
            case sll:  // logical shift left
            case sra:  // arithmetic shift right
            case srl:  // logical shift right
				ss >> word;
                rd = assembler::getR(word);
				ss >> word;
                rt = assembler::getR(word);
				ss >> shamt;

				break;

            case sllv:  // logical shift left
            case srav:  // arithmetic shift right
            case srlv:  // logical shift right
				ss >> word;
                rd = assembler::getR(word);
				ss >> word;
                rt = assembler::getR(word);
				ss >> word;
                rs = assembler::getR(word);

				break;

            case jalr:  // jump instruction jalr
				ss >> word;
                rs = assembler::getR(word);
				ss >> word;
                rd = assembler::getR(word);
				break;

            case jr:  // jump instruction jr
				ss >> word;
                rs = assembler::getR(word);
				break;

            case teq:  // exception instruction
			case tne:
			case tge:
			case tgeu:
			case tlt:
			case tltu:
            case div:  // signed division
            case divu: // unsigned division
            case mult: // multiply operation
			case multu:
				ss >> word;
                rs = assembler::getR(word);
				ss >> word;
                rt = assembler::getR(word);
				break;

            case mfhi:  // assigns the value of the special register HI to register address rd
            case mflo:  // assigns the value of the special register LO to register address rd
				ss >> word;
                rd = assembler::getR(word);
				break;

            case mthi:  // assigns the value of the register address rs to the special register HI
            case mtlo:  // assigns the value of the register address rs to the special register LO
				ss >> word;
                rs = assembler::getR(word);
				break;

            default:  // other Rtypes
				ss >> word;
                rd = assembler::getR(word);
				ss >> word;
                rs = assembler::getR(word);
				ss >> word;
                rt = assembler::getR(word);
				break;

			}
			inst |= (rs & 0x1f) << 21;
			inst |= (rt & 0x1f) << 16;
			inst |= (rd & 0x1f) << 11;
			inst |= (shamt & 0x1f) << 6;
		} else if (op == J || op == jal) {
			// J-Type
			string target;
			ss >> target;
            if (assembler::labels.find(target) == assembler::labels.end()) {
                cout << "Can't find lable " << target << std::endl;
				return 1;
			}
            inst |= assembler::labels[target];
		} else {
			// I-Type
			int imm = 0;
			char temp;
			switch (op) {
            case 1:  // branch instruction
			case bgtz:
			case blez:
                rt = assembler::rts[word];
				ss >> word;
                rs = assembler::getR(word);
				ss >> word;
                imm = assembler::getImm(word, addr);
				//imm = imm - (addr + 1);
				break;

			case beq:
			case bne:
				ss >> word;
                rs = assembler::getR(word);
				ss >> word;
                rt = assembler::getR(word);
				ss >> word;
                imm = assembler::getImm(word, addr);
//				imm = imm - (addr + 1);
				break;

            case lui:  // 16 bits immediate number is saved to the high 16 bits of the register address rt

            case lb:  // byte loading instruction
            case lbu: // unsigned byte load instruction
            case lh:  // half-word load instruction
            case lhu: // unsigned half-word load instruction
            case lw:  // word loading instruction
			case lwl:
			case lwr:
            case ll:  // load linked
            case sb:  // byte storage instruction
            case sh:  // half-word storage instruction
            case sw:  // word storage instruction
            case swl:
			case swr:
			case sc:
				ss >> word;
                rt = assembler::getR(word);
				ss >> imm;
				ss >> temp;
				ss >> word;
				if (ss) {
					word = word.substr(0, word.length() - 1);
                    rs = assembler::getR(word);
				}
				break;

            default: // other Itypes
				ss >> word;
                rt = assembler::getR(word);
				ss >> word;
                rs = assembler::getR(word);
				ss >> word;
                imm = assembler::getImm(word, addr);
				break;
			}

			inst |= (rs & 0x1f) << 21;
			inst |= (rt & 0x1f) << 16;
			inst |= (imm & 0xffff);
		}
		string bits;
		for (int i = 0; i < 32; i++) {
			if (inst & 1) {
				bits = '1' + bits;
			} else {
				bits = '0' + bits;
			}
			inst = inst >> 1;
		}

        cout << bits << std::endl;
        os << bits << std::endl;
		addr++;

	}
	return 0;
}
