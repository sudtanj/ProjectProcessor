#include <cstddef>

#include "Register.h"

Register::Register(int rSize, std::string rName)
{
    bits = new bool[rSize];
    regSize = rSize;
    clearBits();
    name = rName;
}

void Register::clearBits()
{
    if(bits!=NULL)
        for(int i=0; i<regSize; i++) bits[i] = false;
}

bool* Register::getBits()
{
    return bits;
}

bool Register::setBits(bool* newBits)
{
    for(int i=0; i<regSize; i++)
        bits[i] = newBits[i];
	return false;
}

int Register::getNumberOfBits()
{
    return regSize;
}

std::string Register::getName()
{
    return name;
}
