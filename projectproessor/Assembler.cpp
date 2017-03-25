#include <cctype>
#include <sstream>
#include <cstddef>
#include <iostream>

#include "Assembler.h"

#define REGISTER_TYPE 1
#define MEMORY_TYPE 2
#define IMMEDIATE_TYPE 3
#define ADDRESS_TYPE 4

Assembler::Assembler()
{

}

std::vector<std::string> Assembler::splitString(std::string str, char delimiter)
{
	std::vector<std::string> split;
	int sz = str.length(), lastIdx = -1, i;

	for(i=0; i<sz; i++)
	{
		if(str[i]==delimiter)
		{
			split.push_back(str.substr(lastIdx+1, i-lastIdx-1));
			lastIdx = i;
		}
	}
	if(i==sz && lastIdx!=i-1) split.push_back(str.substr(lastIdx+1));

	return split;
}

void Assembler::insertBoolByString(bool* instruction, std::string input, int fromIdx)
{
    int len = input.length();
    for(int i=0; i<=len; i++) instruction[fromIdx+i] = input[i]-'0' ? true : false;
}

std::string Assembler::toBinaryString(int num, int stringSize)
{
    std::string out="";
    while(stringSize)
    {
        out = char(num%2 + '0') + out;
        num/=2;
        stringSize--;
    }
    return out;
}

int Assembler::findRegister(Processor* processor, std::string registerName)
{
    std::vector<std::string> registersName = processor->getRegistersName();
    int totalName = registersName.size();
    for(int i=0; i<totalName; i++)
    {
        if(registerName == registersName[i]) return i;
    }
    return -1;
}

int Assembler::inputType(Processor* processor, std::string input)
{
    int idx = findRegister(processor, input);
    if(idx >= 0) return REGISTER_TYPE;

    if(input[0] == '[' && input[input.length()-1] == ']') return MEMORY_TYPE;

    if(isNumeric(input)) return IMMEDIATE_TYPE;

    return ADDRESS_TYPE;
}

bool Assembler::isNumeric(std::string input)
{
    int len = input.length();
    for(int i=0; i<len; i++)
        if(!isdigit(input[i])) return false;
    return true;
}

int Assembler::getInt(std::string input)
{
    std::istringstream ssinput(input);
	int check;

	ssinput >> check;
	return check;
}

