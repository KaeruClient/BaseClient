#pragma once
#include <windows.h>
#include <string>
#include <vector>

#include "../../../Utils/Utils.h"
#include "../../../SDK/SDK.h"


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
	keybind_t
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
	Keybind _keybind;
	std::string _text;
};

struct Setting {
	SettingType type;
	SettingValue* value;
	SettingValue max;
	SettingValue min;
	std::string name;
};
class Module {
private:
	std::vector<Setting*> settings;
	bool visible = false;
	bool enabled = false;
	Keybind keybind = 0x0;
	std::string tooltip = "";
	Category category;
public:
	Module(Category cat, Keybind keybind, const char* tooltip);
	const Category getCategory() { return category; };
	std::vector<Setting*>* getSettings() {
		return &settings;
	}
	bool isVisible() { return visible; };
	const char* getName() { 
		const char* name = typeid(*this).name();
		return name;
	}; //ƒNƒ‰ƒX‚Ì–¼‘O‚ð•Ô‚·
	virtual std::string getMode() { return ""; };
	template <typename T>
	inline void addSetting(std::string name, T* ptr, float min = 0, float max = 0) { 
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
			setting->min._float= min;
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
		settings.push_back(setting);
	}
	virtual void onEnable() {}
	virtual void onDisable() {}
	void setEnabled(bool enabled) {
		if (this->enabled == enabled) return;
		this->enabled = enabled;
		if (enabled) onEnable();
		else onDisable();
	}
	void setKeybind(int keybind) { this->keybind = keybind; };
	int getKeybind() { return keybind.keybind; }
	Keybind* KeybindData() { return &keybind; }// for clickgui lol
};