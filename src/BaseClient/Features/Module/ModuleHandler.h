#pragma once
#include <vector>
#include <memory>
#include "Modules/Module.h"

#include "Modules/Misc/TestModule.h"
#include "Modules/Client/ArrayList.h"
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
	std::vector<Module*> getEnabledList() {
		std::vector<Module*> enabled;
		for (auto& mod : moduleList)
			if (mod->isEnabled()) enabled.push_back(mod);
		return enabled;
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
				if (dynamic_cast<T*>(it)) {
					logF("Found module : %s", typeid(T).name());
					return (T*)it;
				}
			}
			logF("Falid to find : %s", typeid(T).name());
		}
		logF("Falid to get the module");
		return nullptr;
	}

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<Module*> getByName(const std::string name) {
		if (isInitialized()) {
			std::string nameCopy = name;
			std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

			for (std::vector<Module*>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
				Module* mod = *it;
				std::string modNameCopy = mod->getName();
				std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
				if (modNameCopy == nameCopy)
					return std::optional<Module*>(mod);
			}
		}
		return nullptr;
	}


	void flushPresentEvent(ImDrawList* d) {
		for (Module* mod : moduleList) {
			if (mod->isEnabled()) 
				mod->onPresentEvent(d);
		}
	};

	void flushKeyFeedEvent(unsigned char key, bool isDown, bool& shouldCancel) {
		for (Module* mod : moduleList) {
			mod->onKeyFeedEvent(key, isDown, shouldCancel);
			if (key == mod->getKeybind() && isDown)
				mod->setEnabled(!mod->isEnabled());
		}
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
