#pragma once
#include<iostream>
#include <string>
#include <Windows.h>
#include <time.h>
#include "Assembler.h"
#include "Processor.h"
class Console {
private:
	std::string line;
	Processor* processor;
	Assembler* assembler;
	int consoleCounter=0;
public:
	Console() {
		Processor* processor=new Processor(32, 8196);
		this->processor = processor;
	};
	void Input() {
		int counter = 20;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		short ret;
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		ret = info.wAttributes;
		srand(time(NULL));
		prompt:
		while (counter != 0) {
			system("cls");
			if (counter == 1) {
				system("color 07");
			}
			else 
				SetConsoleTextAttribute(hConsole, rand()%255+1);
			for (int i = 0;i != counter;i++) 
				std::cout << std::endl;
			for (int i = 0;i != counter;i++)
				std::cout << " ";
			std::cout << "     ============================================     " << std::endl;
			for (int i = 0;i != counter;i++)
				std::cout << " ";
			std::cout << "     |               CPU V1.0                   |     " << std::endl;
			for (int i = 0;i != counter;i++)
				std::cout << " ";
			std::cout << "     |   Copyright(c) 2016. All Right Reserved. |     " << std::endl;
			for (int i = 0;i != counter;i++)
				std::cout << " ";
			std::cout << "     |          Author =                        |     " << std::endl;
			for (int i = 0;i != counter;i++)
				std::cout << " ";
			std::cout << "     ============================================     " << std::endl;
			if (counter == 20) {
				for (int i = 0;i != counter+12;i++)
					std::cout << " ";
				system("pause");
			}
			counter--;
			Sleep(300);
		}
		std::cout << std::endl << "Initialization Done!" << std::endl;
		std::cout << "Ready for input...." << std::endl;
		while (this->line != "run") {
			std::cout << ">";
			std::getline(std::cin,this->line);
			std::cout << "\a";
			if (this->line == "exit") {
				exit(EXIT_SUCCESS);
			}
			assembler->insertInstructionToMemory(processor, this->line);
			consoleCounter++;
		}
		this->line = "";
		system("cls");
		while (consoleCounter != 0) {
			processor->fetch();
			processor->decode_and_execute();
			showRegisters(processor);
			consoleCounter--;
		}
		system("pause");
		system("cls");
		counter = 1;
		goto prompt;
	};
	void errorMessage(std::string errormessage,bool systemExit) {
		system("cls");
		std::cout << "     ===================    " << std::endl;
		std::cout << "     | × Error Message |    " << std::endl;
		std::cout << "     ===================    ";
		for (int i = 0;i < errormessage.size();i++) {
			if (i % 24 == 0) {
				std::cout << std::endl;
				std::cout << "   ";
			}
			std::cout << errormessage[i];
		}
		std::cout << std::endl<<std::endl;
		system("pause");
		if (systemExit==true)
			system("exit");
	}
	void showRegisters(Processor* processor)
	{
		int total = processor->getNumberOfRegister();
		Register** temp = processor->getPublicRegisters();
		for (int i = 0; i<total; i++)
		{
			cout << temp[i]->getName() << " : ";
			for (int j = 0; j<temp[i]->getNumberOfBits(); j++) cout << temp[i]->getBits()[j];
			cout << endl;
		}
		cout << "Flag Register ";
		for (int j = 0; j<processor->getFlagRegister()->getNumberOfBits(); j++) cout << processor->getFlagRegister()->getBits()[j];
		cout << endl;
		cout << endl;
	}
	int boolLength(bool*data) {
		return 32;
	}
};