#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

const char message[] =  "world";

char message2[] = "make world better";

void main()
{
	string localString = "hello world";
	while (1)
	{
		cout << "Stack memory: " << localString<< endl;
		cout << "Const static memory: " << message << endl;
		cout << "Static memory: " << message2 << endl;
		_getch();
	}
}