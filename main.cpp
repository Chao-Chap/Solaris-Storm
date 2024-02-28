#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"

extern "C"
{

	//Include your animal mod - use an include statement like above.
	#include "crabtest.mini"

	//registering data functions. - Needs to exist.
	void (*RegisterDataFunc)(void* ptr);

	//NJS Type texture name and texture list. As is for old documentation.
	NJS_TEXNAME ExampleTex[10];
	NJS_TEXLIST example_texlist = { arrayptrandlength(ExampleTex) };

	//Define models
	ModelInfo* MDLExampleFruit;

	//Define Black Market Attributes
	BlackMarketItemAttributes BMExampleFruit = { 250, 100, 0, -1, -1, 0 };

	//Define Fruit Stats
	ChaoItemStats customFruitStats = { 10, 10, 0, 0, 0, 0, 0, 0, 0, 0 }; //mood, belly, swim, fly, run, power, stamina, luck, intel, unknown;

	//Custom Fruit Function - Changes ChaoData Stats on the last bite of the fruit.
	void __cdecl customFruitFunction(ChaoData* data, ObjectMaster* fruit)
	{
		data->data.Happiness += 0.1;
	}

	//main CWE Load function -- Important stuff like adding your CWE mod goes here
	void CWELoad(CWE_REGAPI* cwe_api)
	{
		cwe_api->RegisterChaoTexlistLoad("ExampleTex", &example_texlist);
		
		//define the fruit so it's obtainable
		int ExampleFruitID = cwe_api->RegisterChaoFruit(MDLExampleFruit->getmodel(), &example_texlist, &customFruitStats, &BMExampleFruit, customFruitFunction, "Example Fruit Name", "Example Fruit Description");

		//define an ID for the animal, so that it can be registered to the fruit
		int mini_ExampleAnimalID = cwe_api->AddChaoMinimal(&Crab_entry);

		//register the animal to the fruit using the API
		cwe_api->RegisterChaoMinimalFruit(ExampleFruitID, mini_ExampleAnimalID, 0, 50);
	}

	//initialization function - MUST exist in order to have CWE and SA2 see your mod
	__declspec(dllexport) void Init(const char* path)
	{
		HMODULE h = GetModuleHandle(L"CWE");

		std::string pathStr = std::string(path) + "\\";

		//Tell what models need to be associated to their variables here.
		MDLExampleFruit = new ModelInfo(pathStr + "ExampleFruit.sa2mdl");

		RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
		RegisterDataFunc(CWELoad);
	}
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
