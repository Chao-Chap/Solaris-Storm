#include "pch.h"
#include "cwe_api.h"
#include "ModelInfo.h"
#include <ctime>
#include "Trampoline.h"
int stats[] = { 100, 40, 20, -20, -40, -100 };

extern "C"
{

	//registering data functions. - Needs to exist.
	void (*RegisterDataFunc)(void* ptr);
	__declspec(dllexport) void(__cdecl* ALS_LensSpecial)(ObjectMaster*, ObjectMaster*);
	NJS_TEXNAME SolEye[10];
	NJS_TEXLIST Soleye_texlist = { arrayptrandlength(SolEye) };

	NJS_TEXNAME MephEye[10];
	NJS_TEXLIST Mepheye_texlist = { arrayptrandlength(MephEye) };

	NJS_TEXNAME IblisEye[10];
	NJS_TEXLIST Ibliseye_texlist = { arrayptrandlength(IblisEye) };

	NJS_TEXNAME Solguard[10];
	NJS_TEXLIST solguard_texlist = { arrayptrandlength(Solguard) };

	NJS_TEXNAME sonicmanhat[10];
	NJS_TEXLIST sonicmanhat_texlist = { arrayptrandlength(sonicmanhat) };

	NJS_TEXNAME gunhat[10];
	NJS_TEXLIST gunhat_texlist = { arrayptrandlength(gunhat) };
	int SolarisLensID;
	int MephLensID;
	int IblisLensID;
	int WillEyeColorID;
	int MephEyeColorID;
	int IblisEyeColorID;
	int SolguardID;
	int sonicmanhatID;
	int gunhatID;
	ModelInfo* MDLSolarisWill;
	ModelInfo* MDLIblisEye;
	ModelInfo* MDLIbliseye;
	ModelInfo* MDLforeboding;
	ModelInfo* MDLSolarisChao;
	ModelInfo* MDLIblisChao;
	ModelInfo* MDLMephilesChao;
	ModelInfo* MDLSolGuard;
	ModelInfo* MDLgunhat;
	ModelInfo* MDLsonicmanhat;
	int mini_IblisID;
	int mini_MephID;
	int mini_SolarisID;
	static bool IblisChaoEvo(ObjectMaster* tp)
	{
		Uint8 eye_color = *(Uint8*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x59A);

		if (tp->Data1.Chao->ChaoDataBase_ptr->SA2BFaceType == mini_IblisID and eye_color == IblisEyeColorID and (tp->Data1.Chao->ChaoDataBase_ptr->Reincarnations >= 2))
			return true;
		else
			return false;
	}
	static bool MephilesChaoEvo(ObjectMaster* tp)
	{
		Uint8 eye_color = *(Uint8*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x59A);
		if (tp->Data1.Chao->ChaoDataBase_ptr->SA2BForeheadType == mini_MephID and eye_color == MephEyeColorID and (tp->Data1.Chao->ChaoDataBase_ptr->Reincarnations >= 2))
			return true;
		else
			return false;
	}
	static bool SolarisChaoEvo(ObjectMaster* tp)
	{
		Uint8 eye_color = *(Uint8*)((int)(tp->Data1.Chao->ChaoDataBase_ptr) + 0x59A);
		if (tp->Data1.Chao->ChaoDataBase_ptr->SA2BForeheadType == mini_SolarisID and eye_color == WillEyeColorID and (tp->Data1.Chao->ChaoDataBase_ptr->Reincarnations >= 2))

			return true; 
		else
			return false;
	}
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

	FunctionPointer(ObjectMaster*, AL_MinimalExecutor_Load_, (Uint8 a1, NJS_VECTOR* a2, int a3, void* a4, int a5), 0x548D30);
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
			for (int i = 0; i < nbWorldEntry[2]; i++){
				ObjectMaster* pAnimal = GetChaoObject(2, i);
				if (!pAnimal) continue;

				if (pAnimal->Data1.Entity->Index == mini_IblisID) {
					foundIblis = true;
					pIblis = pAnimal;
				}

				if (pAnimal->Data1.Entity->Index == mini_MephID) {
					foundMeph = true;
				}

				if (pAnimal->Data1.Entity->Index == mini_SolarisID) {
					foundSolaris = true;
					PlayJingle("chao_g_born_c.adx");
				}
			}
			if (foundIblis && foundMeph && !foundSolaris) {
				void* save = AL_GetNewItemSaveInfo(2);
				if (save) {
					NJS_VECTOR velo = { 0, 0.25f, 0 };
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



	//Lens function - Needed for CWE to recognize the lens.

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
		BlackMarketItemAttributes BMSolGuard = { 500, 250, 0, -1, -1, 0 };
		BlackMarketItemAttributes BMsonicmanhat = { 200, 100, 0, -1, -1, 0 };
		BlackMarketItemAttributes BMgunhat = { 200, 100, 0, -1, -1, 0 };
		cwe_api->RegisterChaoTexlistLoad("SolGuard", &solguard_texlist);
		cwe_api->RegisterChaoTexlistLoad("GunHelmet", &gunhat_texlist);
		cwe_api->RegisterChaoTexlistLoad("sonicmanhat", &sonicmanhat_texlist);
		cwe_api->RegisterChaoTexlistLoad("willofsolaris", &Soleye_texlist);
		cwe_api->RegisterChaoTexlistLoad("forebodingshadow", &Mepheye_texlist);
		cwe_api->RegisterChaoTexlistLoad("IblisEyes", &Ibliseye_texlist);
		ChaoItemStats ScepterFlux = { stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], stats[rand() % LengthOfArray(stats)], 50, 0 }; //mood, belly, swim, fly, run, power, stamina, luck, intel, unknown;
		cwe_api->RegisterChaoTexlistLoad("IblisCoreTex", &Iblis_texlist);
		cwe_api->RegisterChaoTexlistLoad("scepter", &Scepter_texlist);
		//define the fruit so it's obtainable
		BlackMarketItemAttributes BMSolLens = { 1000, 500, 0, -1, -1, 0 };
		BlackMarketItemAttributes BMMephLens = { 1000, 500, 0, -1, -1, 0 };
		BlackMarketItemAttributes BMIblisLens = { 1000, 500, 0, -1, -1, 0 };
		int IblisCoreID = cwe_api->RegisterChaoFruit(IblisCoreMDL->getmodel(), &Iblis_texlist, &IblisPower, &BMIblisFruit, IblisLifespan, "Iblis Core", "A core that summons the minions of the Iblis.");
		int ScepterID = cwe_api->RegisterChaoFruit(ScepterMDL->getmodel(), &Scepter_texlist, &ScepterFlux, &BMMephFruit, 0, "Scepter of Darkness", "A scepter said to hold dark power.");
		//Register your lens to the black market:
	SolguardID =cwe_api->RegisterChaoAccessory(Head, MDLSolGuard->getmodel(), &solguard_texlist, &BMSolGuard, "Soleanna Royal Guard Cap", "Part of the uniform for the Soleannaen Royal Guard");
	sonicmanhatID = cwe_api->RegisterChaoAccessory(Head, MDLsonicmanhat->getmodel(), &sonicmanhat_texlist, &BMsonicmanhat, "Sonic Man's Hat", "Little is known about the mysterious sonic man, but what is apparent is his style.");
	gunhatID = cwe_api->RegisterChaoAccessory(Head, MDLgunhat->getmodel(), &gunhat_texlist, &BMgunhat, "G.U.N. Soldier Helmet", "Are we the baddies?");
		SolarisLensID = cwe_api->RegisterChaoSpecial(MDLSolarisWill->getmodel(), &Soleye_texlist, &BMSolLens, ALS_LensSpecial, NULL, "The Will of Solaris", "The very will of an ancient god.", false);
		MephLensID = cwe_api->RegisterChaoSpecial(MDLforeboding->getmodel(), &Mepheye_texlist, &BMMephLens, ALS_LensSpecial, NULL, "Foreboding Shadow", "A very eerie shadow", false);
		IblisLensID = cwe_api->RegisterChaoSpecial(MDLIblisEye->getmodel(), &Ibliseye_texlist, &BMIblisLens, ALS_LensSpecial, NULL, "Flames of Disaster", "Wade Whipple Approved", false);
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

		//Associate the lens as a custom eye color:
	WillEyeColorID = cwe_api->RegisterEyeColor("willofsolaris", &Soleye_texlist, SolarisLensID);
	MephEyeColorID = cwe_api->RegisterEyeColor("forebodingshadow", &Mepheye_texlist, MephLensID);
	IblisEyeColorID = cwe_api->RegisterEyeColor("IblisEyes", &Ibliseye_texlist, IblisLensID);
		cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_TrickerID, 0, 30);
		cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_StalkerID, 31, 54);
		cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_TitanID, 75, 95);
		cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_GazerID, 55, 75);
		cwe_api->RegisterChaoMinimalFruit(ScepterID, mini_MephID, 95, 100);

		CWE_API_CHAO_DATA CharChao_SolarisData =
		{
			MDLSolarisChao->getmodel(), //pObject
			{ 0 },                        //pSecondEvoList[5]

				"SolarisChao",              //TextureName
				7,                          //TextureCount
				0xFFE52932,                 //IconColor - hex, 4 bytes, 0xAARRGGBB
				ICON_TYPE_BALL,             //IconType
				NULL,                       //pIconData

				SolarisChaoEvo,          //pEvolveFunc

				0,                          //Flags
				"Solaris Chao",             //Name
				"SolarisChao",              //id
		};
		cwe_api->AddChaoType(&CharChao_SolarisData);

		CWE_API_CHAO_DATA CharChao_MephData =
		{
			MDLMephilesChao->getmodel(), //pObject
			{ 0 },                        //pSecondEvoList[5]

				"MephChao",              //TextureName
				7,                          //TextureCount
				0x66C300FF,                 //IconColor - hex, 4 bytes, 0xAARRGGBB
				ICON_TYPE_SPIKY,             //IconType
				NULL,                       //pIconData

				MephilesChaoEvo,          //pEvolveFunc

				0,                          //Flags
				"Mephiles Chao",             //Name
				"mephmephChao",              //id
		};
		cwe_api->AddChaoType(&CharChao_MephData);

		CWE_API_CHAO_DATA CharChao_IblisData =
		{
			MDLIblisChao->getmodel(), //pObject
			{ 0 },                        //pSecondEvoList[5]

				"IblisChao",              //TextureName
				7,                          //TextureCount
				0xFF00FF09,                 //IconColor - hex, 4 bytes, 0xAARRGGBB
				ICON_TYPE_BALL,             //IconType
				NULL,                       //pIconData

				IblisChaoEvo,          //pEvolveFunc

				0,                          //Flags
				"Iblis Chao",             //Name
				"IblisChao",              //id
		};
		cwe_api->AddChaoType(&CharChao_IblisData);
	}


	__declspec(dllexport) void Init(const char* path)
	{
		srand(time(0));
		HMODULE h = GetModuleHandle(L"CWE");

		std::string pathStr = std::string(path) + "\\";
		//Lens function - This talks to CWE to get the lens to work when registering it as a special object.
		ALS_LensSpecial = (decltype(ALS_LensSpecial))GetProcAddress(GetModuleHandle(L"CWE"), "ALS_LensSpecial");
		//Tell what models need to be associated to their variables here.
		MDLSolarisWill = new ModelInfo(pathStr + "willofsolaris.sa2mdl");
		MDLforeboding = new ModelInfo(pathStr + "forebodingshadow.sa2mdl");
		MDLIblisEye = new ModelInfo(pathStr + "IblisEyes.sa2mdl");

		IblisCoreMDL = new ModelInfo(pathStr + "IblisCore.sa2mdl");
		ScepterMDL = new ModelInfo(pathStr + "Scepter.sa2mdl");
		MDLSolGuard = new ModelInfo(pathStr + "solguard.sa2mdl");
		MDLsonicmanhat = new ModelInfo(pathStr + "sonicmanhat.sa2mdl");
		MDLgunhat = new ModelInfo(pathStr + "gunhelmet.sa2mdl");
		MDLSolarisChao = new ModelInfo(pathStr + "SolarisChao.sa2mdl");
		MDLMephilesChao = new ModelInfo(pathStr + "MephChao.sa2mdl");
		MDLIblisChao = new ModelInfo(pathStr + "IblisChao.sa2mdl");
		RegisterDataFunc = (void (*)(void* ptr))GetProcAddress(h, "RegisterDataFunc");
		RegisterDataFunc(CWELoad);
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
