#include <iostream>
#include <Windows.h>
#include <WinUser.h>

#define m_iTeamNum 0xF4
#define dwLocalPlayer 0xDB458C
#define dwEntityList 0x4DCFA94
#define m_iCrosshairId 0x11838
#define m_fFlags 0x104
#define dwForceJump 0x527998C

uintptr_t moduleBase;

INPUT inputs[2] = {};

template<typename T> T RPM(uintptr_t address) {
    try { return *(T*)address; }
    catch (...) { return T(); }
}

uintptr_t getLocalPlayer() { //This will get the address to localplayer. 
    return RPM< uintptr_t>(moduleBase + dwLocalPlayer);
}

uintptr_t getPlayer(int index) {  //Each player in the game has an index.
    return RPM< uintptr_t>(moduleBase + dwEntityList + index * 0x10); //We use index times 0x10 because the distance between each player 0x10.
}

int flag(uintptr_t player) {
    return RPM<int>(player + m_fFlags);
}

int getTeam(uintptr_t player) {
    return RPM<int>(player + m_iTeamNum);
}


int getCrosshairID(uintptr_t player) {
    return RPM<int>(player + m_iCrosshairId);
}

void Trigger()
{
    int CrosshairID = getCrosshairID(getLocalPlayer());
    int CrosshairTeam = getTeam(getPlayer(CrosshairID - 1));
    int LocalTeam = getTeam(getLocalPlayer());

    if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
    {
            mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
            Sleep(100); //Optional
    }
}

void BunnyHop()
{
    if (flag(getLocalPlayer()) == 257)
    {

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        Sleep(50);
    }
}

void WallHack()
{

}

DWORD WINAPI MainThread(HMODULE hModule)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Welcome\n";

    moduleBase = (DWORD)GetModuleHandle("client.dll");
    
    //---- VARIABLES PARA BUNNYHOP-----//
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.time = 0;
    inputs[0].ki.wVk = 0;
    inputs[0].ki.dwExtraInfo = 0;
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    inputs[0].ki.wScan = 0x1B;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.time = 0;
    inputs[1].ki.wVk = 0;
    inputs[1].ki.dwExtraInfo = 0;
    inputs[1].ki.wScan = 0x1B;
    inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    bool pause = false;

    while (true)
    {
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            pause = !pause;
        }
        if (!pause)
        {
            if (GetAsyncKeyState(VK_END) & 1)
            {
                break;
            }

            if (GetAsyncKeyState(VK_MENU /*alt key*/))
            {
                Trigger();
            }

            if (GetAsyncKeyState(VK_SPACE))
            {
                BunnyHop();
            }
        }
        else
        {
            Sleep(500);
        }
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}