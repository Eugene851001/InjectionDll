#include <Windows.h>
#include <stdio.h>
#include <iostream>

//#include "..\\SomeDll\Header.h"
//#pragma comment(lib, "..\\x64\\Debug\\SomeDll.lib")

using namespace std;

extern "C" __declspec(dllexport) void WINAPI Replace(const char*, const char*);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Replace("world", "bsuir");
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}