#include "header.h"
#define MAX_PATH_LENGHT 256

bool generateMod(params* user_input) {

	FILE* f = nullptr;
	fopen_s(&f, "directories.txt", "r");

	if (!f)return false;

	char buffer[MAX_PATH_LENGHT];
	fgets(buffer, sizeof(buffer), f);

	std::string str = buffer;
	str.append("\\GenerateMod.bat");

	
	fclose(f);
	return true;
}