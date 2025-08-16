// devlor_script_the_last_of_us_part_ii
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetProcessIDByName(const char* processName) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, processName) == 0) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return 0;
}

int main() {
    const char* targetProcess = "tlou-ii.exe";

    // мейн меню на F1 короче
    uintptr_t address1 = 0x1188197D70; // main
    BYTE openValue1 = 1;
    BYTE closedValue1 = 0;
    bool menuOpen1 = false;

    // квик меню на F2 короче
    uintptr_t address2 = 0x1188146130; // квик
    BYTE openValue2 = 1;
    BYTE closedValue2 = 0;
    bool menuOpen2 = false;

    // запускаем игру короче
    ShellExecuteA(NULL, "open", targetProcess, NULL, NULL, SW_SHOW);

    // ждем процесс короче
    DWORD pid = 0;
    while (pid == 0) {
        pid = GetProcessIDByName(targetProcess);
        Sleep(500);
    }

    // открываем процесс короче
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return 1;

    // основной цикл короче
    while (GetProcessIDByName(targetProcess) != 0) {
        // ф1
        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            menuOpen1 = !menuOpen1;
            BYTE value = menuOpen1 ? openValue1 : closedValue1;
            SIZE_T written;
            WriteProcessMemory(hProcess, (LPVOID)address1, &value, sizeof(value), &written);
            Sleep(200);
        }

        // ф2
        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            menuOpen2 = !menuOpen2;
            BYTE value = menuOpen2 ? openValue2 : closedValue2;
            SIZE_T written;
            WriteProcessMemory(hProcess, (LPVOID)address2, &value, sizeof(value), &written);
            Sleep(200);
        }
        Sleep(10);
    }
    CloseHandle(hProcess);
    return 0;
}