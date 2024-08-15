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
public:
	static void Initialize();
	template <typename T>
	static void addModule() {
		static_assert(std::is_base_of<Module, T>::value, "It isn't module!");
		modules.moduleList.push_back(new T);
	};
	std::vector<Module*> getList() { return moduleList; }
	std::vector<Module*> getListByCategory(Category category) { 
		std::vector<Module*> categoryedList;
		for (auto& mod : moduleList) 
			if (mod->getCategory() == category) categoryedList.push_back(mod);
		return categoryedList;
	}
	static void Restore() { modules.moduleList.clear(); }
	static bool isInitialized() { return modules.initialized; };
	template <typename T>
	inline static T* get() {
		static_assert(std::is_base_of<Module, T>::value, "It isn't module!");
		if (isInitialized()) {
			for (auto& it : moduleList) {
				if (dynamic_cast<T*>(it.get()))
					return (T*)it;
			}
		}
		return nullptr;
	}
};