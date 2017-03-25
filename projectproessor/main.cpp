#include <iostream>

using namespace std;

#include "Assembler.h"
#include "Processor.h"
#include "Console.h"

void showRegisters(Processor* processor);

int main()
{
	Console* konsol = new Console();
	konsol->Input();
    cout<<"MAIN\n";
    Processor processor(32, 8196);
    Assembler assembler;
    cout<<"DONE PROCESSOR\n";
    bool* res = processor.toBinary(66);
    for(int i=0; i<32; i++)cout<<res[i]; cout<<endl;
    bool* res1 = processor.toBinary(12);
    for(int i=0; i<32; i++)cout<<res1[i]; cout<<endl;

/*    processor.divide(NULL, res, res1, processor.getNumberOfBits());
    for(int i=0; i<32; i++)cout<<res[i]; cout<<endl;
    for(int i=0; i<32; i++)cout<<res1[i]; cout<<endl;

    std::cout<<std::endl;
    std::cout<<std::endl;

    processor.modulo(NULL, res, res1, processor.getNumberOfBits());
    for(int i=0; i<32; i++)cout<<res[i]; cout<<endl;
    for(int i=0; i<32; i++)cout<<res1[i]; cout<<endl;
*/
    cout<<endl;

    assembler.insertInstructionToMemory(&processor, "mov az 10");
    assembler.insertInstructionToMemory(&processor, "mov bz 11");
    assembler.insertInstructionToMemory(&processor, "sub bz az");

    showRegisters(&processor);

    processor.fetch();
    processor.decode_and_execute();
    showRegisters(&processor);
    processor.fetch();
    processor.decode_and_execute();
    showRegisters(&processor);
    processor.fetch();
    processor.decode_and_execute();
    showRegisters(&processor);
	system("pause");
    return 0;
}

void showRegisters(Processor* processor)
{
    int total = processor->getNumberOfRegister();
    Register** temp = processor->getPublicRegisters();
    for(int i=0; i<total; i++)
    {
        cout<<temp[i]->getName()<<" : ";
        for(int j=0; j<temp[i]->getNumberOfBits(); j++) cout<<temp[i]->getBits()[j];
        cout<<endl;
    }
    cout<<"Flag Register : ";
    for(int j=0; j<processor->getFlagRegister()->getNumberOfBits(); j++) cout<<processor->getFlagRegister()->getBits()[j];
    cout<<endl;
    cout<<endl;
}
