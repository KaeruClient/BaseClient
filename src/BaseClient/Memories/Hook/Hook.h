#pragma once
#include <memory>
#include <string_view>
#include "../../../include/MinHook.h"
#include "../../../include/xorstr.hpp"

#include "../../../SDK/SDK.h"
#include "../../../GameData.h"

#include "../../Module/ModuleHandler.h"

class Hook {
public:
    std::string name;
    Hook(const char* a) { name = a; };
    virtual bool Initialize() = 0;
};

class HookClass {
private:
    void* address = nullptr;
    void* origFunc = nullptr;
    bool valid = false;
public:
    template <typename T>
    HookClass(uintptr_t address, T* hooked) {
        this->address = (void*)address;
        this->valid = (MH_CreateHook((void*)address, hooked, &origFunc) == MH_OK);
    }
    ~HookClass() {
        MH_DisableHook(this->address);
        MH_RemoveHook(this->address);
    }
    template <typename TRet, typename... TArgs>
    inline auto* GetOrigFunc() {
        using Fn = TRet(__fastcall*)(TArgs...);
        return reinterpret_cast<Fn>(origFunc);
    };
    bool isValid() {
        return valid;
    };
};

template <typename T>
static inline bool CreateHook(std::unique_ptr<HookClass> &hook, uintptr_t address, T* funcPtr) {
    hook = std::make_unique<HookClass>(address, funcPtr);
    return hook->isValid();
}