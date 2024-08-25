#pragma once
#include <windows.h>
#include <string>
#include <vector>

#include "../../../../Utils/JsonUtils.h"
#include "../../../../Utils/Utils.h"
#include "../../../../SDK/SDK.h"
#include <functional>


enum class Category {
	Combat = 0,
	Render = 1,
	Movement = 2,
	World = 3,
	Misc = 4,
	Client = 5
};

enum class SettingType {
	bool_t,
	int_t,
	float_t,
	text_t,
	keybind_t,
	settinggroup_t
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
	bool _bool;
	int _int;
	float _float;
	std::string _text;
	Keybind _keybind;
};


struct Setting {
	SettingType type;
	SettingValue* value;
	SettingValue max;
	SettingValue min;
	std::string name;
	std::string tooltip;
};
class Module {
private:
	std::vector<Setting*> settings;
	bool visible = false;
	bool enabled = false;
	Keybind keybind = 0x0;
	std::string tooltip = "";
	std::string moduleName = "";
	Category category;
public:
	Module(std::string moduleName, Category cat, Keybind keybind, const char* tooltip);
	virtual ~Module() {
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
		SettingValue* value = new SettingValue();
		if constexpr (std::is_same<T, bool>::value) {
			setting->type = SettingType::bool_t;
			value->_bool = *ptr;
		}
		else if constexpr (std::is_same<T, int>::value) {
			setting->type = SettingType::int_t;
			value->_int = (int)*ptr;
			setting->min._int = (int)min;
			setting->max._int = (int)max;
		}
		else if constexpr (std::is_same<T, float>::value) {
			setting->type = SettingType::float_t;
			value->_float = *ptr;
			setting->min._float = min;
			setting->max._float = max;
		}
		else if constexpr (std::is_same<T, std::string>::value || std::is_same<T, const char*>::value) {
			setting->type = SettingType::text_t;
			value->_text = *ptr;
		}
		else if constexpr (std::is_same<T, Keybind>::value) {
			setting->type = SettingType::keybind_t;
			value->_keybind = *ptr;
			setting->min._int = 0x0;
			setting->max._int = 0xBE;
		}
		setting->value = value;
		setting->name = name;
		setting->tooltip = tooltip;
		settings.push_back(setting);
		return setting;
	}

public:
	virtual void onUpdateEvent(ClientInstance* ci) {}; //ClientInstance::update 
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
	void SaveConfig(nlohmann::json* json) {
		nlohmann::json object;
		for (auto& setting : settings) {
			switch (setting->type)
			{
			case SettingType::bool_t:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_bool);
				break;
			case SettingType::int_t:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_int);
				break;
			case SettingType::float_t:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_float);
				break;
			case SettingType::text_t:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_text);
				break;
			case SettingType::keybind_t:
				JsonUtils::writeJson(&object, setting->name.c_str(), setting->value->_keybind.keybind);
				break;
			default:
				break;
			}
		}
		json->emplace(this->getName(), object);
	}
	void LoadConfig(nlohmann::json* json) {
		if (json->contains(this->getName())) {
			nlohmann::json object = json->at(this->getName());
			if (object.is_null())
				return;
			for (auto& setting : settings) {
				switch (setting->type)
				{
				case SettingType::bool_t:
					setting->value->_bool = JsonUtils::readJson<bool>(&object, setting->name.c_str());
					break;
				case SettingType::int_t:
					setting->value->_int = JsonUtils::readJson<int>(&object, setting->name.c_str());
					break;
				case SettingType::float_t:
					setting->value->_float = JsonUtils::readJson<float>(&object, setting->name.c_str());
					break;
				case SettingType::text_t:
					setting->value->_text = JsonUtils::readJson<std::string>(&object, setting->name.c_str());
					break;
				case SettingType::keybind_t:
					setting->value->_keybind.keybind = JsonUtils::readJson<int>(&object, setting->name.c_str());
					break;
				default:
					break;
				}
			}

		}
	}
};