#pragma once
#include"const.h"

struct SectionInfo {
	SectionInfo() {}
	~SectionInfo() { _section_datas.clear(); }
	SectionInfo(const SectionInfo& src) {
		this->_section_datas = src._section_datas;
	}
	SectionInfo& operator=(const SectionInfo& src) {
		if (this == &src) {
			return *this;
		}
		this->_section_datas = src._section_datas;
		return *this;
	}
	std::string operator[](const std::string& key) {
		if (_section_datas.find(key) == _section_datas.end())return "";
		return _section_datas[key];
	}
	std::map<std::string, std::string> _section_datas;
};

class ConfigMgr
{
public:
	
	~ConfigMgr() { _config_map.clear(); }
	SectionInfo operator[](const std::string& section) {
		if (_config_map.find(section) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[section];
	}
	ConfigMgr(const ConfigMgr& src) {
		this->_config_map = src._config_map;
	}
	ConfigMgr& operator=(const ConfigMgr& src) {
		if (this == &src)return *this;
		this->_config_map = src._config_map;

	}
	static ConfigMgr& GetIns() {
		static ConfigMgr cfg_mgr;
		return cfg_mgr;
	}
private:
	ConfigMgr();
	std::map<std::string, SectionInfo> _config_map;
};

