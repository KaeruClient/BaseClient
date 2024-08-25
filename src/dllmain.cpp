#include "dllmain.h"
#include <Windows.h>
//#include <cassert>

#include "include/MinHook.h"
#include "include/discord/cpp/discord.h"
#include "BaseClient/Memories/HookHandler.h"
#include "BaseClient/Module/ModuleHandler.h"

#include <discord_rpc.h>
#include <discord_register.h>

bool dllmain::isRunning = true;
std::unique_ptr<discord::Core> dllmain::core = {};
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
    Discord_ClearPresence();
    Sleep(1000);
    logF("Ejected!");

    FreeLibraryAndExitThread(GetDllMod(), 1);
    return 1;
}


static const char* APPLICATION_ID = "345229890980937739";
static int FrustrationLevel = 0;
static int64_t StartTime;
static int SendPresence = 1;
static void updateDiscordPresence()
{
    if (SendPresence) {
        char buffer[256];
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state = "West of House";
        //sprintf(buffer, "Frustration level: %i", FrustrationLevel);
        discordPresence.details = "死ねゴミ";
        discordPresence.startTimestamp = StartTime;
        discordPresence.endTimestamp = time(0) + 5 * 60;
        discordPresence.largeImageKey = "canary-large";
        discordPresence.smallImageKey = "ptb-small";
        discordPresence.partyId = "party1234";
        discordPresence.partySize = 1;
        discordPresence.partyMax = 6;
        //discordPresence.partyPrivacy = DISCORD_PARTY_PUBLIC;
        discordPresence.matchSecret = "xyzzy";
        discordPresence.joinSecret = "join";
        discordPresence.spectateSecret = "look";
        discordPresence.instance = 0;
        Discord_UpdatePresence(&discordPresence);
    }
    else {
        Discord_ClearPresence();
    }
}

static void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("\nDiscord: connected to user %s#%s - %s\n",
        connectedUser->username,
        connectedUser->discriminator,
        connectedUser->userId);
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
    printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
    printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
    printf("\nDiscord: spectate (%s)\n", secret);
}
static void discordInit()
{
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    //handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

static void gameLoop()
{
    char line[512];
    char* space;

    StartTime = time(0);

    printf("You are standing in an open field west of a white house.\n");
    while (dllmain::isRunning) {
        updateDiscordPresence();
        
#ifdef DISCORD_DISABLE_IO_THREAD
        Discord_UpdateConnection();
#endif
        Discord_RunCallbacks();

        Sleep(100);
    }
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
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)EjectThread, NULL, NULL, NULL); 
    discord::Core* coreInstance;
    if (discord::Core::Create(1275439673631576086, (int)discord::CreateFlags::Default, &coreInstance) == discord::Result::Ok) {
        dllmain::core.reset(coreInstance);
        discord::Activity activity;
        activity.SetState("Test");
        activity.SetDetails("Unknown");
        dllmain::core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
    }
    while (dllmain::isRunning) {
        if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::isKeyDown(VK_END)) {
            dllmain::isRunning = false;
            logF("Ejecting...");
        }
        if (dllmain::core) dllmain::core->RunCallbacks();
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

