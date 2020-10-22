#pragma once
#include <Windows.h>

extern "C" __declspec(dllexport) void WINAPI Replace(const char* data, const char* replacement);
extern "C" __declspec(dllexport) void WINAPI PrintMessage();