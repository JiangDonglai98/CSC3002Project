#include <bitset> 
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <sstream>

#define MAX_SIZE 10000
#define TEXT_START 0x400000
#define DATA_START 0x10000000

// namespace declare
using namespace std;

// opcode map
unordered_map <string, int> opmap = {{"addu", 0}, {"and", 0}, {"nor", 0}, {"or", 0},
    {"sltu", 0}, {"sll", 0}, {"srl", 0}, {"subu", 0}, {"jr", 0}, {"addiu", 9},
    {"andi", 12}, {"lui", 15}, {"ori", 13}, {"sltiu", 11}, {"beq", 4}, {"bne", 5},
    {"lw", 35}, {"sw", 43}, {"j", 2}, {"jal", 3}, {"la", 99}};

// fn map
unordered_map <string, int> fnmap = {{"addu", 0x21}, {"and", 0x24}, {"nor", 0x27},
    {"or", 0x25}, {"sltu", 0x2b}, {"sll", 0}, {"srl", 2}, {"subu", 0x23},
    {"jr", 8}};

// data value, data address map, label map
unordered_map <string, int> v_map;
unordered_map <string, int> a_map;
unordered_map <string, int> labelmap;

char delim[] = " ,$\t"; // possible separators
char delim2[] = " \t";  // possible separators

int pc = DATA_START;

// class definition
/**
 * @brief 
 * This class simulate the R-type instruction with 6 register identification code.
 */
class Rtype {
    private:
        bitset<6> opcode; // operation code
        bitset<5> rs;     // first source operand register
        bitset<5> rt;     // second source operand register
        bitset<5> rd;     // result register
        bitset<5> sft;    // the displacement of the displacement instruction
        bitset<6> fn;     // R-type instruction's specific operation
    public:
        /**
        * @brief 
        * SetInfo receive the input instructions to each R-type instruction variable
        * @param _opcode, recieve operation code
        * @param _rs, recieve instruction of first source operand register
        * @param _rt, recieve instruction of second source operand register
        * @param _rd, recieve instruction of result register
        * @param _sft, recieve the displacement of the displacement instruction
        * @param _fn, recieve R-type instruction's specific operation
        * @return void
        */
        void SetInfo(int _opcode, int _rs, int _rt, int _rd, int _sft, int _fn) {
            opcode = bitset<6>(_opcode);
            rs = bitset<5>(_rs);
            rt = bitset<5>(_rt);
            rd = bitset<5>(_rd);
            sft = bitset<5>(_sft);
            fn = bitset<6>(_fn);
        }
        /**
        * @brief 
        * Use format() function to return the generated machine instruction in string.
        * @return string
        */
        string format() {
            return opcode.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + sft.to_string() + fn.to_string();
        }
    };

/**
 * @brief 
 * This class simulate the I-type instruction with 4 register identification code.
 */
class Itype {
    private:
        bitset<6> opcode; // operation code
        bitset<5> rs;     // first source operand register
        bitset<5> rt;     // second source operand register
        bitset<16> imm;   // immediate number or load/store instruction and branch instruction's offset address
    public:
        /**
        * @brief 
        * SetInfo receive the input instructions to each I-type instruction variable
        * @param _opcode, recieve operation code
        * @param _rs, recieve instruction of first source operand register
        * @param _rt, recieve instruction of second source operand register
        * @param _imm, recieve immediate number or load/store instruction and branch instruction's offset address
        * @return void
        */
        void SetInfo(int _opcode, int _rs, int _rt, int _imm)  {
            opcode = bitset<6>(_opcode);
            rs = bitset<5>(_rs);
            rt = bitset<5>(_rt);
            imm = bitset<16>(_imm);
        }
        /**
        * @brief 
        * Use format() function to return the generated machine instruction in string.
        * @return string
        */
        string format() {
            return opcode.to_string() + rs.to_string() + rt.to_string() + imm.to_string();
        }
    };

/**
 * @brief 
 * This class simulate the J-type instruction with 2 register identification code.
 */
