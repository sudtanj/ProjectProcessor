#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
    private:
        bool** bits;
        int size;
        int length;
        int temporary_Result;
        int last_Memory_Location_Set=0;
    public:
        Memory(int memoryBit, int memorySize);
        void setMemory(bool* data, int location);
        bool* getMemory(int location);
        int memorySize();
        int memoryLength();
        bool checkIfMemoryExist(bool* data);
        int getLocation(bool* data);
        bool checkMemoryEmpty(int location);
        bool setMemoryAutoSafe(bool* data);
        bool setMemoryAutoUnsafe(bool* data);
        void deleteMemory(int location);
        void resetMemory();
};

#endif // MEMORY_H
