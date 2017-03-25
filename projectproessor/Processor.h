#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

#include "Register.h"
#include "Memory.h"
#include "ALU.h"

class Processor
{
    private:
        Register** registers;
        Register* instructionRegister;
        Register* flagRegister;
        Register* programCounter;
        ALU* alu;
        Memory* memory;
        int numBits;
        int numOfRegister;
        int instructionMemoryStart;
        int dataMemoryStart;
        void two_operand(std::string instruction, bool* value_1, bool* value_2, int bits);
    public:
        Processor(int bits, int memorySize);
        int getNumberOfBits();
        int getNumberOfRegister();
        Register** getPublicRegisters();
        Register* getFlagRegister();
        std::vector<std::string> getRegistersName();
        void add(Register* destination, bool* num_1, bool* num_2, int numberOfBits);
        void substract(Register* destination, bool* num_1, bool* num_2, int numberOfBits);
        void multiply(Register* destination, bool* num_1, bool* num_2, int numberOfBits);
        void divide(Register* destination, bool* num_1, bool* num_2, int numberOfBits);
        void modulo(Register* destination, bool* num_1, bool* num_2, int numberOfBits);
        void compare(bool* num_1, bool* num_2, int numberOfBits);
        void shiftRight();
        void fetch();
        void decode_and_execute();
        Memory* getMemory();
        bool* toBinary(int number);
        unsigned int toDecimal_Unsigned(bool* bits, int numOfBits);
};

#endif // PROCESSOR_H

/** Flag Register
0  - Zero Flag
1  - Sign Flag
2  - Carry Flag
3  - Overflow Flag
4  -
5  -
6  -
7  -
8  -
9  -
10 -
11 -
12 -
13 -
14 -
15 -
16 -
17 -
18 -
19 -
20 -
21 -
22 -
23 -
24 -
25 -
26 -
27 -
28 -
29 -
30 -
31 -
*/
