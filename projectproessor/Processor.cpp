#include <iostream>

#include "Processor.h"

Processor::Processor(int bits, int memorySize)
{
    registers = new Register*[8];
    std::cout<<"REGISTER\n";
    registers[0] = new Register(bits, "az");
    registers[1] = new Register(bits, "bz");
    registers[2] = new Register(bits, "cz");
    registers[3] = new Register(bits, "dz");
    registers[4] = new Register(bits, "ez");
    registers[5] = new Register(bits, "fz");
    registers[6] = new Register(bits, "gz");
    registers[7] = new Register(bits, "hz");
    instructionRegister = new Register(bits, "ir");
    flagRegister = new Register(bits, "flag");
    programCounter = new Register(bits, "pc");
    alu = new ALU();
    memory = new Memory(bits, memorySize);
    instructionMemoryStart = 0;
    dataMemoryStart = memorySize/2;
    numBits = bits;
    numOfRegister = 8;
}

int Processor::getNumberOfBits()
{
    return numBits;
}

int Processor::getNumberOfRegister()
{
    return numOfRegister;
}

Register** Processor::getPublicRegisters()
{
    return registers;
}

Register* Processor::getFlagRegister()
{
    return flagRegister;
}

std::vector<std::string> Processor::getRegistersName()
{
    std::vector<std::string> names;
    for(int i=0; i<8; i++) names.push_back(registers[i]->getName());
    return names;
}

void Processor::add(Register* destination, bool* num_1, bool* num_2, int numberOfBits)
{
    bool* res = alu->add(num_1, num_2, numberOfBits, flagRegister);
    destination->setBits(res);
}

void Processor::substract(Register* destination, bool* num_1, bool* num_2, int numberOfBits)
{
    bool* res = alu->substract(num_1, num_2, numberOfBits, flagRegister);
    for(int i=0; i<numBits; i++) std::cout<<res[i]; std::cout<<std::endl;
    //destination->setBits(res);
}

void Processor::multiply(Register* destination, bool* num_1, bool* num_2, int numberOfBits)
{
    bool* res = alu->multiply(num_1, num_2, numberOfBits, flagRegister);
    for(int i=0; i<numBits; i++) std::cout<<res[i]; std::cout<<std::endl;

}

void Processor::divide(Register* destination, bool* num_1, bool* num_2, int numberOfBits)
{
    bool* res = alu->divide(num_1, num_2, numberOfBits, flagRegister);
    if(res!=NULL) for(int i=0; i<numBits; i++) std::cout<<res[i]; std::cout<<std::endl;

}

void Processor::modulo(Register* destination, bool* num_1, bool* num_2, int numberOfBits)
{
    bool* res = alu->modulo(num_1, num_2, numberOfBits, flagRegister);
    if(res!=NULL) for(int i=0; i<numBits; i++) std::cout<<res[i]; std::cout<<std::endl;

}

void Processor::compare(bool* num_1, bool* num_2, int numberOfBits)
{
    bool* res = alu->substract(num_1, num_2, numberOfBits, flagRegister);
    flagRegister->getBits()[1] = res[0];
    int i;
    for(i=0; i<numberOfBits; i++)
        if(res[i]) break;
    if(i<numberOfBits) flagRegister->getBits()[0] = 0;
    else flagRegister->getBits()[0] = 1;
}

bool* Processor::toBinary(int number)
{
    return alu->toBinary(number, numBits);
}

void Processor::fetch()
{
    bool* now = new bool[numBits];
    bool* inst = memory->getMemory(toDecimal_Unsigned(programCounter->getBits(), programCounter->getNumberOfBits()));
    for(int i=0; i<numBits; i++) now[i] = inst[i];
    instructionRegister->setBits(inst);
    std::cout<<"INSTRUCTION\n";
    for(int i=0; i<instructionRegister->getNumberOfBits(); i++) std::cout<<instructionRegister->getBits()[i]; std::cout<<std::endl;
    programCounter->setBits(alu->add(programCounter->getBits(), toBinary(1), numBits, flagRegister));
}