class Jtype {
    private:
        bitset<6> opcode;  // operation code
        bitset<26> target; // unconditionally transfer number of the lower 26 bits of the address
    public:
        /**
        * @brief 
        * SetInfo receive the input instructions to each J-type instruction variable
        * @param _opcode, recieve operation code
        * @param _target, recieve unconditionally transfer numbfer of the lower 26 bits of the address
        * @return void
        */
        void SetInfo(int _opcode, int _target) {
            opcode = bitset<6>(_opcode);
            target = bitset<26>(_target);
        }
        /**
        * @brief 
        * Use format() function to return the generated machine instruction in string.
        * @return string
        */
        string format() {
            return opcode.to_string() + target.to_string();
        }
    };

/* 
 * @brief 
 * htoi: Converts a char of hexadecimal digits 
 *       (containing the optional prefix) to its integer value
 * @param char* h: address of hex c-string ex) "0xf"
 * @return integer ex) 15
 */
int htoi(char* h) {
    string h_str = (string) h;

    stringstream ss;
    ss << hex << h;

    int n;
    ss >> n;

    return n;
}

/* @brief 
 * interpret_instruction: extract_instruction
 * @param char* line: is an preprocessed assembly line
 * @return string  appropriate 32 bit formatted instruction
 */
string interpret_instruction(char* line) {
    int op;

    char* token = strtok(line, delim);

    op = opmap[(string) token];

    int data_address;
    int rs, rt, rd, sft, fn;
    int target, offset, imm;

    Rtype r;
    Itype i, i1, i2;
    Jtype j;

    // We will figure which type this operation is based on opname

    switch (op) {
        // la
        case 99:

            token = strtok(NULL, delim);
            rt = atoi(token);

            token = strtok(NULL, delim);
            data_address = a_map[(string) token];

            if (data_address > DATA_START)  {
                i1.SetInfo(0xf, 0, rt, 0x1000);
                i2.SetInfo(0xd, rt, rt, (data_address & 0xffff));

                return (i1.format() + i2.format());
            }

            else    {
                i1.SetInfo(0xf, 0, rt, 0x1000);
                return i1.format();
            }


        // R type instructions
        case 0  :
            fn = fnmap[token];

            token = strtok(NULL, delim);
            rd = atoi(token);

            switch (fn) {
                // sll
                case 0:
                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    token = strtok(NULL, delim);
                    sft = atoi(token);

                    r.SetInfo(op, 0, rt, rd, sft, 0);
                    break;

                // srl
                case 2:
                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    token = strtok(NULL, delim);
                    sft = atoi(token);

                    r.SetInfo(op, 0, rt, rd, sft, 2);
                    break;

                // jr
                case 8:
                    r.SetInfo(op, rd, 0, 0, 0, 8);
                    break;

                // other R type instructions
                default:
                    token = strtok(NULL, delim);
                    rs = atoi(token);

                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    r.SetInfo(op, rs, rt, rd, 0, fn);
                    break;
            }

            return r.format();

        // J type instructions
        case 2:
        case 3:
            token = strtok(NULL, delim);

            // label
            target = labelmap[(string) token];

            j.SetInfo(op, target/4);
            return j.format();

        // I type instructions
        default:
            switch(op)   {
                // beq, bne
                case 4:
                case 5:
                    token = strtok(NULL, delim);
                    rs = atoi(token);

                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    token = strtok(NULL, delim);
                    target = labelmap[(string) token];

                    offset = (target - (pc+4)) / 4;
                    i.SetInfo(op, rs, rt, offset);

                    break;

                // lw, sw;
                case 0x23:
                case 0x2b:
                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    token = strtok(NULL, " \t(");

                    offset = atoi(token);

                    token = strtok(NULL, "$) ");
                    rs = atoi(token);
                    i.SetInfo(op, rs, rt, offset);

                    break;

                // lui
                case 0xf:
                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    token = strtok(NULL, delim);
                    if (strstr(token, "0x"))   {
                        imm = htoi(token);
                    }

                    else    {
                        imm = atoi(token);
                    }
                    i.SetInfo(op, 0, rt, imm);
                    break;

                // other I type instructions
                default:
                    token = strtok(NULL, delim);
                    rt = atoi(token);

                    token = strtok(NULL, delim);
                    rs = atoi(token);

                    token = strtok(NULL, delim);
                    if (strstr(token, "0x"))   {
                        imm = htoi(token);
                    }

                    else    {
                        imm = atoi(token);
                    }
                    i.SetInfo(op, rs, rt, imm);

                    break;
            }

            return i.format();
    }
}
