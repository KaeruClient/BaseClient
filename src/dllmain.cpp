#include "dllmain.h"
#include <Windows.h>

#include "include/MinHook.h"
#include "BaseClient/Memories/HookManager.h"
#include <cassert>
bool dllmain::isRunning = true;
auto GetDllMod(void) -> HMODULE {
    MEMORY_BASIC_INFORMATION info;
    size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllMod, &info, sizeof(info));
    assert(len == sizeof(info));
    return len ? (HMODULE)info.AllocationBase : NULL;
}

DWORD WINAPI EjectThread(LPVOID a) {
    while (dllmain::isRunning) Sleep(100);
    HookManager::Restore();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    Sleep(1000);
    FreeLibraryAndExitThread(GetDllMod(), 1);
    return 1;
}

DWORD WINAPI InitializeClient(LPVOID lpParam) {
    makeFolder();
    makeAssetsFolder("Configs");
    makeAssetsFolder("Assets");
    makeAssetsFolder("Logs");
    MH_Initialize();
    HookManager::Initialize();
    while (dllmain::isRunning) {
        if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::isKeyDown(VK_END))
        {
            dllmain::isRunning = false;
        }
        Sleep(100);
    }

    return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitializeClient, hModule, NULL, NULL);
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

