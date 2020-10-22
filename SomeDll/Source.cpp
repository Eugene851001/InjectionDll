#include "Header.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <vector>

using namespace std;

void WINAPI PrintMessage()
{
	printf("Message from DLL\n");
}

void WINAPI Replace(const char* data, const char* replacement)
{
	HANDLE process = GetCurrentProcess();
	size_t len = strlen(data);
	size_t replacementLength = strlen(replacement);

	if (process)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		MEMORY_BASIC_INFORMATION info;
		std::vector<char> buffer;
		char* p = (char*)si.lpMinimumApplicationAddress;
		while (p < si.lpMaximumApplicationAddress)
		{
			if (VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info) )
			{
				if (info.State == MEM_COMMIT && info.Protect == PAGE_READWRITE)
				{
					p = (char*)info.BaseAddress;
					buffer.resize(info.RegionSize);
					SIZE_T bytesRead;

					if (ReadProcessMemory(process, p, &buffer[0], info.RegionSize, &bytesRead))
					{
						for (size_t i = 0; i < (bytesRead - len); ++i)
						{
							if (memcmp(data, &buffer[i], len) == 0)
							{
								char* ref = (char*)p + i;
								for (int j = 0; j < len; j++) {
									ref[j] = replacement[j];
								}
							
							//	ref[len] = 0;
							}
						}
					}
				}
				p += info.RegionSize;
			}
		}
	} 
}