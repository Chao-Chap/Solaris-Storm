#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"
#include <ctime>
#include "Trampoline.h"
int stats[] = { 100, 40, 20, -20, -40, -100 };

extern "C"
{
	int mini_IblisID;
	int mini_MephID;
	int mini_SolarisID;
	
	struct al_entry_work
	{
		unsigned __int16 category;
		unsigned __int16 num;
		unsigned __int16 kind;
		unsigned __int16 flag;
		void* pSaveInfo;
		int CommuID;
		NJS_POINT3 pos;
		Rotation ang;
		float radius;
		float offset;
		float CamDist;
		__int16 command;
		__int16 command_value;
		int state;
		ObjectMaster* tp;
		al_entry_work* pCommu;
		al_entry_work* pLockOn;
	};
	
	const int GetChaoObjectPtr = 0x00530410;
	ObjectMaster* GetChaoObject(int a1, int a2)
	{
		ObjectMaster* val;
		__asm
		{
			mov eax, a1
			mov edi, a2
			call GetChaoObjectPtr
			mov val, eax
		}
		return val;
	}
	
	DataArray(int, nbWorldEntry, 0x1DC0F80, 10);

	FunctionPointer(ObjectMaster *, AL_MinimalExecutor_Load_, (Uint8 a1, NJS_VECTOR *a2, int a3, void *a4, int a5), 0x548D30);
	ThiscallFunctionPointer(void*, AL_GetNewItemSaveInfo, (int a1), 0x52F9E0);

	void __cdecl ALW_Control_Main_Hook(ObjectMaster* a1);
	Trampoline ALW_Control_t(0x00530850, 0x00530859, ALW_Control_Main_Hook);
	void __cdecl ALW_Control_Main_Hook(ObjectMaster* a1)
	{
		const auto original = reinterpret_cast<decltype(ALW_Control_Main_Hook)*>(ALW_Control_t.Target());
		original(a1);

		if (a1->Data1.Entity->Action == 2) {
			bool foundSolaris = false;
			bool foundIblis = false;
			bool foundMeph = false;
			ObjectMaster* pIblis = NULL;
			for(int i = 0; i < nbWorldEntry[2]; i++){
				ObjectMaster* pAnimal = GetChaoObject(2, i);
				if(!pAnimal) continue;

				if(pAnimal->Data1.Entity->Index == mini_IblisID) {
					foundIblis = true;
					pIblis = pAnimal;
				}
				
				if(pAnimal->Data1.Entity->Index == mini_MephID) {
					foundMeph = true;
				}

				if(pAnimal->Data1.Entity->Index == mini_SolarisID) {
					foundSolaris = true;
					PlayJingle("chao_g_born_c.adx");
				}
			}
			if(foundIblis && foundMeph && !foundSolaris) {
				void* save = AL_GetNewItemSaveInfo(2);
				if(save) {
					NJS_VECTOR velo = {0, 0.25f, 0};
					AL_MinimalExecutor_Load_(
						mini_SolarisID,
						&pIblis->Data1.Entity->Position,
						NJM_DEG_ANG(rand() * 360.f),
						&velo,						
						(int)save
					);
				}
			}
		}
	}	
	
	//initialization function - MUST exist in order to have CWE and SA2 see your mod

	//Include your animal mod - use an include statement like above.
	#include "taker.mini"
	#include "golem.mini"
	#include "tricker.mini"
	#include "titan.mini"
	#include "biter.mini"
	#include "stalker.mini"
	#include "crawler.mini"
	#include "gazer.mini"
	#include "iblis.mini"
	#include "mephilies.mini"
	#include "solaris.mini"
	//registering data functions. - Needs to exist.
	void (*RegisterDataFunc)(void* ptr);

	//NJS Type texture name and texture list. As is for old documentation.
	NJS_TEXNAME IblisCoreTex[5];
	NJS_TEXLIST Iblis_texlist = { arrayptrandlength(IblisCoreTex) };
	NJS_TEXNAME ScepterTex[5];
	NJS_TEXLIST Scepter_texlist = { arrayptrandlength(ScepterTex) };

	//Define models
	ModelInfo* IblisCoreMDL;
	ModelInfo* ScepterMDL;

	//Define Black Market Attributes
	BlackMarketItemAttributes BMIblisFruit = { 500, 250, 0, -1, -1, 0 };
	BlackMarketItemAttributes BMMephFruit = { 500, 250, 0, -1, -1, 0 };

	//Define Fruit Stats
	

	
	ChaoItemStats IblisPower = { 10, 10, 50, 50, 50, 50, 50, 0, 0, 0 }; //mood, belly, swim, fly, run, power, stamina, luck, intel, unknown;
	
//Custom Fruit Function - Changes ChaoData Stats on the last bite of the fruit.
	void __cdecl IblisLifespan(ChaoData* data, ObjectMaster* fruit)
	{
		// changed this from 0.1 to 10, 0.1 wouldnt do anything
		// 1 is too little (in my opinion) so i made it 10
		data->data.Lifespan += 10;
	}

	//main CWE Load function -- Important stuff like adding your CWE mod goes here
	void CWELoad(CWE_REGAPI* cwe_api)
	{
		ChaoItemStats ScepterFlux = { stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], 50, 0 }; //mood, belly, swim, fly, run, power, stamina, luck, intel, unknown;
		cwe_api->RegisterChaoTexlistLoad("IblisCoreTex", &Iblis_texlist);
		cwe_api->RegisterChaoTexlistLoad("scepter", &Scepter_texlist);
		//define the fruit so it's obtainable
	
		int IblisCoreID = cwe_api->RegisterChaoFruit(IblisCoreMDL->getmodel(), &Iblis_texlist , &IblisPower, &BMIblisFruit, IblisLifespan, "Iblis Core", "A core that summons the minions of the Iblis.");
		int ScepterID = cwe_api->RegisterChaoFruit(ScepterMDL->getmodel(), &Scepter_texlist, &ScepterFlux, &BMMephFruit, 0 , "Scepter of Darkness", "A scepter said to hold dark power.");
		//define an ID for the animal, so that it can be registered to the fruit
		int mini_TakerID = cwe_api->AddChaoMinimal(&taker_entry);
		int mini_GolemID = cwe_api->AddChaoMinimal(&golem_entry);
		int mini_TrickerID = cwe_api->AddChaoMinimal(&tricker_entry);
		int mini_TitanID = cwe_api->AddChaoMinimal(&titan_entry);
		int mini_BiterID = cwe_api->AddChaoMinimal(&biter_entry);
		int mini_StalkerID = cwe_api->AddChaoMinimal(&stalker_entry);
		int mini_CrawlerID = cwe_api->AddChaoMinimal(&crawler_entry);
		int mini_GazerID = cwe_api->AddChaoMinimal(&gazer_entry);
		mini_IblisID = cwe_api->AddChaoMinimal(&iblis_entry);
		mini_MephID = cwe_api->AddChaoMinimal(&mephilies_entry);
		mini_SolarisID = cwe_api->AddChaoMinimal(&solaris_entry);

			cwe_api->RegisterChaoMinimalFruit(IblisCoreID, mini_TakerID, 0, 30);
			cwe_api->RegisterChaoMinimalFruit(IblisCoreID, mini_BiterID, 31, 54);
			cwe_api->RegisterChaoMinimalFruit(IblisCoreID, mini_GolemID, 75, 95);
			cwe_api->RegisterChaoMinimalFruit(IblisCoreID, mini_CrawlerID, 55, 75);
			cwe_api->RegisterChaoMinimalFruit(IblisCoreID, mini_IblisID, 95, 100);

		
	
			cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_TrickerID, 0, 30);
			cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_StalkerID, 31, 54);
			cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_TitanID, 75, 95);
			cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_GazerID, 55, 75);
			cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_MephID, 95, 100);

	

	}

	__declspec(dllexport) void Init(const char* path)
	{
		srand(time(0));
		HMODULE h = GetModuleHandle(L"CWE");

		std::string pathStr = std::string(path) + "\\";

		//Tell what models need to be associated to their variables here.
		IblisCoreMDL = new ModelInfo(pathStr + "IblisCore.sa2mdl");
		ScepterMDL = new ModelInfo(pathStr + "Scepter.sa2mdl");

		RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
		RegisterDataFunc(CWELoad);
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
