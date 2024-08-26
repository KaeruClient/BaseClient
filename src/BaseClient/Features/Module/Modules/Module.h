#pragma once
#include <windows.h>
#include <string>
#include <vector>

#include "../../../../Utils/JsonUtils.h"
#include "../../../../Utils/Utils.h"
#include "../../../../SDK/SDK.h"
#include <functional>

#pragma warning(disable: 4624)

enum class Category {
	Combat = 0,
	Render = 1,
	Movement = 2,
	World = 3,
	Misc = 4,
	Client = 5
};

enum class SettingType {
	BOOL,
	INT,
	FLOAT,
	TEXT,
	KEYBIND
};

struct Keybind {//shit code
	int keybind;
	bool isCaptureingKey = false; // for clickgui
	Keybind() { this->keybind = 0x0; };
	Keybind(int keybind) {
		this->keybind = keybind;
	}
};

struct SettingValue {
	union {
		bool _bool;
		int _int;
		float _float;
		std::string _text;
		Keybind _keybind;
	};
	SettingValue() {};
};


struct Setting {
	SettingType type;
	SettingValue* value = nullptr;
	SettingValue* max = nullptr;
	SettingValue* min = nullptr;
	std::string name;
	std::string tooltip;
};
class Module {
private:
	std::vector<Setting*> settings;
	bool visible = true;
	bool enabled = false;
	Keybind keybind = 0x0;
	std::string tooltip = "";
	std::string moduleName = "";
	Category category;
public:
	Module(std::string moduleName, Category cat, Keybind keybind, const char* tooltip);
	~Module() {
		for (auto it = this->settings.begin(); it != this->settings.end(); it++) {
			delete& (*it)->max;
			delete& (*it)->min;
			delete* it;
		}
		settings.clear();
	};
	const Category getCategory() { return category; };
	std::vector<Setting*>* getSettings() {
		return &settings;
	}
	bool isVisible() { return visible; };
	const char* getName() {
		return moduleName.c_str();
	}; 
	virtual std::string getMode() { return ""; };
	template <typename T>
	inline Setting* registerSetting(std::string name, std::string tooltip, T* ptr, float min = 0, float max = 0) {
		static_assert
			(
				std::is_same<T, bool>::value ||
				std::is_same<T, int>::value ||
				std::is_same<T, float>::value ||
				std::is_same<T, Keybind>::value,
				"This type cannot be used!"
				);
		Setting* setting = new Setting();
		SettingValue* maxVal = new SettingValue();
		SettingValue* minVal = new SettingValue();
		if constexpr (std::is_same<T, bool>::value) {
			setting->type = SettingType::BOOL;
		}
		else if constexpr (std::is_same<T, int>::value) {
			setting->type = SettingType::INT;
			minVal->_int = (int)min;
			maxVal->_int = (int)max;
		}
		else if constexpr (std::is_same<T, float>::value) {
			setting->type = SettingType::FLOAT;
			minVal->_float = min;
			maxVal->_float = max;
		}
		else if constexpr (std::is_same<T, std::string>::value || std::is_same<T, const char*>::value) {
			setting->type = SettingType::TEXT;
		}
		else if constexpr (std::is_same<T, Keybind>::value) {
			setting->type = SettingType::KEYBIND;
			minVal->_int = 0x0;
			maxVal->_int = 0xBE;
		}
		setting->value = reinterpret_cast<SettingValue*>(ptr);
		setting->max = maxVal;
		setting->min = minVal;
		setting->name = name;
		setting->tooltip = tooltip;
		settings.push_back(setting);
		return setting;
	}

public:
	virtual void onKeyFeedEvent(unsigned char key, bool isDown, bool& shouldCancel) {}
	virtual void onUpdateEvent(ClientInstance* ci) {}; //ClientInstance::update 
	virtual void onPresentEvent(ImDrawList* d) {}
	virtual void onEnableEvent() {}
	virtual void onDisableEvent() {}
	void setEnabled(bool enabled) {
		if (this->enabled == enabled) return;
		this->enabled = enabled;
		if (enabled) onEnableEvent();
		else onDisableEvent();
	}
	void setKeybind(int keybind) { this->keybind = keybind; };
	int getKeybind() { return keybind.keybind; }
	Keybind* KeybindData() { return &keybind; }
	bool isEnabled() { return enabled; }
	void SaveConfig(nlohmann::json* json) {
		logF("Saving %s's config...", this->getName());
		nlohmann::json object;
		for (auto& setting : settings) {
			switch (setting->type)
			{
			case SettingType::BOOL:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_bool);
				logF("Saved boolean");
				logF("name:%s, value:%i", setting->name.c_str(), setting->value->_bool);
				break;
			case SettingType::INT:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_int);
				logF("Saved intger");
				logF("name:%s, value:%i", setting->name.c_str(), setting->value->_int);
				break;
			case SettingType::FLOAT:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_float);
				logF("name:%s, value:%.5f", setting->name.c_str(), setting->value->_float);
				logF("Saved float");
				break;
			case SettingType::TEXT:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_text);
				logF("Saved text");
				logF("name:%s, value:%s", setting->name.c_str(), setting->value->_text.c_str());
				break;
			case SettingType::KEYBIND:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_keybind.keybind);
				logF("Saved keybind");
				logF("name:%s, value:%i", setting->name.c_str(), setting->value->_keybind.keybind);
				break;
			default:
				break;
			}
		}
		JsonUtils::writeJson(json, this->getName(), object);
	}
	void LoadConfig(nlohmann::json* json) {
		if (json->contains(this->getName())) {
			auto object = json->at(this->getName());
			if (object.is_null()) {
				logF("Config load error reason 1");
				return;
			}
			logF("Loading %s's config...", this->getName());

			for (auto it = this->settings.begin(); it != this->settings.end(); ++it) {
				Setting* setting = *it;
				switch (setting->type)
				{
				case SettingType::BOOL:
					setting->value->_bool = JsonUtils::readJson<bool>(object, setting->name.c_str());
					logF("Loaded boolean");
					logF("name:%s, value:%d", setting->name.c_str(), setting->value->_bool);
					break;
				case SettingType::INT:
					setting->value->_int = JsonUtils::readJson<int>(object, setting->name.c_str());
					logF("Loaded intger");
					logF("name:%s, value:%d", setting->name.c_str(), setting->value->_int);
					break;
				case SettingType::FLOAT:
					setting->value->_float = JsonUtils::readJson<float>(object, setting->name.c_str());
					logF("name:%s, value:%.5f", setting->name.c_str(), setting->value->_float);
					logF("Loaded float");
					break;
				case SettingType::TEXT:
					setting->value->_text = JsonUtils::readJson<std::string>(object, setting->name.c_str());
					logF("Loaded text");
					logF("name:%s, value:%d", setting->name.c_str(), setting->value->_text);
					break;
				case SettingType::KEYBIND:
					setting->value->_keybind.keybind = JsonUtils::readJson<int>(object, setting->name.c_str());
					logF("Loaded keybind");
					logF("name:%s, value:%d", setting->name.c_str(), setting->value->_keybind.keybind);
					break;
				default:
					logF("Config load error reason 2");
					break;
				}
			}

		}
	}
};