#include "dllmain.h"
//#include <cassert>

#include "include/MinHook.h"
#include "include/discord/cpp/discord.h"
#include "BaseClient/Memories/HookHandler.h"
#include "BaseClient/Features/Module/ModuleHandler.h"
#include "BaseClient/Features/Config/ConfigHandler.h"

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

bool dllmain::isRunning = true;
std::unique_ptr<discord::Core> core = {};
auto GetDllMod(void) -> HMODULE {
    MEMORY_BASIC_INFORMATION info;
    size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetDllMod, &info, sizeof(info));
    assert(len == sizeof(info));
    return len ? (HMODULE)info.AllocationBase : NULL;
}

DWORD WINAPI EjectThread(LPVOID a) {
    while (dllmain::isRunning) Sleep(100);
    DirectXHook::shutdown();
    HookHandler::Restore();
    ModuleHandler::Restore();

    auto disp = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher();
    disp.RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(L"");});

    Sleep(1000);
    logF("Ejected!");

    FreeLibraryAndExitThread(GetDllMod(), 1);
    return 1;
}

DWORD WINAPI InitializeClient(LPVOID lpParam) {
    makeFolder();
    makeAssetsFolder("Configs");
    makeAssetsFolder("Assets");
    makeAssetsFolder("Logs");
    MH_Initialize();
    logF("Injected!");
    ModuleHandler::Initialize();
    HookHandler::Initialize();
    ConfigHandler::Initialize();
    auto disp = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher();
    disp.RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {
        winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(
            Utils::stringToWstring(
                client.getName() + (std::string)" " + client.getVersion()
            )
        );
     });
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EjectThread, NULL, NULL, NULL); 
    /*discord::Core* coreInstance;
    if (discord::Core::Create(1275439673631576086, (uint64_t)discord::CreateFlags::Default, &coreInstance) == discord::Result::Ok) {
        core.reset(coreInstance);
        discord::Activity activity;
        activity.SetState("Test");
        activity.SetDetails("Unknown");
        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
    }*/
    while (dllmain::isRunning) {
        if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::isKeyDown(VK_END)) {
            dllmain::isRunning = false;
            logF("Ejecting...");
        }
        //if (core) core->RunCallbacks();
        Sleep(100);
    }
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

