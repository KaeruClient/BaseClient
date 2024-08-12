#pragma once
#include <memory>
#include <string_view>
#include "../../../include/MinHook.h"
#include "../../../include/xorstr.hpp"

#include "../Signature.h"
#include "../../../GameData.h"
class Hook {
public:
    std::string_view name;
    Hook(const char* a) { name = a; };
    virtual bool Initialize() = 0;
};

class FuncHook {
public:
    void* funcPtr;
    void* funcReal;

    FuncHook(void* func, void* hooked) {
        funcPtr = func;

        MH_STATUS ret = MH_CreateHook(func, hooked, &funcReal);
        if (ret == MH_OK && (__int64)func > 10) {
        }
    };

    FuncHook(uintptr_t func, void* hooked) {
        funcPtr = reinterpret_cast<void*>(func);

        MH_STATUS ret = MH_CreateHook(funcPtr, hooked, &funcReal);
        if (ret == MH_OK && (__int64)funcPtr > 10) {
        }
    };

    bool enableHook(bool enable = true) {
        if (funcPtr != nullptr) {
            if (enable) { return MH_EnableHook(funcPtr) == MH_OK; }
            else { return MH_DisableHook(funcPtr) == MH_OK; }
        }
        return false;
    }

    ~FuncHook() {
        Restore();
    }

    void Restore() {
        if (funcPtr != nullptr)
            MH_DisableHook(funcPtr);
    }

    template <typename TRet, typename... TArgs>
    inline auto* GetFastcall() {
        using Fn = TRet(__fastcall*)(TArgs...);
        return reinterpret_cast<Fn>(funcReal);
    };
}; //sorry i skidded horion ;(

__forceinline static bool CreateHook(std::unique_ptr<FuncHook>& breh, void* func, void* hooked) {
    breh = std::make_unique<FuncHook>(func, hooked);
    return breh->enableHook(true);
}