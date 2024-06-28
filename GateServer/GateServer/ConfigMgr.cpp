#include "ConfigMgr.h"
#include"const.h"
#include<fstream>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/ini_parser.hpp>
#include<map>
ConfigMgr::ConfigMgr()
{
	boost::filesystem::path current_path = boost::filesystem::current_path();
	boost::filesystem::path config_path = current_path / "config.ini";

	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	for (const auto& section_pair : pt)//这一层遍历是读ini的 GateServer和port
	{
		const std::string& section_name = section_pair.first;
		const boost::property_tree::ptree& section_ptree = section_pair.second;

		std::map<std::string, std::string> section_config;
		for (const auto& ket_value_pair:section_ptree) {//这一层是遍历数值,比如key是port，value是8080
			const std::string& key = ket_value_pair.first;
			const std::string& value = ket_value_pair.second.get_value<std::string>();
			section_config[key] = value;
		}
		SectionInfo sectioninfo;
		sectioninfo._section_datas = section_config;
		_config_map[section_name] = sectioninfo;
	}	
	for (const auto& section_entry : _config_map) {
		const std::string& section_name = section_entry.first;
		const SectionInfo& section_config = section_entry.second;
		std::cout << "[" << section_name << "]" << std::endl;
		for (const auto& key_value_pair : section_config._section_datas) {
			std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
		}
	}
}
