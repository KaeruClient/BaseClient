#include "dllmain.h"
//#include <cassert>

#include "include/MinHook.h"
#include "include/discord/cpp/discord.h"
#include "BaseClient/Memories/HookHandler.h"
#include "BaseClient/Features/Module/ModuleHandler.h"
#include "BaseClient/Features/Config/ConfigHandler.h"
#include "BaseClient/Features/Command/CommandHandler.h"

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

#include "Utils/TextColor.h"

bool dllmain::isRunning = true;
std::unique_ptr<discord::Core> core = {};
auto GetDllMod(void) -> HMODULE {
    MEMORY_BASIC_INFORMATION info;
    size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllMod, &info, sizeof(info));
    assert(len == sizeof(info));
    return len ? (HMODULE)info.AllocationBase : NULL;
}
DWORD WINAPI InitializeClient(LPVOID lpParam) {
    makeFolder();
    makeAssetsFolder("Configs");
    makeAssetsFolder("Assets");
    makeAssetsFolder("Logs");
    MH_Initialize();
    initColors();
    logF("Injected!");
    ModuleHandler::Initialize();
    CommandHandler::Initialize();
    HookHandler::Initialize();
    ConfigHandler::Initialize();
    winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {
        winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(
            Utils::stringToWstring(
                client.getName() + (std::string)" " + client.getVersion()
            )
        );
     });
    while (dllmain::isRunning) {
        if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::isKeyDown(VK_END)) {
            dllmain::isRunning = false;
            logF("Ejecting...");
        }
        Sleep(100);
    }

    ConfigHandler::Save();
    DirectXHook::shutdown();
    HookHandler::Restore();
    CommandHandler::Restore();
    ModuleHandler::Restore();
    winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(L""); });

    Sleep(1000);
    logF("Ejected!");

    FreeLibraryAndExitThread(GetDllMod(), 1);
    return 1;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
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