void Assembler::insertInstructionToMemory(Processor* processor, std::string command)
{
    std::vector<std::string> splitCommand = splitString(command, ' ');
    bool* instruction = new bool[processor->getNumberOfBits()];
    int input1, input2;

    /*MNEMONIC OPERAND OPERAND*/
    if(splitCommand.size()==3)
    {
        /*OPCODE*/
        if(splitCommand[0]=="add") insertBoolByString(instruction, "00000", 0);
        else if(splitCommand[0]=="sub") insertBoolByString(instruction, "00001", 0);
        else if(splitCommand[0]=="mul") insertBoolByString(instruction, "00010", 0);
        else if(splitCommand[0]=="div") insertBoolByString(instruction, "00011", 0);
        else if(splitCommand[0]=="mod") insertBoolByString(instruction, "00100", 0);
        else if(splitCommand[0]=="cmp") insertBoolByString(instruction, "00101", 0);
        else if(splitCommand[0]=="and") insertBoolByString(instruction, "01001", 0);
        else if(splitCommand[0]=="or") insertBoolByString(instruction, "01010", 0);
        else if(splitCommand[0]=="xor") insertBoolByString(instruction, "01100", 0);
        else if(splitCommand[0]=="mov") insertBoolByString(instruction, "10010", 0);

        /*OPERAND*/
        input1 = inputType(processor, splitCommand[1]);
        input2 = inputType(processor, splitCommand[2]);
        if(input1 == REGISTER_TYPE && input2 == REGISTER_TYPE)
        {
            insertBoolByString(instruction, "000", 5);
            int idx = findRegister(processor, splitCommand[1]);
            insertBoolByString(instruction, toBinaryString(idx, 3), 8);
            idx = findRegister(processor, splitCommand[2]);
            insertBoolByString(instruction, toBinaryString(idx, 3), 11);
            for(int i=14; i<32; i++) insertBoolByString(instruction, "0", i);
        }
        else if(input1 == REGISTER_TYPE && input2 == MEMORY_TYPE)
        {
            insertBoolByString(instruction, "001", 5);
            int idx = findRegister(processor, splitCommand[1]);
            insertBoolByString(instruction, toBinaryString(idx, 3), 8);
            idx = findRegister(processor, splitCommand[2].substr(1, splitCommand[2].length()-2));
            insertBoolByString(instruction, toBinaryString(idx, 3), 11);
            for(int i=14; i<32; i++) insertBoolByString(instruction, "0", i);
        }
        else if(input1 == REGISTER_TYPE && input2 == IMMEDIATE_TYPE)
        {
            insertBoolByString(instruction, "010", 5);
            int idx = findRegister(processor, splitCommand[1]);
            insertBoolByString(instruction, toBinaryString(idx, 3), 8);
            insertBoolByString(instruction, toBinaryString(getInt(splitCommand[2]), 16), 11);
            for(int i=27; i<32; i++) insertBoolByString(instruction, "0", i);
        }
        else if(input1 == MEMORY_TYPE && input2 == REGISTER_TYPE)
        {
            insertBoolByString(instruction, "001", 5);
            int idx = findRegister(processor, splitCommand[1].substr(1, splitCommand[2].length()-2));
            insertBoolByString(instruction, toBinaryString(idx, 3), 8);
            idx = findRegister(processor, splitCommand[2]);
            insertBoolByString(instruction, toBinaryString(idx, 3), 11);
            for(int i=14; i<32; i++) insertBoolByString(instruction, "0", i);
        }
    }
    /*MNEMONIC OPERAND*/
    else if(splitCommand.size()==2)
    {
        int type = inputType(processor, splitCommand[1]);
        if(type == REGISTER_TYPE)
        {
            if(splitCommand[0]=="shr") insertBoolByString(instruction, "00110", 0);
            else if(splitCommand[0]=="shl") insertBoolByString(instruction, "00111", 0);
            else if(splitCommand[0]=="rot") insertBoolByString(instruction, "01000", 0);
            else if(splitCommand[0]=="not") insertBoolByString(instruction, "01011", 0);
            else if(splitCommand[0]=="read") insertBoolByString(instruction, "10011", 0);
            else if(splitCommand[0]=="write") insertBoolByString(instruction, "10100", 0);
            insertBoolByString(instruction, "000", 5);
            int idx = findRegister(processor, splitCommand[1]);
            insertBoolByString(instruction, toBinaryString(idx, 3), 8);
        }
        else if(type == MEMORY_TYPE)
        {
            if(splitCommand[0]=="shr") insertBoolByString(instruction, "00110", 0);
            else if(splitCommand[0]=="shl") insertBoolByString(instruction, "00111", 0);
            else if(splitCommand[0]=="rot") insertBoolByString(instruction, "01000", 0);
            else if(splitCommand[0]=="not") insertBoolByString(instruction, "01011", 0);
            else if(splitCommand[0]=="read") insertBoolByString(instruction, "10011", 0);
            else if(splitCommand[0]=="write") insertBoolByString(instruction, "10100", 0);
            insertBoolByString(instruction, "000", 5);
            int idx = findRegister(processor, splitCommand[1].substr(1, splitCommand[1].length()-2));
            insertBoolByString(instruction, toBinaryString(idx, 3), 8);
        }
        else if(type == ADDRESS_TYPE)
        {
            if(splitCommand[0]=="jump") insertBoolByString(instruction, "01101", 0);
            else if(splitCommand[0]=="jl") insertBoolByString(instruction, "01110", 0);
            else if(splitCommand[0]=="jg") insertBoolByString(instruction, "01111", 0);
            else if(splitCommand[0]=="je") insertBoolByString(instruction, "10000", 0);
            else if(splitCommand[0]=="loop") insertBoolByString(instruction, "10001", 0);
            else if(splitCommand[0]=="call") insertBoolByString(instruction, "10101", 0);
            insertBoolByString(instruction, "100", 5);

            /*NEED ADDRESS (LOCATION OF) FOR THESE FUNCTION*/
        }
    }
    processor->getMemory()->setMemoryAutoSafe(instruction);
}