void Processor::decode_and_execute()
{
    bool* instruction = instructionRegister->getBits();
    std::string instructionString="";
    for(int i=0; i<instructionRegister->getNumberOfBits(); i++)
    {
        instructionString = instructionString + char(instruction[i]+'0');
    }
    bool* value_1 = new bool[numBits];
    bool* value_2 = new bool[numBits];
    bool* n;
    Register* destination;
    std::string operationCode = instructionString.substr(0, 5);
    std::cout<<operationCode<<std::endl;
    if(operationCode=="00000")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        bool* addition = alu->add(value_1, value_2, numBits, flagRegister);
        destination->setBits(addition);
        std::cout<<"add "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="00001")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        bool* substraction = alu->substract(value_1, value_2, numBits, flagRegister);
        std::cout<<std::endl;
        for(int i=0; i<numBits; i++) std::cout<<substraction[i];
        std::cout<<std::endl;
        destination->setBits(substraction);
        std::cout<<"sub "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="00010")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        bool* multiplication = alu->multiply(value_1, value_2, numBits, flagRegister);
        destination->setBits(multiplication);
        std::cout<<"mul "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="00011")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        bool* division = alu->divide(value_1, value_2, numBits, flagRegister);
        destination->setBits(division);
        std::cout<<"div "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="00100")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        bool* mod = alu->modulo(value_1, value_2, numBits, flagRegister);
        destination->setBits(mod);
        std::cout<<"mod "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="00101")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        compare(value_1, value_2, numBits);
        std::cout<<"cmp "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="01001")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        bool* substraction = alu->substract(value_1, value_2, numBits, flagRegister);
        destination->setBits(substraction);
        std::cout<<"sub "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
    else if(operationCode=="10010")
    {
        two_operand(instructionString, value_1, value_2, numBits);
        n = new bool[3];
        for(int i=0; i<3; i++) n[i] = instruction[i+8];
        destination = registers[toDecimal_Unsigned(n, 3)];
        destination->setBits(value_2);
        std::cout<<"mov "<<toDecimal_Unsigned(value_1, numBits)<<" "<<toDecimal_Unsigned(value_2, numBits)<<std::endl;
    }
}

Memory* Processor::getMemory()
{
    return memory;
}

unsigned int Processor::toDecimal_Unsigned(bool* bits, int numOfBits)
{
    int number = 0, multiplier = 1;
    for(int i=numOfBits-1; i>=0; i--)
    {
        if(bits[i]) number += multiplier;
        multiplier*=2;
    }
    return number;
}

void Processor::two_operand(std::string instruction, bool* value_1, bool* value_2, int bits)
{
    std::string type = instruction.substr(5, 3);
    bool* n_1 = new bool[bits];
    bool* n_2 = new bool[bits];
    for(int i=0; i<bits; i++)
    {
        value_1[i] = 0;
        value_2[i] = 0;
    }
    if(type == "000")
    {
        for(int i=0; i<3; i++) n_1[i] = instruction[i+8]-'0';
        n_1 = registers[toDecimal_Unsigned(n_1, 3)]->getBits();
        for(int i=0; i<bits; i++) value_1[i] = n_1[i];
        for(int i=0; i<3; i++) n_2[i] = instruction[i+11]-'0';
        n_2 = registers[toDecimal_Unsigned(n_2, 3)]->getBits();
        for(int i=0; i<bits; i++) value_2[i] = n_2[i];
    }
    else if(type == "001")
    {
        for(int i=0; i<3; i++) n_1[i] = instruction[i+8]-'0';
        n_1 = registers[toDecimal_Unsigned(n_1, 3)]->getBits();
        for(int i=0; i<bits; i++) value_1[i] = n_1[i];
        for(int i=0; i<3; i++) n_2[i] = instruction[i+11]-'0';
        n_2 = memory->getMemory(toDecimal_Unsigned(n_2, 3));
        for(int i=0; i<bits; i++) value_2[i] = n_2[i];
    }
    else if(type == "010")
    {
        for(int i=0; i<3; i++) n_1[i] = instruction[i+8]-'0';
        n_1 = registers[toDecimal_Unsigned(n_1, 3)]->getBits();
        for(int i=0; i<bits; i++) value_1[i] = n_1[i];
        for(int i=0; i<16; i++) value_2[bits-16+i] = instruction[i+11]-'0';
    }
    else if(type == "011")
    {
        for(int i=0; i<3; i++) n_1[i] = instruction[i+8]-'0';
        n_1 = memory->getMemory(toDecimal_Unsigned(n_1, 3));
        for(int i=0; i<bits; i++) value_1[i] = n_1[i];
        for(int i=0; i<3; i++) n_2[i] = instruction[i+11]-'0';
        n_2 = registers[toDecimal_Unsigned(n_2, 3)]->getBits();
        for(int i=0; i<bits; i++) value_2[i] = n_2[i];
    }
}
