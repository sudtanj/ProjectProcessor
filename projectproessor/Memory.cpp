#include <malloc.h>
#include <string.h>

#include "Memory.h"

Memory::Memory(int memoryBit, int memorySize)
{
    bits = new bool*[memorySize];
    for(int i=0; i<memorySize; i++) bits[i] = new bool[memoryBit];
    size = memorySize;
    length = memoryBit;
    resetMemory();
}

void Memory::setMemory(bool* data, int location)
{
    for(int i=0;i<32;i++)
        *(*(bits+location)+i)=*(data+i);
}

bool* Memory::getMemory(int location)
{
    return *(bits+location);
}

int Memory::memorySize()
{
    return size;
}

int Memory::memoryLength()
{
    return length;
}

bool Memory::checkIfMemoryExist(bool* data)
{
    int i=0,j=0;
    for(j=0;j<memorySize();j++)
    {
        for(i=0;i<memoryLength();i++)
            if(*(*(bits+j)+i)!=*(data+i))
                break;
        if(i==memoryLength())
        {
            temporary_Result=j;
            return true;
        }
    }
    return false;
}

int Memory::getLocation(bool* data)
{
    if(checkIfMemoryExist(data))
    {
        return temporary_Result;
    }
    return -1;
}

bool Memory::checkMemoryEmpty(int location)
{
    for(int i=0;i<memoryLength();i++){
        if(*(*(bits+location)+i))
        {
            return false;
        }
    }
    return true;
}

bool Memory::setMemoryAutoSafe(bool* data)
{
    for(int i=0;i<memorySize();i++)
        if(checkMemoryEmpty(i))
        {
            setMemory(data,i);
            return true;
        }
    return false;
}

bool Memory::setMemoryAutoUnsafe(bool* data)
{
    if(!(checkMemoryEmpty(memorySize())))
    {
        setMemory(data,last_Memory_Location_Set);
        if(last_Memory_Location_Set==memorySize())
            last_Memory_Location_Set=0;
        else
            last_Memory_Location_Set++;
    }
    else
        setMemoryAutoSafe(data);
	return false;
}

void Memory::deleteMemory(int location)
{
    memset(*(bits+location),false,length);
}

void Memory::resetMemory()
{
    for(int i=0;i<memorySize();i++)
        deleteMemory(i);
}
