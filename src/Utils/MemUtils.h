#pragma once
#include <Windows.h>
#include <chrono>
#include <cstdint>
#include <psapi.h>
#include <string>
#include <vector>
#include <string_view>

#include <memory>
#include <libloaderapi.h>
#include <processthreadsapi.h>

#include <libhat.hpp>

#include "../include/xorstr.hpp"

class MemUtils {
public:

    static uintptr_t getBase() {
        static uintptr_t mc = reinterpret_cast<uintptr_t>(GetModuleHandleA("Minecraft.Windows.exe"));
        return mc;
    }

private:
    static unsigned long GetMinecraftSize() {
        HMODULE base = GetModuleHandleA("Minecraft.Windows.exe");
        if (base == nullptr) return 0;

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), base, &moduleInfo, sizeof(MODULEINFO))) 
            return 0;

        return moduleInfo.SizeOfImage;
    }

    static uintptr_t SlideAddress(uintptr_t offset)
    {
        return getBase() + offset;
    }
    static std::optional<uintptr_t> SigScanSafe(std::string_view signature)
    {
        const auto parsed = hat::parse_signature(signature);
        //if (!parsed.has_value()) Assert("Invalid signature! {:s}", signature);

        const auto begin = reinterpret_cast<std::byte*>(getBase());
        const auto end = begin + GetMinecraftSize();
        const auto result = hat::find_pattern(begin, end, parsed.value());

        if (!result.has_result()) return std::nullopt;
        return reinterpret_cast<uintptr_t>(result.get());
    }
public:
    static uintptr_t SigScan(std::string_view signature) {
        auto result = SigScanSafe(signature);
        //if (!result.has_value()) Assert("Failed to find signature \"{:s}\"", signature);
        return result.value();
    }
    static uintptr_t** getVtable(const char* szSignature, int offset) {
        uintptr_t** signatureOffset = 0x0;
        if (signatureOffset == 0x0) {
            uintptr_t sigOffset = SigScan(szSignature);
            if (sigOffset != 0x0) {
                int finalOffset = *reinterpret_cast<int*>((sigOffset + offset));
                signatureOffset = reinterpret_cast<uintptr_t**>(sigOffset + finalOffset + 7);
                return signatureOffset;
            }
        }
        return 0u;
    }


};

#pragma region DirectClassAccess

template <typename Ret, typename Type>
// The type and offset
Ret& DirectAccess(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

#define FAKE_FIELD(type, name)                                                                                       \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name()

#define BUILD_ACCESS(type, name, offset)                                                                             \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { return DirectAccess<type>(this, offset); }												 \
void set##name(type v) const { DirectAccess<type>(this, offset) = v; }

#pragma endregion

static __forceinline void patchBytes(unsigned char* dst, unsigned char* src, unsigned int size) {
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

static __forceinline void nopBytes(unsigned char* dst, unsigned int size) {
    DWORD oldprotect;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}

template <unsigned int IIdx, typename TRet, typename... TArgs>
static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
    using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
    return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
}
template <typename TRet, typename... TArgs>
static inline TRet getFastCall(uintptr_t address, TArgs... args) {
    using func_t = TRet(__fastcall*)(decltype(args));
    static func_t oFunc = reinterpret_cast<func_t>(address);
    return oFunc(args);
}
#define VtableScan(signature, offset) MemUtils::getVtable(xorstr_(signature), offset)
#define SigScan(signature) MemUtils::SigScan(xorstr_(signature))