#pragma once
#include <vector>
#include <memory>
#include "Modules/Module.h"

#include "Modules/Misc/TestModule.h"

extern class ModuleHandler modules;
class ModuleHandler {
private:
	std::vector<Module*> moduleList;
	bool initialized = false;
	
	template <typename T>
	static void add() {
		static_assert(std::is_base_of<Module, T>::value, "It isn't module!");
		modules.moduleList.push_back(new T);
		std::string name = typeid(T).name();
		logF("Module added | %s", name.substr(6, name.length()));
	};
public:
	static void Initialize();
	std::vector<Module*> getList() { return moduleList; }
	std::vector<Module*> getListByCategory(Category category) {
		std::vector<Module*> categoryedList;
		for (auto& mod : moduleList) 
			if (mod->getCategory() == category) categoryedList.push_back(mod);
		return categoryedList;
	}
	static void Restore() { 
		modules.moduleList.clear();
		logF("Restore ModuleHandler...");
	}
	static bool isInitialized() { return modules.initialized; };
	template <typename T>
	inline T* get() {
		if (isInitialized()) {
			for (auto& it : moduleList) {
				if (dynamic_cast<T*>(it))
					return (T*)it;
			}
		}
		return nullptr;
	}

	void SaveConfig(nlohmann::json* json) {
		for (Module* mod : moduleList) {
			mod->SaveConfig(json);
		}
	}
	void LoadConfig(nlohmann::json* json) {
		for (Module* mod : moduleList) {
			mod->LoadConfig(json);
		}
	}
};