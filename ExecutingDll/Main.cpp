#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <tlhelp32.h>

//#include "..\\SomeDll\\Header.h"
//#pragma comment(lib, "..\\x64\\Debug\\SomeDll.lib")

extern "C" __declspec(dllexport) void WINAPI Replace(const char*, const char*);
extern "C" __declspec(dllimport) void WINAPI PrintMessage();

using namespace std;

void injectProcess(DWORD pid, string argument)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) {
		printf("Error: the specified process couldn't be found.\n");
	}

	LPVOID threadFunction = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if (threadFunction == NULL) {
		printf("Error: the LoadLibraryA function was not found inside kernel32.dll library.\n");
	}

	LPVOID argumentAddress = VirtualAllocEx(hProcess, NULL, argument.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (argumentAddress == NULL) {
		printf("Error: the memory could not be allocated inside the chosen process.\n");
	}

	int n = WriteProcessMemory(hProcess, argumentAddress, argument.c_str(), argument.length() + 1, NULL);
	if (n == 0) {
		printf("Error: there was no bytes written to the process's address space.\n");
	}

	HANDLE threadID = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)threadFunction, argumentAddress, 0, NULL);
	if (threadID == NULL) {
		printf("Error: the remote thread could not be created.\n");
	}
	else {
		printf("Success: the remote thread was successfully created.\n");
	}
//	Sleep(1000);
	CloseHandle(hProcess);
}

DWORD GetProcessId(PCSTR exeName)
{
	DWORD pid = -1;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processentry;
	ZeroMemory(&processentry, sizeof(processentry));
	processentry.dwSize = sizeof(processentry);
	BOOL isFound = FALSE;
	if (Process32First(hSnapshot, &processentry))
	{
		do
		{
			if (!strcmp(processentry.szExeFile, exeName))
			{
				pid = processentry.th32ProcessID;
				isFound = TRUE;
			}

		} while (Process32Next(hSnapshot, &processentry) && !isFound);

	}
	CloseHandle(hSnapshot);
	return pid;
}

void main()
{

	printf("Test static import\n");
	PrintMessage();

	printf("Test dynamic import\n");
	void (WINAPI * PrintMessageDynamic)();
	HMODULE hModule = LoadLibrary("SomeDll.dll");
	if (hModule != NULL)
	{
		PrintMessageDynamic = (void (WINAPI*)())GetProcAddress(hModule, "_PrintMessage@0");
		if (PrintMessageDynamic == NULL)
		{
			printf("Can not find procesure");
		}
		else
		{
			PrintMessageDynamic();
		}
		FreeLibrary(hModule);
	}
	else
	{
		printf("Can not find library\n");
	}

	//dll injection
	cout << "Enter name process: " << endl;
	std::string AppName;
	std::cin >> AppName;
	DWORD pid = GetProcessId(AppName.c_str());
	cout << "PID: ";
	cout << pid << endl;


	if (pid != -1)
	{

		cout << "\nThe process was found" << endl;

		injectProcess(pid, "InjectionDll.dll");
	}
	else
		cout << "Cant find process" << endl;

	cout << "\nEnter key for exit" << endl;
	_getch();
}