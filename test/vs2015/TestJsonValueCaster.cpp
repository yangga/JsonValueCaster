// JsonValueCaster.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../JsonValueCaster.h"

#include <iostream>

int main()
{
	/// testing normal case
	std::cout << "= testing normal ===================================" << std::endl;
	Json::Value vMe;
	vMe["id"] = 1000;
	vMe["name"] = "Yangga";

	Json::Value vMom;
	vMom["id"] = 2000;
	vMom["name"] = "Jane";
	vMe["mom"] = vMom;

	Json::Value vKids;

	Json::Value vKid0;
	vKid0["id"] = 3000;
	vKid0["name"] = "CuteBoy";
	vKids.append(vKid0);

	Json::Value vKid1;
	vKid1["id"] = 3001;
	vKid1["name"] = "CuteGirl";
	vKids.append(vKid1);
	vMe["kids"] = vKids;

	try {
		Json::Caster c(vMe);

		// getting Integer type value
		auto id = c.get<int>("id");
		std::cout << id << std::endl;

		// getting String type value
		auto name = c.get<std::string>("name");
		std::cout << name << std::endl;

		// getting value from child object
		auto momName = c.get<std::string>("mom", "name");
		std::cout << momName << std::endl;

		// getting value from child array
		auto kid1Name = c.get<std::string>("kids", 0, "name");
		std::cout << kid1Name << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	/// testing boolean
	std::cout << "= testing boolean ===================================" << std::endl;

	Json::Value vBooleanTest;
	vBooleanTest["boolean"] = true;
	vBooleanTest["string"] = "true";
	vBooleanTest["number"] = 1;

	Json::CasterBoolean cb(vBooleanTest);
	std::cout << "case boolean - " << cb.get("boolean", false) << std::endl;
	std::cout << "case string - " << cb.get("string", false) << std::endl;
	std::cout << "case number - " << cb.get("number", false) << std::endl;

    return 0;
}
