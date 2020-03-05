#include <catch2/catch.hpp>
#include <inifile.h>
#include <iostream>
#include <sstream>

using namespace std::literals;

TEST_CASE("inifile.save")
{
	auto const settings = inifile::settings{
		{"Section 1", {
			{"Key 1", "Value 1"},
			{"Key 2", "Value 2"}
		}},
		{"Area52", {
			{"Alien_Count", "52"},
		}}
	};

	auto const expectedOutput =
		"[Area52]\n"
		"Alien_Count=52\n"
		"\n"
		"[Section 1]\n"
		"Key 1=Value 1\n"
		"Key 2=Value 2\n"
		"\n"s;

	auto sstr = std::stringstream{};
	inifile::save(settings, sstr);
	REQUIRE(sstr.str() == expectedOutput);
}

TEST_CASE("inifile.load")
{
	auto const settingsText =
		"[Area52]\n"
		"Alien_Count=52\n"
		"\n"
		"[Section 1]\n"
		"Key 1=Value 1\n"
		"Key 2=Value 2\n"
		"\n"s;

	auto sstr = std::istringstream{settingsText};
	auto settings = inifile::settings{};
	inifile::load(settings, sstr);

	REQUIRE(settings.size() == 2);

	CHECK(settings["Area52"]["Alien_Count"] == "52");
	REQUIRE(settings.count("Area52") == 1);

	REQUIRE(settings.count("Section 1") == 1);
	CHECK(settings["Section 1"]["Key 1"] == "Value 1");
	CHECK(settings["Section 1"]["Key 2"] == "Value 2");
}
