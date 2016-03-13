#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int getVarValue(string variable, string filePath);
string replaceStrChar(string str, const string& replace, char ch);

int main()
{
	int value = getVarValue("kappa", "settings.ini");
	cout << "Value : " << value << endl;
	return 0;
	
}
string replaceStrChar(string str, const string& replace, char ch) {
	size_t found = str.find_first_of(replace);
	while (found != string::npos) 
	{ 
		str[found] = ch; 
		found = str.find_first_of(replace, found + 1); 
	}
	return str; 
}
int getVarValue(string variable, string filePath) 
{
	int value;
	string line, test;
	ifstream duom(filePath);
	if (duom.is_open())
	{
		while (getline(duom, line))
		{
			if (line.find(variable) != string::npos && line.find("[") == string::npos)
			{		
				line = replaceStrChar(line, "=", ' ');
				istringstream str(line);
				str >> variable>> value;
			}
		}
		duom.close();
	}
	return value;
}