#pragma once

#include <filesystem>
#include <istream>
#include <map>
#include <fstream>
#include <ostream>
#include <string>
#include <stdexcept>

namespace inifile {

using category_type = std::string;
using key_type = std::string;
using value_type = std::string;

using section_type = std::map<key_type, value_type>;

using settings = std::map<category_type, section_type>;

/// Loads a DOS-INI file @p _filename into @p _settings.
void load(settings& _settings, std::filesystem::path const& _filename);
void load(settings& _settings, std::istream& _input);

/// Stores settings into a file with DOS-INI syntax.
void save(settings const& _settings, std::filesystem::path const& _filename);
void save(settings const& _settings, std::ostream& _output);

// {{{ implementation save()
inline void save(settings const& _settings, std::ostream& _output)
{
	for (auto && [section_name, section] : _settings)
	{
		_output << '[' << section_name << ']' << '\n';
		for (auto && [key, value] : section)
			_output << key << '=' << value << '\n';
		_output << '\n';
	}
}

inline void save(settings const& _settings, std::filesystem::path const& _filename)
{
	auto output = std::ofstream{_filename.string()};
	if (!output.good())
		throw std::runtime_error("Could not open output file: " + _filename.string());
	save(_settings, output);
}
// }}}

// {{{ implementation load()
inline void load(settings& _settings, std::filesystem::path const& _filename)
{
	auto input = std::ifstream{_filename.string()};
	if (!input.good())
		throw std::runtime_error("Could not open input file: " + _filename.string());
	load(_settings, input);
}

inline void load(settings& _settings, std::istream& _input)
{
	// Reads a non-empty line (skipping empty lines until a non-empty line), or returns an empty line on EOF.
	auto const getline = [&]() -> std::string {
		while (_input.good())
		{
			auto line = std::string{};
			std::getline(_input, line);
			if (!line.empty())
				return line;
		}
		return {};
	};

	enum class State { SectionHeader, SectionBody };

	auto state = State::SectionHeader;
	auto currentSectionName = std::string{};

	for (auto line = getline(); _input.good() && !line.empty(); )
	{
		switch (state)
		{
			case State::SectionHeader:
				if (line.front() == '[' && line.back() == ']')
				{
					state = State::SectionBody;
					currentSectionName = line.substr(1, line.size() - 2);
					line = getline();
				}
				else
					throw std::runtime_error("Illegal line: \"" + line + "\"");
				break;
			case State::SectionBody:
				if (std::string::size_type i = line.find('='); i != std::string::npos)
				{
					_settings[currentSectionName][line.substr(0, i)] = line.substr(i + 1);
					line = getline();
				}
				else
					state = State::SectionHeader;
				break;
		}
	}
}
// }}}

} // end namespace
