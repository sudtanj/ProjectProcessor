#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <string>

#include "Processor.h"

class Assembler
{
    private:
        std::vector<std::string> splitString(std::string str, char delimiter);
        void insertBoolByString(bool* instruction, std::string input, int fromIdx);
        std::string toBinaryString(int num, int stringSize);
        int findRegister(Processor* processor, std::string registerName);
        int inputType(Processor* processor, std::string input);
        bool isNumeric(std::string input);
        int getInt(std::string input);
    public:
        Assembler();
        void insertInstructionToMemory(Processor* processor, std::string command);
};

#endif // ASSEMBLER_H
