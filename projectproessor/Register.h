#ifndef REGISTER_H
#define REGISTER_H

#include <string>

class Register
{
    private:
        bool* bits;
        int regSize;
        std::string name;
    public:
        Register(int rSize, std::string name);
        bool setBits(bool* newBits);
        bool* getBits();
        int getNumberOfBits();
        void clearBits();
        std::string getName();
};

#endif // REGISTER_H

