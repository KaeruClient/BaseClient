#pragma once
#include "../Hook.h"

namespace ClientInstanceHook {
	class Update : public Hook {
	private:
		static inline std::unique_ptr<HookClass> funcPtr;
	public:
		Update() : Hook(xorstr_("ClientInstance::update")) {};
		static void handle(ClientInstance* _this, bool a2);
		bool Initialize() {
			return CreateHook(funcPtr, Signature::ClientInstance::update, handle);
		}
	};
}