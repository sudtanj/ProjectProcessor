#include "Console.h"
#include <string>

void saveMemory(std::string line) {
	std::cout << line << std::endl;
}

int main() {
	Console* konsol = new Console();
	//konsol->setMemorySave(saveMemory);
	konsol->Input();
	return 0;
}