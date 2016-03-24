//// Includes
#include "stdafx.h"
#include "ButtonMonitor.h"
#include "Globals.h"
#include "Hooks.h"
#include "MenuDisplay.h"
#include "Natives.h"
#include "Tools.h"
#include <stdio.h>
#include <algorithm>
using namespace std;

//// Global Variables
/// GTA Vars 
Entity _Entity;
Ped _Ped;
Player _Player;
Position _Position;
Vehicle _Vehicle;
bool _VehicleExists = false;
/// Menu Display
MenuDisplay mdCurrentMenu("");
MenuDisplay mdMainMenu("Main Menu");
MenuDisplay mdMainMods("Main Mods");
MenuDisplay mdPlayerMods("Player Mods");
MenuDisplay mdPlayerMods_OnlinePlayers("Player Mods > Online Players");
MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer("Player Mods > Online Players > %s");
MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions("Player Mods > Online Players > %s > Cash Drop Options");
MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions("Player Mods > Online Players > %s > Explosion Options");
MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions("Player Mods > Online Players > %s > Weapon Options");
MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions("Player Mods > Online Players > %s > Vehicle Options");
MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions("Player Mods > Online Players > %s > Teleport Options");
MenuDisplay mdWeaponMods("Weapon Mods");
MenuDisplay mdWeaponMods_CannonOptions("Weapon Mods > Cannon Options");
MenuDisplay mdVehicleMods("Vehicle Mods");
MenuDisplay mdVehicleMods_VehicleEditor("Vehicle Mods > Vehicle Editor");
MenuDisplay mdVehicleMods_VehicleSpawner("Vehicle Mods > Vehicle Spawner");
MenuDisplay mdVehicleMods_VehicleSpawner_Boat("Vehicle Mods > Vehicle Spawner > Boat");
MenuDisplay mdVehicleMods_VehicleSpawner_Commercial("Vehicle Mods > Vehicle Spawner > Commercial");
MenuDisplay mdVehicleMods_VehicleSpawner_Compact("Vehicle Mods > Vehicle Spawner > Compact");
MenuDisplay mdVehicleMods_VehicleSpawner_Coupe("Vehicle Mods > Vehicle Spawner > Coupe");
MenuDisplay mdVehicleMods_VehicleSpawner_Cycle("Vehicle Mods > Vehicle Spawner > Cycle");
MenuDisplay mdVehicleMods_VehicleSpawner_Emergency("Vehicle Mods > Vehicle Spawner > Emergency");
MenuDisplay mdVehicleMods_VehicleSpawner_Helicopter("Vehicle Mods > Vehicle Spawner > Helicopter");
MenuDisplay mdVehicleMods_VehicleSpawner_Industrial("Vehicle Mods > Vehicle Spawner > Industrial");
MenuDisplay mdVehicleMods_VehicleSpawner_Military("Vehicle Mods > Vehicle Spawner > Military");
MenuDisplay mdVehicleMods_VehicleSpawner_Motorcycle("Vehicle Mods > Vehicle Spawner > Motorcycle");
MenuDisplay mdVehicleMods_VehicleSpawner_Muscle("Vehicle Mods > Vehicle Spawner > Muscle");
MenuDisplay mdVehicleMods_VehicleSpawner_OffRoad("Vehicle Mods > Vehicle Spawner > Off-Road");
MenuDisplay mdVehicleMods_VehicleSpawner_Plane("Vehicle Mods > Vehicle Spawner > Plane");
MenuDisplay mdVehicleMods_VehicleSpawner_Sedan("Vehicle Mods > Vehicle Spawner > Sedan");
MenuDisplay mdVehicleMods_VehicleSpawner_Service("Vehicle Mods > Vehicle Spawner > Service");
MenuDisplay mdVehicleMods_VehicleSpawner_Sport("Vehicle Mods > Vehicle Spawner > Sport");
MenuDisplay mdVehicleMods_VehicleSpawner_SportClassic("Vehicle Mods > Vehicle Spawner > Sport Classic");
MenuDisplay mdVehicleMods_VehicleSpawner_Super("Vehicle Mods > Vehicle Spawner > Super");
MenuDisplay mdVehicleMods_VehicleSpawner_SUV("Vehicle Mods > Vehicle Spawner > SUV");
MenuDisplay mdVehicleMods_VehicleSpawner_Utility("Vehicle Mods > Vehicle Spawner > Utility");
MenuDisplay mdVehicleMods_VehicleSpawner_Van("Vehicle Mods > Vehicle Spawner > Van");
MenuDisplay mdWorldEditor("World Editor");
MenuDisplay mdWorldEditor_Forge("World Editor > Forge");
MenuDisplay mdWorldEditor_Forge_ObjectEditor("World Editor > Forge > Object Editor");
MenuDisplay mdWorldEditor_Weather("World Editor > Weather");
MenuDisplay mdTeleportMenu("Teleport Menu");
/// Menu Vars
bool Menu_CanOpen = true;
DWORD Menu_Index = -1;
bool Menu_Open = false;
DWORD Menu_Scroll = 0;
DWORD Menu_SelectedPlayer = 0;
bool Toggle_ESP = false;
bool Toggle_ExplosiveAmmo = false;
bool Toggle_FireAmmo = false;
bool Toggle_FireworkAmmo = false;
bool Toggle_FlyingBikes = false;
bool Toggle_FPSCounter = false;
bool Toggle_HandbrakeDrift = false;
bool Toggle_Hydraulics = false;
bool Toggle_LaserSight = false;
bool Toggle_Moses = false;
bool Toggle_NoWanted = false;
bool Toggle_Noclip = false;
bool Toggle_NOS = false;
bool Toggle_ParachuteEject = false;
bool Toggle_RainbowPaint = false;
bool Toggle_RainbowSmoke = false;
bool Toggle_SharkOMatic = false;
bool Toggle_Speedometer = false;
bool Toggle_SuperJump = false;
bool Toggle_SuperSpeed = false;
bool Toggle_SupermanPunch = false;
bool Toggle_TeleportGun = false;
bool Toggle_UnlimitedAmmo = false;
bool Toggle_VehicleCollision = false;
bool Toggle_VehicleInvincibility = false;
bool Toggle_Whiplash = false;
/// Native Hook
DWORD NativeHitCount = 0;
Node* NativePool;
Node* NativePoolHead;
/// Scripting
// Aimbot
DWORD Aimbot_Client = 0;
bool Aimbot_Arrest = false;
Ped Aimbot_ArrestHandle = 0;
bool Aimbot_ArrestHit = false;
bool Aimbot_HasAimed = false;
bool Aimbot_HasAimedAtPlayer = false;
bool Aimbot_Toggle = false;
// Cash Lobby
DWORD CashLobby_Amount;
bool CashLobby_Toggle[16];
// First Person
Camera FirstPerson_Camera;
bool FirstPerson_Check = false;
bool FirstPerson_Load = false;
bool FirstPerson_Toggle = false;
// Forge
Prop Forge_Object = 0;
DWORD Forge_ObjectHash = 0;
Position Forge_ObjectPosition;
Position Forge_ObjectRotation;
bool Forge_Toggle = false;
// Grappling Hook
bool GrapplingHook_MovingToTarget = false;
Rope GrapplingHook_Rope = 0;
bool GrapplingHook_Toggle = false;
// Ride Animal
bool RideAnimal_Exists = false;
DWORD RideAnimal_Hash = 0;
Ped RideAnimal_Handle = 0;
Position RideAnimal_Position;
bool RideAnimal_Riding = false;
bool RideAnimal_Toggle = false;
// Weapon Cannon
bool WeaponCannon_Toggle = false;
WeaponCannon_Projectiles WeaponCannon_Type = WeaponCannon_Projectiles::Animal;
// Zombies
bool Zombies_Toggle = false;
/// Other
bool bInitialized = false;
bool bListInitialized = false;
bool bPlayingGTAV = false;
ButtonMonitor* Controller = new ButtonMonitor;
/// Virtual Machine
DWORD Check_Airstrike = -1;
DWORD Check_SetModel = 0;
bool Check_SpawnVeh_Fulfilled = false;
Vehicle Check_SpawnVeh_Handle = 0;
DWORD Check_SpawnVeh_Hash = 0;
Position Check_SpawnVeh_Position;
char* Request_AnimDict = "";
char* Request_ClipSet = "";
DWORD Request_Hash = 0;
/// XUID Spoof
char* XUIDSpoof_Gamertag = "";
bool XUIDSpoof_Spoofing = false;
XUID XUIDSpoof_XUID = 0;

//// Methods
/// ASM Patches
#pragma optimize("", off)
void __declspec(naked) TheSaiyanProject_DevMode()
{
	__asm
	{
		li r3, 1
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
void __declspec(naked) TheSaiyanProject_ScriptRPFBypass()
{
	// r30 = file size (0x022CC800)
	// r3 = script.rpf CRC (0xF6D6AA59)
	__asm
	{
		// Spoof the file size
		lis r30, 0x022C
		ori r30, r30, 0xC800
		stw r30, 0x18(r29)

		// Spoof the CRC checksum
		lis r3, 0xF6D6
		ori r3, r3, 0xAA59

		// NOP the call to the function that checks the script
		nop
	}
}
#pragma optimize("", on)
/// Script Threads
void TheSaiyanProject_Aimbot()
{
	while (true)
	{
		if (bPlayingGTAV)
		{
			if (Aimbot_Toggle)
			{
				if (Aimbot_Client != _Player)
				{
					Ped ped = GET_PLAYER_PED(Aimbot_Client);
					Position pos = GET_ENTITY_COORDS(ped);
					if (_Position.Distance(pos) < 100.1 && Controller->TRIGGER_LEFT && IS_PED_ON_FOOT(_Ped) && HAS_ENTITY_CLEAR_LOS_TO_ENTITY(_Ped, ped, 17) && !Aimbot_ArrestHit && !IS_ENTITY_DEAD(ped))
					{
						Aimbot_HasAimedAtPlayer = true;
						Aimbot_ArrestHandle = ped;
						Aimbot_Arrest = true;
						Aimbot_ArrestHit = true;
					}
				}
				if (Controller->TRIGGER_LEFT_JR)
				{
					if (Aimbot_HasAimedAtPlayer)
					{
						CLEAR_PED_TASKS(_Ped);
						Aimbot_HasAimed = true;
						Aimbot_HasAimedAtPlayer = false;
						Aimbot_ArrestHit = false;
					}
				}
				if (Aimbot_Arrest)
				{
					TASK_ARREST_PED(_Ped, Aimbot_ArrestHit);
					Aimbot_Arrest = false;
				}
				Aimbot_Client++;
				if (Aimbot_Client > 15)
					Aimbot_Client = 0;
			}
		}
	}
}
void TheSaiyanProject_AntiReport()
{
	bool cleared = false;
	while (true)
	{
		if (bPlayingGTAV)
		{
			Sleep(20000);

			DWORD stats_int[] = { 0xB7963518, 0x6814DD4E, 0x547DD233, 0xBB8166FD, 0xC8133386, 0xD7976693, 0xAC343C58, 0xE5C7FE4E, 0xF760A5C1, 0xFAD6EEE4, 0x24D5E103, 0x856D83E0, 0x7B77D2B7, 0xEE78099C, 0x73A08EE1, 0xCEF953BA, 0x72799E94, 0xCE0C3CF1, 0x8556B2A9, 0xA0326B6F, 0xC6F9DD8B, 0x1E400AED, 0x9E188177, 0xA98E936F, 0x5227604E, 0x057C9EBD };
			for (int i = 0; i < (sizeof(stats_int) / 4); i++)
			{
				if (STAT_GET_INT(stats_int[i]) != 0)
				{
					STAT_SET_INT(stats_int[i], 0);
					cleared = true;
				}
			}

			DWORD stats_bool[] = { 0x48FAC4AE, 0x9E15A6E2 };
			for (int i = 0; i < (sizeof(stats_bool) / 4); i++)
			{
				if (STAT_GET_BOOL(stats_bool[i]))
				{
					STAT_SET_BOOL(stats_bool[i], false);
					cleared = true;
				}
			}

			if (cleared)
				XNotify(L"The Saiyan Project - Cleared player reports!");
			cleared = false;

			Sleep(20000);
		}
	}
}
void TheSaiyanProject_CashLobby()
{
	while (true)
	{
		if (bPlayingGTAV)
		{
			for (int i = 0; i < 16; i++)
			{
				if (CashLobby_Toggle[i])
				{
					Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(i));
					if (pos.DistanceTo(_Position) < 300.0f)
						_DROP_CASH(pos, CashLobby_Amount);
					else
						CashLobby_Toggle[i] = false;
				}
			}
		}
	}
}
void TheSaiyanProject_Forge()
{
	while (true)
	{
		if (bPlayingGTAV)
		{
			if (Forge_Toggle)
			{
				Prop aimed = GET_AIMED_ENTITY(_Player);
				if (aimed != 0)
					Forge_Object = aimed;

				if (Forge_Object != 0)
				{
					Forge_ObjectHash = GET_ENTITY_MODEL(Forge_Object);
					Forge_ObjectPosition = GET_ENTITY_COORDS(Forge_Object);
					Forge_ObjectRotation = GET_ENTITY_ROTATION(Forge_Object);
				}
			}
			else
				Forge_Object = 0;
		}
	}
}
void TheSaiyanProject_ParachuteEject()
{
	while (true)
	{
		if (bPlayingGTAV)
		{
			if (Toggle_ParachuteEject)
			{
				if (Controller->MAIN_Y_JP && _Vehicle != 0 && !IS_PED_GETTING_INTO_A_VEHICLE(_Ped) && (GET_ENTITY_SPEED(_Vehicle) > 20.0f))
				{
					CLEAR_PED_TASKS_IMMEDIATELY(_Ped);
					for (int i = 0; i < 6; i++)
					if (GET_PED_IN_VEHICLE_SEAT(_Vehicle, i) == _Ped)
						SET_VEHICLE_DOOR_OPEN(_Vehicle, i, false, true);
					_TELEPORT(_Position.x, _Position.y, _Position.z + 30.0f);
					SET_ENTITY_PROOFS(_Ped, false, true, true, false, false, false, false, false);
					SET_PED_CAN_RAGDOLL(_Ped, true);
					_APPLY_RELATIVE_FORCE(_Ped, Position(0.0f, 0.0f, 16.0f));
					TASK_PARACHUTE(_Ped);
					SET_ENTITY_PROOFS(_Ped, false, false, false, false, false, false, false, false);
					TASK_PARACHUTE_TO_TARGET(_Ped, _Position.x, _Position.y, _Position.z);
					_APPLY_RELATIVE_FORCE(_Ped, Position(0.0f, 0.0f, 16.0f));
				}
			}
		}
	}
}
void TheSaiyanProject_RideAnimal()
{
	/*while (true)
	{
		if (bPlayingGTAV)
		{
			if (RideAnimal_Toggle)
			{
				Entity attachedTo = GET_ENTITY_ATTACHED_TO(_Ped);
				RideAnimal_Position = RideAnimal_Exists ? GET_ENTITY_COORDS(RideAnimal_Handle) : Position();
				RideAnimal_Riding = RideAnimal_Exists && attachedTo == RideAnimal_Handle;

				if (RideAnimal_Exists)
				{
					if (Controller->MAIN_Y_JP)
					{
						if (_Vehicle != 0 || IS_PLAYER_DEAD(_Player) || IS_PED_GETTING_INTO_A_VEHICLE(_Ped))
							break;

						if (attachedTo == RideAnimal_Handle)
						{
							Position pos = _GET_COORDS_INFRONT_OF_ENTITY_WITH_DISTANCE(RideAnimal_Handle, 1.0, 90.0);
							pos.z = GET_GROUND_Z_FOR_3D_COORD(pos.x, pos.y, pos.z);
							float heading = GET_ENTITY_HEADING(RideAnimal_Handle);
							DETACH_ENTITY(_Ped);
							CLEAR_PED_TASKS_IMMEDIATELY(_Ped);
							_TELEPORT(pos);
							SET_ENTITY_HEADING(_Ped, heading);
						}
						else
						{
							if (_Position.DistanceTo(RideAnimal_Position) < 3.0)
							{
								FREEZE_ENTITY_POSITION(_Ped, true);
								FREEZE_ENTITY_POSITION(RideAnimal_Handle, true);
								_TELEPORT(RideAnimal_Position);
								ATTACH_ENTITY_TO_ENTITY(_Ped, RideAnimal_Handle, GET_PED_BONE_INDEX(RideAnimal_Handle, Assets_Models_BoneIndex::SKEL_Spine1), -0.3, 0.0, 0.3, 180.0, 0.0, 90.0, false, false, false, false, 2, true);
								_PLAY_ANIMATION(_Ped, "rcmjosh2", "josh_sitting_loop", 8.0, -1, true, -1);
								FREEZE_ENTITY_POSITION(_Ped, false);
								FREEZE_ENTITY_POSITION(RideAnimal_Handle, false);
							}
						}
					}

					if ((_Vehicle == 0 || !IS_PED_GETTING_INTO_A_VEHICLE(_Ped)) && attachedTo == RideAnimal_Handle)
					{
						float xNormal, yNormal;

						float speed = 2.0;
						float range = 4.0;
						if (Controller->MAIN_A)
						{
							speed = 3.0;
							range = 8.0;
						}

						Position goToOffset = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(RideAnimal_Handle, xNormal * range, yNormal * -1.0 * range, 0.0);
						TASK_LOOK_AT_COORD(RideAnimal_Handle, goToOffset.x, goToOffset.y, goToOffset.z);
						TASK_GO_STRAIGHT_TO_COORD(RideAnimal_Handle, goToOffset.x, goToOffset.y, goToOffset.z, speed, 20000, 40000.0, 0.5);
					}
				}
			}
		}
	}*/
}
void TheSaiyanProject_WeaponCannon()
{
	#pragma region Animals
	DWORD animals[] = { Assets_Models::a_c_boar, Assets_Models::a_c_chickenhawk, Assets_Models::a_c_chimp, Assets_Models::a_c_chop,
						Assets_Models::a_c_cormorant, Assets_Models::a_c_cow, Assets_Models::a_c_coyote, Assets_Models::a_c_crow,
						Assets_Models::a_c_deer, Assets_Models::a_c_fish, Assets_Models::a_c_hen, Assets_Models::a_c_husky,
						Assets_Models::a_c_mtlion, Assets_Models::a_c_pig, Assets_Models::a_c_pigeon, Assets_Models::a_c_rat,
						Assets_Models::a_c_retriever, Assets_Models::a_c_rhesus, Assets_Models::a_c_rottweiler, Assets_Models::a_c_seagull };
	#pragma endregion
	#pragma region Objects
	DWORD objects[] = { 0 };
	#pragma endregion
	#pragma region Pedestrians
	DWORD pedestrians[] = { Assets_Models::a_f_m_beach_01, Assets_Models::a_f_m_beach_01_p, Assets_Models::a_f_m_bevhills_01, Assets_Models::a_f_m_bevhills_01_p,
							Assets_Models::a_f_m_bevhills_02, Assets_Models::a_f_m_bevhills_02_p, Assets_Models::a_f_m_bodybuild_01, Assets_Models::a_f_m_bodybuild_01_p,
							Assets_Models::a_f_m_business_02, Assets_Models::a_f_m_business_02_p, Assets_Models::a_f_m_downtown_01, Assets_Models::a_f_m_downtown_01_p,
							Assets_Models::a_f_m_eastsa_01, Assets_Models::a_f_m_eastsa_01_p, Assets_Models::a_f_m_eastsa_02, Assets_Models::a_f_m_eastsa_02_p,
							Assets_Models::a_f_m_fatbla_01, Assets_Models::a_f_m_fatbla_01_p, Assets_Models::a_f_m_fatcult_01, Assets_Models::a_f_m_fatcult_01_p };
	#pragma endregion
	#pragma region Vehicles
	DWORD vehicles[] = { Assets_Vehicles::carbonrs, Assets_Vehicles::hotknife, Assets_Vehicles::tiptruck, Assets_Vehicles::faggio2,
						 Assets_Vehicles::youga, Assets_Vehicles::glendale, Assets_Vehicles::dominator, Assets_Vehicles::kalahari,
						 Assets_Vehicles::coquette, Assets_Vehicles::btype, Assets_Vehicles::boxville3, Assets_Vehicles::baller2,
						 Assets_Vehicles::miljet, Assets_Vehicles::freightcar, Assets_Vehicles::speeder, Assets_Vehicles::freightcont2,
						 Assets_Vehicles::dinghy2, Assets_Vehicles::tropic, Assets_Vehicles::gburrito2, Assets_Vehicles::hexer,
						 Assets_Vehicles::crusader, Assets_Vehicles::cogcabrio, Assets_Vehicles::vacca, Assets_Vehicles::polmav,
						 Assets_Vehicles::proptrailer, Assets_Vehicles::cargoplane, Assets_Vehicles::surano, Assets_Vehicles::raketrailer,
						 Assets_Vehicles::squalo, Assets_Vehicles::turismor, Assets_Vehicles::kuruma2, Assets_Vehicles::infernus,
						 Assets_Vehicles::boxville4, Assets_Vehicles::handler, Assets_Vehicles::cruiser, Assets_Vehicles::policet,
						 Assets_Vehicles::tornado, Assets_Vehicles::lguard, Assets_Vehicles::mixer2, Assets_Vehicles::huntley,
						 Assets_Vehicles::fusilade, Assets_Vehicles::dinghy3, Assets_Vehicles::utillitruck, Assets_Vehicles::voodoo2,
						 Assets_Vehicles::boattrailer, Assets_Vehicles::dune2, Assets_Vehicles::khamelion, Assets_Vehicles::packer,
						 Assets_Vehicles::tankercar, Assets_Vehicles::luxor, Assets_Vehicles::romero, Assets_Vehicles::barracks3,
						 Assets_Vehicles::lectro, Assets_Vehicles::freightgrain, Assets_Vehicles::surfer, Assets_Vehicles::trailersmall,
						 Assets_Vehicles::speedo2, Assets_Vehicles::slamvan, Assets_Vehicles::sadler2, Assets_Vehicles::buffalo2,
						 Assets_Vehicles::pranger, Assets_Vehicles::sovereign, Assets_Vehicles::frogger, Assets_Vehicles::buzzard2,
						 Assets_Vehicles::ztype, Assets_Vehicles::alpha, Assets_Vehicles::submersible, Assets_Vehicles::rhino,
						 Assets_Vehicles::sanchez, Assets_Vehicles::buzzard, Assets_Vehicles::slamvan2, Assets_Vehicles::annihilator,
						 Assets_Vehicles::rhapsody, Assets_Vehicles::bjxl, Assets_Vehicles::biff, Assets_Vehicles::jetmax,
						 Assets_Vehicles::metrotrain, Assets_Vehicles::sentinel2, Assets_Vehicles::habanero, Assets_Vehicles::intruder,
						 Assets_Vehicles::utillitruck2, Assets_Vehicles::burrito4, Assets_Vehicles::mule, Assets_Vehicles::mesa,
						 Assets_Vehicles::freightcont1, Assets_Vehicles::casco, Assets_Vehicles::pony2, Assets_Vehicles::duster,
						 Assets_Vehicles::hydra, Assets_Vehicles::sultan, Assets_Vehicles::sandking2, Assets_Vehicles::coquette2,
						 Assets_Vehicles::graintrailer, Assets_Vehicles::freight, Assets_Vehicles::ninef, Assets_Vehicles::dinghy,
						 Assets_Vehicles::skylift, Assets_Vehicles::jb700, Assets_Vehicles::jet, Assets_Vehicles::bobcatxl,
						 Assets_Vehicles::barracks2, Assets_Vehicles::velum2, Assets_Vehicles::pigalle, Assets_Vehicles::superd,
						 Assets_Vehicles::bfinjection, Assets_Vehicles::fbi, Assets_Vehicles::tribike, Assets_Vehicles::bmx,
						 Assets_Vehicles::burrito5, Assets_Vehicles::caddy, Assets_Vehicles::rumpo, Assets_Vehicles::ambulance,
						 Assets_Vehicles::dubsta, Assets_Vehicles::seminole, Assets_Vehicles::hakuchou, Assets_Vehicles::landstalker,
						 Assets_Vehicles::airbus, Assets_Vehicles::serrano, Assets_Vehicles::vestra, Assets_Vehicles::oracle,
						 Assets_Vehicles::sentinel, Assets_Vehicles::flatbed, Assets_Vehicles::warrener, Assets_Vehicles::cargobob3,
						 Assets_Vehicles::tractor3, Assets_Vehicles::paradise, Assets_Vehicles::forklift, Assets_Vehicles::picador,
						 Assets_Vehicles::hauler, Assets_Vehicles::tornado2, Assets_Vehicles::stinger, Assets_Vehicles::airtug,
						 Assets_Vehicles::cargobob2, Assets_Vehicles::tractor, Assets_Vehicles::rancherxl, Assets_Vehicles::akuma,
						 Assets_Vehicles::dilettante2, Assets_Vehicles::stratum, Assets_Vehicles::rapidgt2, Assets_Vehicles::bison3,
						 Assets_Vehicles::stockade, Assets_Vehicles::enduro, Assets_Vehicles::tornado3, Assets_Vehicles::dloader,
						 Assets_Vehicles::washington, Assets_Vehicles::mower, Assets_Vehicles::tr3, Assets_Vehicles::besra,
						 Assets_Vehicles::peyote, Assets_Vehicles::thrust, Assets_Vehicles::camper, Assets_Vehicles::bulldozer,
						 Assets_Vehicles::fugitive, Assets_Vehicles::police3, Assets_Vehicles::trash, Assets_Vehicles::sheriff2,
						 Assets_Vehicles::rancherxl2, Assets_Vehicles::firetruk, Assets_Vehicles::tourbus, Assets_Vehicles::frogger2,
						 Assets_Vehicles::taco, Assets_Vehicles::tanker2, Assets_Vehicles::titan, Assets_Vehicles::daemon,
						 Assets_Vehicles::cavalcade, Assets_Vehicles::trailerlogs, Assets_Vehicles::futo, Assets_Vehicles::police,
						 Assets_Vehicles::benson, Assets_Vehicles::insurgent2, Assets_Vehicles::bison2, Assets_Vehicles::carbonizzare,
						 Assets_Vehicles::tr2, Assets_Vehicles::tr4, Assets_Vehicles::pounder, Assets_Vehicles::utillitruck3,
						 Assets_Vehicles::rocoto, Assets_Vehicles::bagger, Assets_Vehicles::docktrailer, Assets_Vehicles::phantom,
						 Assets_Vehicles::dump, Assets_Vehicles::blazer, Assets_Vehicles::manana, Assets_Vehicles::stunt,
						 Assets_Vehicles::guardian, Assets_Vehicles::stingergt, Assets_Vehicles::technical, Assets_Vehicles::phoenix,
						 Assets_Vehicles::tractor2, Assets_Vehicles::coach, Assets_Vehicles::mesa3, Assets_Vehicles::trailers3,
						 Assets_Vehicles::mule3, Assets_Vehicles::rebel2, Assets_Vehicles::tornado4, Assets_Vehicles::pbus,
						 Assets_Vehicles::feltzer2, Assets_Vehicles::boxville, Assets_Vehicles::police4, Assets_Vehicles::stretch,
						 Assets_Vehicles::rapidgt, Assets_Vehicles::asterope, Assets_Vehicles::surge, Assets_Vehicles::premier,
						 Assets_Vehicles::emperor2, Assets_Vehicles::insurgent, Assets_Vehicles::asea, Assets_Vehicles::asea2,
						 Assets_Vehicles::gauntlet, Assets_Vehicles::policeold2, Assets_Vehicles::rumpo2, Assets_Vehicles::granger,
						 Assets_Vehicles::tvtrailer, Assets_Vehicles::mammatus, Assets_Vehicles::gburrito, Assets_Vehicles::burrito3,
						 Assets_Vehicles::rubble, Assets_Vehicles::scrap, Assets_Vehicles::bullet, Assets_Vehicles::sabregt,
						 Assets_Vehicles::sheriff, Assets_Vehicles::velum, Assets_Vehicles::double_, Assets_Vehicles::dune,
						 Assets_Vehicles::maverick, Assets_Vehicles::radi, Assets_Vehicles::fbi2, Assets_Vehicles::armytrailer2,
						 Assets_Vehicles::police2, Assets_Vehicles::voltic, Assets_Vehicles::valkyrie, Assets_Vehicles::trailers2,
						 Assets_Vehicles::gresley, Assets_Vehicles::policeold1, Assets_Vehicles::stanier, Assets_Vehicles::armytrailer,
						 Assets_Vehicles::ninef2, Assets_Vehicles::sanchez2, Assets_Vehicles::prairie, Assets_Vehicles::bodhi2,
						 Assets_Vehicles::zentorno, Assets_Vehicles::kuruma, Assets_Vehicles::trflat, Assets_Vehicles::burrito,
						 Assets_Vehicles::towtruck, Assets_Vehicles::surfer2, Assets_Vehicles::cheetah, Assets_Vehicles::jester,
						 Assets_Vehicles::entityxf, Assets_Vehicles::ingot, Assets_Vehicles::lazer, Assets_Vehicles::blazer3,
						 Assets_Vehicles::trash2, Assets_Vehicles::schafter2, Assets_Vehicles::emperor3, Assets_Vehicles::dubsta3,
						 Assets_Vehicles::tribike2, Assets_Vehicles::adder, Assets_Vehicles::shamal, Assets_Vehicles::rebel,
						 Assets_Vehicles::armytanker, Assets_Vehicles::blade, Assets_Vehicles::riot, Assets_Vehicles::zion2,
						 Assets_Vehicles::sandking, Assets_Vehicles::issi2, Assets_Vehicles::primo, Assets_Vehicles::fq2,
						 Assets_Vehicles::dilettante, Assets_Vehicles::zion, Assets_Vehicles::jester2, Assets_Vehicles::rentalbus,
						 Assets_Vehicles::furoregt, Assets_Vehicles::mule2, Assets_Vehicles::comet2, Assets_Vehicles::marquis,
						 Assets_Vehicles::banshee, Assets_Vehicles::seashark, Assets_Vehicles::tailgater, Assets_Vehicles::cutter,
						 Assets_Vehicles::cablecar, Assets_Vehicles::taxi, Assets_Vehicles::tiptruck2, Assets_Vehicles::pcj,
						 Assets_Vehicles::burrito2, Assets_Vehicles::ruffian, Assets_Vehicles::bati2, Assets_Vehicles::docktug,
						 Assets_Vehicles::trailers, Assets_Vehicles::ripley, Assets_Vehicles::monster, Assets_Vehicles::fixter,
						 Assets_Vehicles::vigero, Assets_Vehicles::barracks, Assets_Vehicles::speedo, Assets_Vehicles::baller,
						 Assets_Vehicles::patriot, Assets_Vehicles::cavalcade2, Assets_Vehicles::mixer, Assets_Vehicles::freighttrailer,
						 Assets_Vehicles::mesa2, Assets_Vehicles::schwarzer, Assets_Vehicles::tanker, Assets_Vehicles::bus,
						 Assets_Vehicles::emperor, Assets_Vehicles::buccaneer, Assets_Vehicles::ratloader, Assets_Vehicles::cuban800,
						 Assets_Vehicles::nemesis, Assets_Vehicles::massacro2, Assets_Vehicles::jackal, Assets_Vehicles::seashark2,
						 Assets_Vehicles::sadler, Assets_Vehicles::f620, Assets_Vehicles::ratloader2, Assets_Vehicles::elegy2,
						 Assets_Vehicles::caddy2, Assets_Vehicles::oracle2, Assets_Vehicles::predator, Assets_Vehicles::towtruck2,
						 Assets_Vehicles::monroe, Assets_Vehicles::panto, Assets_Vehicles::tribike3, Assets_Vehicles::baletrailer,
						 Assets_Vehicles::dubsta2, Assets_Vehicles::felon, Assets_Vehicles::penumbra, Assets_Vehicles::bifta,
						 Assets_Vehicles::blista, Assets_Vehicles::swift, Assets_Vehicles::minivan, Assets_Vehicles::buffalo,
						 Assets_Vehicles::suntrap, Assets_Vehicles::boxville2, Assets_Vehicles::ruiner, Assets_Vehicles::stockade3,
						 Assets_Vehicles::scorcher, Assets_Vehicles::innovation, Assets_Vehicles::blimp, Assets_Vehicles::massacro,
						 Assets_Vehicles::vader, Assets_Vehicles::journey, Assets_Vehicles::pony, Assets_Vehicles::bati,
						 Assets_Vehicles::felon2, Assets_Vehicles::savage, Assets_Vehicles::cargobob, Assets_Vehicles::blazer2,
						 Assets_Vehicles::policeb, Assets_Vehicles::bison, Assets_Vehicles::regina, Assets_Vehicles::exemplar };
	#pragma endregion

	DWORD random;
	while (true)
	{
		if (bPlayingGTAV)
		{
			if (WeaponCannon_Toggle)
			{
				if (IS_PED_SHOOTING(_Ped))
				{
					srand(time(0));
					Position pos = _GET_CAMERA_DIRECTION(5.0);
					Position dir = _GET_CAMERA_DIRECTION();
					dir.x = 0.0;
					dir.y = 100.0 + abs(dir.y);
					dir.z *= 130;
					if (WeaponCannon_Type == WeaponCannon_Projectiles::Animal || WeaponCannon_Type == WeaponCannon_Projectiles::Pedestrian)
					{
						random = rand() % 19;

						Ped ped = CREATE_PED(26, WeaponCannon_Type == WeaponCannon_Projectiles::Animal ? animals[random] : pedestrians[random], pos.x, pos.y, pos.z, GET_ENTITY_HEADING(_Ped), true, true);
						APPLY_FORCE_TO_ENTITY(ped, true, dir.x, dir.y, dir.z, 0.0, 0.0, 0.0, false, true, true, true, false, true);
					}
					else if (WeaponCannon_Type == WeaponCannon_Projectiles::Vehicles)
					{
						random = rand() % 350;

						Check_SpawnVeh_Hash = vehicles[random];
						while (!Check_SpawnVeh_Fulfilled);
						APPLY_FORCE_TO_ENTITY(Check_SpawnVeh_Handle, true, dir.x, dir.y, dir.z, 0.0, 0.0, 0.0, false, true, true, true, false, true);
						Check_SpawnVeh_Fulfilled = false;
					}
					else if (WeaponCannon_Type == WeaponCannon_Projectiles::Object)
					{
						random = rand() % 19;

						Prop prop = CREATE_OBJECT(objects[random], pos.x, pos.y, pos.z, true, true, false);
						APPLY_FORCE_TO_ENTITY(prop, true, dir.x, dir.y, dir.z, 0.0, 0.0, 0.0, false, true, true, true, false, true);
					}
				}
			}
		}
	}
}
void TheSaiyanProject_Zombies()
{
	while (true)
	{
		if (bPlayingGTAV)
		{
			if (Zombies_Toggle)
			{
				if (!IS_PLAYER_DEAD(_Player))
				{
					std::vector<Ped> nearbypeds = _GET_NEARBY_PEDS(400.0f, 100);
					for (int i = 0; i < nearbypeds.size(); i++)
					{
						if (nearbypeds[i] != _Ped &&
							!IS_PED_DEAD(nearbypeds[i]) &&
							!IS_PED_A_PLAYER(nearbypeds[i]) &&
							IS_PED_HUMAN(nearbypeds[i]))
						{
							strcpy(Request_ClipSet, "move_m@drunk@verydrunk");
							while (strcmp(Request_ClipSet, "") != FALSE) {}
							SET_PED_MOVEMENT_CLIPSET(nearbypeds[i], "move_m@drunk@verydrunk", 0x3E800000);
							STOP_PED_SPEAKING(nearbypeds[i], true);
							DISABLE_PED_PAIN_AUDIO(nearbypeds[i], true);
							SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(nearbypeds[i], 1);
							SET_PED_FLEE_ATTRIBUTES(nearbypeds[i], 0, 0);
							SET_PED_COMBAT_ATTRIBUTES(nearbypeds[i], 46, 1);
							SET_MOVE_SPEED_MULTIPLIER(nearbypeds[i], 9999.6f);
							TASK_FOLLOW_NAV_MESH_TO_COORD(nearbypeds[i], _Position.x, _Position.y, _Position.z, 1.0f, -1, 0.0f, 0, 0.0f);
							SET_PED_KEEP_TASK(nearbypeds[i], true);
							SET_PED_AS_ENEMY(nearbypeds[i], true);
							SET_ENTITY_HEALTH(nearbypeds[i], 3000);
							APPLY_PED_DAMAGE_PACK(nearbypeds[i], "BigHitByVehicle", 0.0, 9.0);
							APPLY_PED_DAMAGE_PACK(nearbypeds[i], "SCR_Dumpster", 0.0, 9.0);
							APPLY_PED_DAMAGE_PACK(nearbypeds[i], "SCR_Torture", 0.0, 9.0);
							if (IS_PED_IN_ANY_VEHICLE(nearbypeds[i])) SET_VEHICLE_ENGINE_HEALTH(GET_VEHICLE_PED_IS_IN(nearbypeds[i]), 0);
						}
					}
				}
				else
				{
					Menu_CanOpen = true;
					Zombies_Toggle = false;
					SET_BLACKOUT(false);
					_PRINT_TEXT("Zombies ~r~Deactivated");
				}
			}
		}
	}
}
/// Other
void TheSaiyanProject_CreateMenu()
{
	mdMainMenu.AddOption("Main Mods");
	mdMainMenu.AddOption("Player Mods");
	mdMainMenu.AddOption("Weapon Mods");
	mdMainMenu.AddOption("Vehicle Mods");
	mdMainMenu.AddOption("World Editor");
	mdMainMenu.AddOption("Teleport Menu");

	#pragma region Main Mods
	mdMainMods.AddOption("Activate Zombies");
	mdMainMods.AddOption("Toggle ESP");
	mdMainMods.AddOption("Toggle First Person");
	mdMainMods.AddOption("Toggle FPS Counter");
	mdMainMods.AddOption("Toggle Grappling Hook Gun");
	mdMainMods.AddOption("Toggle No Wanted");
	mdMainMods.AddOption("Toggle Noclip");
	mdMainMods.AddOption("Toggle Shark-O-Matic");
	#pragma endregion

	#pragma region Player Mods
	mdPlayerMods.AddOption("Online Players");
	mdPlayerMods.AddOption("Model Editor");
	mdPlayerMods.AddOption("Toggle Invincibility");
	mdPlayerMods.AddOption("Toggle Invisibility");
	mdPlayerMods.AddOption("Toggle Moses Mod");
	mdPlayerMods.AddOption("Toggle Super Jump");
	mdPlayerMods.AddOption("Toggle Super Speed");
	mdPlayerMods.AddOption("Toggle Superman Punch");
	mdPlayerMods.AddOption("Toggle Whiplash");
	#pragma region Online Players
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("Online Player");
	mdPlayerMods_OnlinePlayers.AddOption("SCTV");
	mdPlayerMods_OnlinePlayers.AddOption("SCTV");
	#pragma region Online Player Options
	mdPlayerMods_OnlinePlayers_SelectedPlayer.AddOption("Cash Drop Options");
	mdPlayerMods_OnlinePlayers_SelectedPlayer.AddOption("Explosion Options");
	mdPlayerMods_OnlinePlayers_SelectedPlayer.AddOption("Weapon Options");
	mdPlayerMods_OnlinePlayers_SelectedPlayer.AddOption("Vehicle Options");
	mdPlayerMods_OnlinePlayers_SelectedPlayer.AddOption("Teleport Options");
	#pragma region Cash Drop Options
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $10,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $25,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $50,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $75,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $100,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $250,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $500,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Drop $750,000");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.AddOption("Enter amount of cash to drop");
	#pragma endregion
	#pragma region Explosion Options
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Airstrike");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Blimp Explosion");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Boat Explosion");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Car Explosion");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Fire Bomb");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Fire Extinguisher");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Flare");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Gas Explosion");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Grenade Explosion");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Grenade Explosion (smoke)");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.AddOption("Train Explosion");
	#pragma endregion
	#pragma region Weapon Options
	mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions.AddOption("Give All Weapons");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions.AddOption("Give Unlimited Ammo");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions.AddOption("Take All Weapons");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions.AddOption("Take Unlimited Ammo");
	#pragma endregion
	#pragma region Vehicle Options
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Destroy Vehicle");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Knock Off Bike");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Launch Vehicle");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Lock Doors");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Unlock Doors");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Toggle Remote Control");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.AddOption("Toggle Tow Truck");
	#pragma endregion
	#pragma region Teleport Options
	mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions.AddOption("<enter option here>");
	mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions.AddOption("<enter option here>");
	#pragma endregion
	#pragma endregion
	#pragma endregion
	#pragma endregion

	#pragma region Weapon Mods
	mdWeaponMods.AddOption("Cannon Options");
	mdWeaponMods.AddOption("Activate Ped War");
	mdWeaponMods.AddOption("Activate Poltergeist Gun");
	mdWeaponMods.AddOption("Give All Weapons");
	mdWeaponMods.AddOption("Take All Weapons");
	mdWeaponMods.AddOption("Toggle Aimbot");
	mdWeaponMods.AddOption("Toggle Explosive Ammo");
	mdWeaponMods.AddOption("Toggle Fire Ammo");
	mdWeaponMods.AddOption("Toggle Laser Sight");
	mdWeaponMods.AddOption("Toggle Teleport Gun");
	mdWeaponMods.AddOption("Toggle Unlimited Ammo");
	mdWeaponMods.AddOption("Toggle Weapon Cannon");
	#pragma region Cannon Options
	mdWeaponMods_CannonOptions.AddOption("Animal Cannon");
	mdWeaponMods_CannonOptions.AddOption("Object Cannon");
	mdWeaponMods_CannonOptions.AddOption("Pedestrian Cannon");
	mdWeaponMods_CannonOptions.AddOption("Vehicle Cannon");
	#pragma endregion
	#pragma endregion

	#pragma region Vehicle Mods
	mdVehicleMods.AddOption("Vehicle Editor");
	mdVehicleMods.AddOption("Vehicle Spawner");
	mdVehicleMods.AddOption("Fix and Wash Vehicle");
	mdVehicleMods.AddOption("Lock Vehicle");
	mdVehicleMods.AddOption("Unlock Vehicle");
	mdVehicleMods.AddOption("Toggle Collision");
	mdVehicleMods.AddOption("Toggle Flying Bikes");
	mdVehicleMods.AddOption("Toggle Handbrake Drift");
	mdVehicleMods.AddOption("Toggle Hydraulics");
	mdVehicleMods.AddOption("Toggle Invincibility");
	mdVehicleMods.AddOption("Toggle Invisibility");
	mdVehicleMods.AddOption("Toggle NOS");
	mdVehicleMods.AddOption("Toggle Parachute Eject");
	mdVehicleMods.AddOption("Toggle Rainbow Paint");
	mdVehicleMods.AddOption("Toggle Rainbow Tire Smoke");
	mdVehicleMods.AddOption("Toggle Speedometer");
	#pragma region Vehicle Editor
	mdVehicleMods_VehicleEditor.AddOption("Armor");
	mdVehicleMods_VehicleEditor.AddOption("Brakes");
	mdVehicleMods_VehicleEditor.AddOption("Bumpers");
	mdVehicleMods_VehicleEditor.AddOption("Engine");
	mdVehicleMods_VehicleEditor.AddOption("Exhaust");
	mdVehicleMods_VehicleEditor.AddOption("Explosives");
	mdVehicleMods_VehicleEditor.AddOption("Horn");
	mdVehicleMods_VehicleEditor.AddOption("Lights");
	mdVehicleMods_VehicleEditor.AddOption("Loss / Theft Protection");
	mdVehicleMods_VehicleEditor.AddOption("Plate");
	mdVehicleMods_VehicleEditor.AddOption("Respray");
	mdVehicleMods_VehicleEditor.AddOption("Skirts");
	mdVehicleMods_VehicleEditor.AddOption("Spoiler");
	mdVehicleMods_VehicleEditor.AddOption("Suspension");
	mdVehicleMods_VehicleEditor.AddOption("Transmission");
	mdVehicleMods_VehicleEditor.AddOption("Turbo");
	mdVehicleMods_VehicleEditor.AddOption("Wheels");
	mdVehicleMods_VehicleEditor.AddOption("Windows");
	#pragma endregion
	#pragma region Vehicle Spawner
	mdVehicleMods_VehicleSpawner.AddOption("Boat");
	mdVehicleMods_VehicleSpawner.AddOption("Commercial");
	mdVehicleMods_VehicleSpawner.AddOption("Compact");
	mdVehicleMods_VehicleSpawner.AddOption("Coupe");
	mdVehicleMods_VehicleSpawner.AddOption("Cycle");
	mdVehicleMods_VehicleSpawner.AddOption("Emergency");
	mdVehicleMods_VehicleSpawner.AddOption("Helicopter");
	mdVehicleMods_VehicleSpawner.AddOption("Industrial");
	mdVehicleMods_VehicleSpawner.AddOption("Military");
	mdVehicleMods_VehicleSpawner.AddOption("Motorcycle");
	mdVehicleMods_VehicleSpawner.AddOption("Muscle");
	mdVehicleMods_VehicleSpawner.AddOption("Off-Road");
	mdVehicleMods_VehicleSpawner.AddOption("Plane");
	mdVehicleMods_VehicleSpawner.AddOption("Sedan");
	mdVehicleMods_VehicleSpawner.AddOption("Service");
	mdVehicleMods_VehicleSpawner.AddOption("Sport");
	mdVehicleMods_VehicleSpawner.AddOption("Sport Classic");
	mdVehicleMods_VehicleSpawner.AddOption("Super");
	mdVehicleMods_VehicleSpawner.AddOption("SUV");
	mdVehicleMods_VehicleSpawner.AddOption("Utility");
	mdVehicleMods_VehicleSpawner.AddOption("Van");
	#pragma region Boat
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Dinghy");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Jetmax");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Kraken (EV / RP)");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Marquis");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Seashark");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Speeder (DLC)");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Squalo");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Submersible");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Suntrap");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Toro (DLC)");
	mdVehicleMods_VehicleSpawner_Boat.AddOption("Tropic");
	#pragma endregion
	#pragma region Commercial
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Benson");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Biff");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Hauler");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Mule");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Packer");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Phantom");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Pounder");
	mdVehicleMods_VehicleSpawner_Commercial.AddOption("Stockade");
	#pragma endregion
	#pragma region Compact
	mdVehicleMods_VehicleSpawner_Compact.AddOption("Blista");
	mdVehicleMods_VehicleSpawner_Compact.AddOption("Dilettante");
	mdVehicleMods_VehicleSpawner_Compact.AddOption("Issi (RET)");
	mdVehicleMods_VehicleSpawner_Compact.AddOption("Panto (DLC)");
	mdVehicleMods_VehicleSpawner_Compact.AddOption("Prairie");
	mdVehicleMods_VehicleSpawner_Compact.AddOption("Rhapsody (DLC)");
	#pragma endregion
	#pragma region Coupe
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Exemplar");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("F620");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Felon");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Felon GT (RET)");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Jackal");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Oracle");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Oracle XS");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Sentinel (RET)");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Sentinel XS");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Windsor (DLC)");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Zion");
	mdVehicleMods_VehicleSpawner_Coupe.AddOption("Zion Cabri (RET)");
	#pragma endregion
	#pragma region Cycle
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("BMX");
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("Cruiser");
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("Endurex Race Bike");
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("Fixter");
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("Scorcher");
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("Tri-Cycles Race Bike");
	mdVehicleMods_VehicleSpawner_Cycle.AddOption("Whippet Race Bike");
	#pragma endregion
	#pragma region Emergency
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Air Ambulance");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Ambulance");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("FIB Buffalo");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("FIB");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Fire Truck");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Interceptor");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Lifeguard (SP)");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Park Ranger");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Bike");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Buffalo");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Cruiser");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Maverick");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Rancher (SP)");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Riot");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Roadcruiser (SP)");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Transporter");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Police Prison Bus");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Predator");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Sheriff Cruiser");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Sheriff SUV");
	mdVehicleMods_VehicleSpawner_Emergency.AddOption("Unmarked Cruiser (SP)");
	#pragma endregion
	#pragma region Helicopter
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Annihilator (ON)");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Buzzard");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Buzzard Attack Chopper");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Cargobob");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Frogger");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Maverick");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Savage (DLC / ON)");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Skylift (SP)");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Swift (DLC)");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Swift Deluxe (DLC)");
	mdVehicleMods_VehicleSpawner_Helicopter.AddOption("Valkyrie (DLC / ON)");
	#pragma endregion
	#pragma region Industrial
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Cutter (SP)");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Dock Handler");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Dozer");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Dump");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Flatbed");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Guardian (DLC / ON)");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Mixer");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Rubble");
	mdVehicleMods_VehicleSpawner_Industrial.AddOption("Tipper");
	#pragma endregion
	#pragma region Military
	mdVehicleMods_VehicleSpawner_Military.AddOption("Barracks");
	mdVehicleMods_VehicleSpawner_Military.AddOption("Barracks Semi");
	mdVehicleMods_VehicleSpawner_Military.AddOption("Crusader");
	mdVehicleMods_VehicleSpawner_Military.AddOption("Rhino Tank");
	#pragma endregion
	#pragma region Motorcycle
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Akuma");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Bagger");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Bati 801");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Bati 801RR");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Carbon RS");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Daemon");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Double T");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Enduro (DLC)");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Faggio");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Hakuchou (DLC)");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Hexer");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Innovation (DLC)");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Lectro (DLC / ON)");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Nemesis");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("PCJ 600");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Ruffian");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Sanchez");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Sovereign (DLC)");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Thrust (DLC)");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Vader");
	mdVehicleMods_VehicleSpawner_Motorcycle.AddOption("Vindicator (DLC)");
	#pragma endregion
	#pragma region Muscle
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Blade (DLC)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Burger Shot Stallion (EV / RP)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Buccaneer");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Chino (DLC)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Coquette Blackfin (DLC)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Dominator");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Duke O'Death (EV / RP / SP)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Dukes (EV)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Gauntlet");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Hotknife (CE)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Phoenix");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Picador");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Pisswasser Dominator (EV / RP)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Rat-Loader");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Rat-Truck (DLC)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Redwood Gauntlet (EV / RP)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Ruiner");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Sabre Turbo");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Slamvan (DLC)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Stallion (EV)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Virgo (DLC)");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Vigero");
	mdVehicleMods_VehicleSpawner_Muscle.AddOption("Voodoo");
	#pragma endregion
	#pragma region OffRoad
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Bifta (DLC)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Blazer");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Blazer Lifeguard");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Bodhi");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Brawler (DLC)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Dubsta 6x6 (DLC)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Dune Buggy");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Duneloader");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Hot Rod Blazer");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Injection");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Insurgent (DLC / ON)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Insurgen Pick-Up (DLC / ON)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Kalahari (DLC)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Marshall (EV / RP)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Mesa (Merryweather)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Rancher XL");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Rebel");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Sandking SWB");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Sandking XL");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Space Docker (SP)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("Technical (DLC / ON)");
	mdVehicleMods_VehicleSpawner_OffRoad.AddOption("The Liberator (DLC)");
	#pragma endregion
	#pragma region Plane
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Atomic Blimp (CE / SP)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Besra (DLC)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Cargo Plane (SP)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Cuban 800");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Dodo (EV / RP)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Duster");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Hydra (DLC / ON)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Jet (SP)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Luxor");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Luxor Deluxe (DLC)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Mallard");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Mammatus");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Miljet (DLC)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("P-996 LAZER");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Shamal");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Titan");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Velum");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Vestra (DLC)");
	mdVehicleMods_VehicleSpawner_Plane.AddOption("Xero Blimp (EV / RP / SP)");
	#pragma endregion
	#pragma region Sedan
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Asea");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Asterope");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Emperor");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Fugitive");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Glendale (DLC)");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Ingot");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Intruder");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Premier");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Primo");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Regina");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Romero House");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Schafter");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Stanier");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Stratum");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Stretch");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Super Diamond");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Surge");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Tailgater");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Warrener (DLC)");
	mdVehicleMods_VehicleSpawner_Sedan.AddOption("Washington");
	#pragma endregion
	#pragma region Service
	mdVehicleMods_VehicleSpawner_Service.AddOption("Airport Bus");
	mdVehicleMods_VehicleSpawner_Service.AddOption("Bus");
	mdVehicleMods_VehicleSpawner_Service.AddOption("Dashound");
	mdVehicleMods_VehicleSpawner_Service.AddOption("Rental Shuttle Bus");
	mdVehicleMods_VehicleSpawner_Service.AddOption("Taxi");
	mdVehicleMods_VehicleSpawner_Service.AddOption("Tourbus");
	mdVehicleMods_VehicleSpawner_Service.AddOption("Trashmaster");
	#pragma endregion
	#pragma region Sport
	mdVehicleMods_VehicleSpawner_Sport.AddOption("9F");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("9F Cabrio (RET)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Alpha (DLC)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Banshee");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Blista Compact (EV)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Buffalo");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Buffalo S");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Carbonizzare (RET)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Comet");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Coquette");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Elegy RH8 (R*)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Feltzer");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Furore GT (DLC)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Fusilade");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Futo");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Go Go Monkey Blista (EV / RP)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Jester (DLC)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Jester (Racecar) (DLC)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Khamelion (ON)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Kuruma (DLC / ON)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Kuruma (Armored) (DLC / ON)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Massacro (DLC)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Massacro (Racecar) (DLC)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Penumbra");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Rapid GT");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Rapid GT (Convertible) (RET)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Schwartzer");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Sprunk Buffalo (EV / RP)");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Sultan");
	mdVehicleMods_VehicleSpawner_Sport.AddOption("Surano (RET)");
	#pragma endregion
	#pragma region Sport Classic
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Casco (DLC / ON)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Coquette Classic (DLC)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("JB 700");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Manana");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Monroe");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Peyote");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Pigalle (DLC)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Roosevelt (DLC)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Stinger");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Stinger GT (ON)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Stirling GT (DLC)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Zornado (RET)");
	mdVehicleMods_VehicleSpawner_SportClassic.AddOption("Z-Type");
	#pragma endregion
	#pragma region Super
	mdVehicleMods_VehicleSpawner_Super.AddOption("Adder");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Bullet");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Cheetah");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Entity XF");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Infernus");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Osiris (DLC)");
	mdVehicleMods_VehicleSpawner_Super.AddOption("T20 (DLC)");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Turismo R (DLC)");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Vacca");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Voltic");
	mdVehicleMods_VehicleSpawner_Super.AddOption("Zentorno (DLC)");
	#pragma endregion
	#pragma region SUV
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Baller");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("BeeJay XL");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Cavalcade");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Dubsta");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("FQ 2");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Granger");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Gresley");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Habanero");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Huntley S (DLC)");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Landstalker");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Mesa");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Patriot");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Radius");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Rocoto");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Seminole");
	mdVehicleMods_VehicleSpawner_SUV.AddOption("Serrano");
	#pragma endregion
	#pragma region Utility
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Airtug");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Caddy");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Docktug");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Fieldmaster");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Forklift");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Lawn Mower");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Ripley");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Sadler");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Scrap Truck");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Towtruck (SP)");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Towtruck (small) (SP)");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Tractor");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Utility Truck");
	mdVehicleMods_VehicleSpawner_Utility.AddOption("Utility Truck (small)");
	#pragma endregion
	#pragma region Van
	mdVehicleMods_VehicleSpawner_Van.AddOption("Bison");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Bobcat");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Boxville");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Burrito");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Camper");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Clown Van");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Gang Burrito");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Journey");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Minivan");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Paradise (DLC)");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Pony");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Rumpo");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Speedo");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Surfer");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Taco Van");
	mdVehicleMods_VehicleSpawner_Van.AddOption("Youga");
	#pragma endregion
	#pragma endregion
	#pragma endregion

	#pragma region World Editor
	mdWorldEditor.AddOption("Forge");
	mdWorldEditor.AddOption("Gravity");
	mdWorldEditor.AddOption("Timescale");
	mdWorldEditor.AddOption("Weather");
	#pragma region Forge
	mdWorldEditor_Forge.AddOption("Object Editor");
	mdWorldEditor_Forge.AddOption("Object Spawner");
	mdWorldEditor_Forge.AddOption("Toggle Forge Mode");
	#pragma region Object Editor
	mdWorldEditor_Forge_ObjectEditor.AddOption("Copy Object");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Delete Object");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Get Object Coordinates");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Get Object Hash");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Increase Heading");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Decrease Heading");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Increase Pitch");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Decrease Pitch");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Increase Roll");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Decrease Roll");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Move Up");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Move Down");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Move Left");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Move Right");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Move Forward");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Move Backward");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Snap to the Ground");
	mdWorldEditor_Forge_ObjectEditor.AddOption("Straighten Object");
	#pragma endregion
	#pragma endregion
	#pragma region Weather
	mdWorldEditor_Weather.AddOption("Blizzard");
	mdWorldEditor_Weather.AddOption("Clear");
	mdWorldEditor_Weather.AddOption("Clearing");
	mdWorldEditor_Weather.AddOption("Cloud");
	mdWorldEditor_Weather.AddOption("Extra Sunny");
	mdWorldEditor_Weather.AddOption("Foggy");
	mdWorldEditor_Weather.AddOption("Neutral");
	mdWorldEditor_Weather.AddOption("Overcast");
	mdWorldEditor_Weather.AddOption("Rain");
	mdWorldEditor_Weather.AddOption("Smog");
	mdWorldEditor_Weather.AddOption("Snow");
	mdWorldEditor_Weather.AddOption("Snow Light");
	mdWorldEditor_Weather.AddOption("Thunder");
	#pragma endregion
	#pragma endregion 

	#pragma region Teleport Menu
	mdTeleportMenu.AddOption("10ft Infront");
	mdTeleportMenu.AddOption("Airport (entrance)");
	mdTeleportMenu.AddOption("Airport (strip)");
	mdTeleportMenu.AddOption("Ammu-Nation (office)");
	mdTeleportMenu.AddOption("Ammu-Nation (range)");
	mdTeleportMenu.AddOption("Cargo Ship");
	mdTeleportMenu.AddOption("Casino");
	mdTeleportMenu.AddOption("Cluckin' Bell");
	mdTeleportMenu.AddOption("Epsilon Building");
	mdTeleportMenu.AddOption("FIB Building (lobby)");
	mdTeleportMenu.AddOption("FIB Building (roof)");
	mdTeleportMenu.AddOption("FIB Building (top floor)");
	mdTeleportMenu.AddOption("Fort Zancudo ATC (entrance)");
	mdTeleportMenu.AddOption("Fort Zancudo ATC (top floor)");
	mdTeleportMenu.AddOption("Heist Carrier");
	mdTeleportMenu.AddOption("Humane Labs Tunnel");
	mdTeleportMenu.AddOption("IAA Building (office)");
	mdTeleportMenu.AddOption("IAA Building (roof)");
	mdTeleportMenu.AddOption("Jewelry Store");
	mdTeleportMenu.AddOption("Lester's House");
	mdTeleportMenu.AddOption("LifeInvader Lobby");
	mdTeleportMenu.AddOption("Merryweather Dock");
	mdTeleportMenu.AddOption("NOOSE Headquarters");
	mdTeleportMenu.AddOption("North Yankton");
	mdTeleportMenu.AddOption("North Yankton Bank");
	mdTeleportMenu.AddOption("Playboy Mansion");
	mdTeleportMenu.AddOption("Slaughterhouse");
	mdTeleportMenu.AddOption("Strip Club (dj booth)");
	mdTeleportMenu.AddOption("Torture Room");
	mdTeleportMenu.AddOption("Underwater Hatch");
	mdTeleportMenu.AddOption("Underwater UFO");
	mdTeleportMenu.AddOption("Underwater WW2 Tank");
	mdTeleportMenu.AddOption("Vinewood Sign");
	mdTeleportMenu.AddOption("Waypoint");
	mdTeleportMenu.AddOption("Weed Farm");
	mdTeleportMenu.AddOption("Yacht");
	#pragma endregion
}
void TheSaiyanProject_CreateScripts()
{
	// Aimbot
	HANDLE hAimbot;
	DWORD dwAimbot;
	ExCreateThread(&hAimbot, 0, &dwAimbot, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_Aimbot, 0, 2);
	XSetThreadProcessor(hAimbot, 4);
	ResumeThread(hAimbot);
	CloseHandle(hAimbot);

	// Anti-Report
	HANDLE hAntiReport;
	DWORD dwAntiReport;
	ExCreateThread(&hAntiReport, 0, &dwAntiReport, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_AntiReport, 0, 2);
	XSetThreadProcessor(hAntiReport, 4);
	ResumeThread(hAntiReport);
	CloseHandle(hAntiReport);

	// Cash Lobby
	HANDLE hCashLobby;
	DWORD dwCashLobby;
	ExCreateThread(&hCashLobby, 0, &dwCashLobby, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_CashLobby, 0, 2);
	XSetThreadProcessor(hCashLobby, 4);
	ResumeThread(hCashLobby);
	CloseHandle(hCashLobby);

	// Forge
	HANDLE hForge;
	DWORD dwForge;
	ExCreateThread(&hForge, 0, &dwForge, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_Forge, 0, 2);
	XSetThreadProcessor(hForge, 4);
	ResumeThread(hForge);
	CloseHandle(hForge);

	// Parachute Eject
	HANDLE hParachuteEject;
	DWORD dwParachuteEject;
	ExCreateThread(&hParachuteEject, 0, &dwParachuteEject, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_ParachuteEject, 0, 2);
	XSetThreadProcessor(hParachuteEject, 4);
	ResumeThread(hParachuteEject);
	CloseHandle(hParachuteEject);

	// Ride Animal
	/*HANDLE hRideAnimal;
	DWORD dwRideAnimal;
	ExCreateThread(&hRideAnimal, 0, &dwRideAnimal, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_RideAnimal, 0, 2);
	XSetThreadProcessor(hRideAnimal, 4);
	ResumeThread(hRideAnimal);
	CloseHandle(hRideAnimal);*/

	// Weapon Cannon
	HANDLE hWeaponCannon;
	DWORD dwWeaponCannon;
	ExCreateThread(&hWeaponCannon, 0, &dwWeaponCannon, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_WeaponCannon, 0, 2);
	XSetThreadProcessor(hWeaponCannon, 4);
	ResumeThread(hWeaponCannon);
	CloseHandle(hWeaponCannon);

	// Zombies
	HANDLE hZombies;
	DWORD dwZombies;
	ExCreateThread(&hZombies, 0, &dwZombies, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_Zombies, 0, 2);
	XSetThreadProcessor(hZombies, 4);
	ResumeThread(hZombies);
	CloseHandle(hZombies);
}
void TheSaiyanProject_InputHandler()
{
	Sleep(10000);

	TheSaiyanProject_CreateScripts();
	while (bPlayingGTAV)
	{
		Controller->Refresh();
		if (Controller->BUMPER_LEFT_JP && Controller->BUMPER_RIGHT_JP && Menu_CanOpen)
		{
			Menu_Index = SubMenus::MainMenu;
			Menu_Open = !Menu_Open;
			Menu_Scroll = 0;

			_PRINT_TEXT("Menu %s", Menu_Open ? "~g~Opened" : "~r~Closed");
		}

		if (Menu_Open)
		{
			if (Controller->DPAD_DOWN_JP)
			{
				Menu_Scroll++;

				if (Menu_Scroll == mdCurrentMenu.dwOptions)
					Menu_Scroll = 0;
			}
			else if (Controller->DPAD_UP_JP)
			{
				Menu_Scroll--;

				if (Menu_Scroll == -1)
					Menu_Scroll = (mdCurrentMenu.dwOptions - 1);
			}
			else if (Controller->MAIN_A_JP)
			{
				if (Menu_Index == SubMenus::MainMenu)
				{
					if (Menu_Scroll == 0) // Main Mods
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::MainMods;
					}
					else if (Menu_Scroll == 1) // Player Mods
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::PlayerMods;
					}
					else if (Menu_Scroll == 2) // Weapon Mods
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::WeaponMods;
					}
					else if (Menu_Scroll == 3) // Vehicle Mods
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods;
					}
					else if (Menu_Scroll == 4) // World Editor
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::WorldEditor;
					}
					else if (Menu_Scroll == 5) // Teleport Menu
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::TeleportMenu;
					}
				}
				else if (Menu_Index == SubMenus::MainMods)
				{
					if (Menu_Scroll == 0) // Activate Zombies
					{
						Menu_CanOpen = false;
						Menu_Open = false;
						Zombies_Toggle = true;
						SET_BLACKOUT(true);
						SET_PLAYER_INVINCIBLE(_Player, false);
						_PRINT_TEXT("Zombies ~g~Activated~w~, you cannot use the menu until zombies are deactivated (when you die)");
					}
					else if (Menu_Scroll == 1) // Toggle ESP
					{
						Toggle_ESP = !Toggle_ESP;
						_PRINT_TEXT("ESP %s", Toggle_ESP ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 2) // Toggle First Person
					{
						// Menu click
						FirstPerson_Toggle = !FirstPerson_Toggle;
						if (FirstPerson_Toggle)
						{
							FirstPerson_Load = true;
							_PRINT_TEXT("First Person ~g~Enabled");
						}
						else
						{
							FirstPerson_Load = false;
							SET_CAM_ACTIVE(FirstPerson_Camera, false);
							DESTROY_CAM(FirstPerson_Camera, true);
							RENDER_SCRIPT_CAMS(false, false, 3000, true, false);
							_PRINT_TEXT("First Person ~r~Disabled");
						}
					}
					else if (Menu_Scroll == 3) // Toggle FPS Counter
					{
						Toggle_FPSCounter = !Toggle_FPSCounter;
						_PRINT_TEXT("FPS Counter %s", Toggle_FPSCounter ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 4) // Toggle Grappling Hook
					{
						/*Toggle_ExplosiveAmmo = false;
						Toggle_FireAmmo = false;
						Toggle_SharkOMatic = false;
						WeaponCannon_Toggle = false;

						GrapplingHook_Toggle = !GrapplingHook_Toggle;
						_PRINT_TEXT("Grappling Hook %s", GrapplingHook_Toggle ? "~g~Enabled" : "~r~Disabled");*/
						_PRINT_TEXT("~rDisabled");
					}
					else if (Menu_Scroll == 5) // Toggle No Wanted
					{
						Toggle_NoWanted = !Toggle_NoWanted;
						_PRINT_TEXT("No Wanted %s", Toggle_NoWanted ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 6) // Toggle Noclip
					{
						Toggle_Noclip = !Toggle_Noclip;
						_PRINT_TEXT("Noclip %s", Toggle_Noclip ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 7) // Toggle Shark-O-Matic
					{
						/*GrapplingHook_Toggle = false;
						Toggle_ExplosiveAmmo = false;
						Toggle_FireAmmo = false;
						WeaponCannon_Toggle = false;

						Toggle_SharkOMatic = !Toggle_SharkOMatic;
						_PRINT_TEXT("Shark-O-Matic %s", Toggle_SharkOMatic ? "~g~Enabled" : "~r~Disabled");*/
						_PRINT_TEXT("~r~Disabled");
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods)
				{
					if (Menu_Scroll == 0) // Online Players
					{
						Menu_Index = SubMenus::PlayerMods_OnlinePlayers;
					}
					else if (Menu_Scroll == 1) // Model Editor
					{
						char* modelName = XKeyboard(VKBD_LATIN_FULL, L"", L"Model Editor", L"Enter the name of the model you'd like your player model to change to.\nList of valid models are available on 7S and GTAF.");
						if (strcmp(modelName, "") != 0)
						{
							DWORD modelHash = GET_HASH_KEY(modelName);
							Check_SetModel = modelHash;
						}
					}
					else if (Menu_Scroll == 2) // Toggle Invincibility
					{
						bool invincibility = !GET_PLAYER_INVINCIBLE(_Player);
						SET_PLAYER_INVINCIBLE(_Player, invincibility);
						_PRINT_TEXT("Invincibility %s", invincibility ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 3) // Toggle Invisibility
					{
						bool invisibility = !IS_ENTITY_VISIBLE(_Ped);
						SET_ENTITY_VISIBLE(_Ped, invisibility);
						_PRINT_TEXT("Invisibility %s", invisibility ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 4) // Toggle Moses Mod
					{
						Toggle_Moses = !Toggle_Moses;
						_PRINT_TEXT("Moses Mod %s", Toggle_Moses ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 5) // Toggle Super Jump
					{
						Toggle_SuperJump = !Toggle_SuperJump;
						_PRINT_TEXT("Super Jump %s", Toggle_SuperJump ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 6) // Toggle Super Speed
					{
						Toggle_SuperSpeed = !Toggle_SuperSpeed;
						SET_SWIM_SPEED_MULTIPLIER(_Player, Toggle_SuperSpeed ? 1.49 : 1.00);
						SET_MOVE_SPEED_MULTIPLIER(_Player, Toggle_SuperSpeed ? 1.49 : 1.00);
						_PRINT_TEXT("Super Speed %s", Toggle_SuperSpeed ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 7) // Toggle Superman Punch
					{
						Toggle_SupermanPunch = !Toggle_SupermanPunch;
						_PRINT_TEXT("Superman Punch %s", Toggle_SupermanPunch ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 8) // Toggle Whiplash
					{
						Toggle_Whiplash = !Toggle_Whiplash;
						_PRINT_TEXT("Whiplash %s", Toggle_Whiplash ? "~g~Enabled" : "~r~Disabled");
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers)
				{
					if (Menu_Scroll < 16)
					{
						Menu_SelectedPlayer = Menu_Scroll;
						Menu_Scroll = 0;
						Menu_Index = SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer;
					}
					else if (Menu_Scroll == 16 || Menu_Scroll == 17)
					{
						_PRINT_TEXT("~b~%s ~w~is an ~r~SCTV admin~w~!", GET_PLAYER_NAME(Menu_Scroll));
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer)
				{
					if (Menu_Scroll == 0) // Cash Drop Options
					{
						Menu_Index = SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions;
					}
					else if (Menu_Scroll == 1) // Explosion Options
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions;
					}
					else if (Menu_Scroll == 2) // Weapon Options
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions;
					}
					else if (Menu_Scroll == 3) // Vehicle Options
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions;
					}
					else if (Menu_Scroll == 4) // Teleport Options
					{
						Menu_Scroll = 0;
						Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions;
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions)
				{
					if (Menu_Scroll == 0)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $10,000
						_DROP_CASH(pos, 10000);
						_PRINT_TEXT("Dropped ~g~$10,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 1)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $25,000
						_DROP_CASH(pos, 25000);
						_PRINT_TEXT("Dropped ~g~$25,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 2)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $50,000
						_DROP_CASH(pos, 50000);
						_PRINT_TEXT("Dropped ~g~$50,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 3)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $75,000
						_DROP_CASH(pos, 75000);
						_PRINT_TEXT("Dropped ~g~$75,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 4)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $100,000
						_DROP_CASH(pos, 100000);
						_PRINT_TEXT("Dropped ~g~$100,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 5)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $250,000
						_DROP_CASH(pos, 250000);
						_PRINT_TEXT("Dropped ~g~$250,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 6)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $500,000
						_DROP_CASH(pos, 500000);
						_PRINT_TEXT("Dropped ~g~$500,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 7)
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer)); // Drop $750,000
						_DROP_CASH(pos, 750000);
						_PRINT_TEXT("Dropped ~g~$750,000 ~w~on ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 8) // Enter amount of cash to drop
					{
						char* szCash = XKeyboard(VKBD_LATIN_NUMERIC, L"", L"Cash Dropper", L"Below you can enter the amount of cash you want to drop on the player of your choice.");
						if (strcmp(szCash, "") != 0)
						{
							int iCash = atoi(szCash);
							if (iCash > 0)
							{
								Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer));
								_DROP_CASH(pos, iCash);
								_PRINT_TEXT("Dropped ~g~%i ~w~on ~b~%s", iCash, GET_PLAYER_NAME(Menu_SelectedPlayer));
							}
						}
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions)
				{
					if (Menu_Scroll == 0) // Airstrike Player
					{
						Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Menu_SelectedPlayer));
						_CALL_AIRSTRIKE(pos.x, pos.y, pos.z);
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions)
				{
					if (Menu_Scroll == 0) // Give All Weapons
					{
						Ped ped = GET_PLAYER_PED(Menu_SelectedPlayer);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::advancedrifle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::appistol, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::assaultrifle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::assaultshotgun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::assaultsmg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::ball, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::bat, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::bottle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::briefcase, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::briefcase_02, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::bullpuprifle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::bullpupshotgun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::bzgas, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::carbinerifle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::combatmg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::combatpdw, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::combatpistol, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::crowbar, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::dagger, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::digiscanner, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::fireextinguisher, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::firework, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::flare, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::flaregun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::garbagebag, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::golfclub, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::grenade, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::grenadelauncher, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::grenadelauncher_smoke, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::gusenberg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::hammer, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::handcuffs, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::heavypistol, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::heavyshotgun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::heavysniper, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::hominglauncher, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::knife, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::marskmanrifle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::mg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::microsmg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::minigun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::molotov, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::musket, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::nightstick, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::petrolcan, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::pistol, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::pistol50, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::proxmine, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::pumpshotgun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::remotesniper, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::rpg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::sawnoffshotgun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::smg, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::smokegrenade, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::sniperrifle, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::snowball, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::snspistol, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::specialcarbine, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::stickybomb, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::stinger_, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::stungun, 9999);
						GIVE_WEAPON_TO_PED(ped, Assets_Weapons::vintagepistol, 9999);
						_PRINT_TEXT("All Weapons ~g~Given ~w~to ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 1) // Give Unlimited Ammo
					{
						SET_PED_INFINITE_AMMO_CLIP(GET_PLAYER_PED(Menu_SelectedPlayer), true);
						_PRINT_TEXT("Unlimited Ammo ~g~Given ~w~to ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 2) // Take All Weapons
					{
						REMOVE_ALL_PED_WEAPONS(GET_PLAYER_PED(Menu_SelectedPlayer));
						_PRINT_TEXT("All Weapons ~r~Taken ~w~from ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 3) // Take Unlimited Ammo
					{
						SET_PED_INFINITE_AMMO_CLIP(GET_PLAYER_PED(Menu_SelectedPlayer), true);
						_PRINT_TEXT("Unlimited Ammo ~r~Taken ~w~from ~b~%s", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions)
				{
					if (Menu_Scroll == 0) // Destroy Vehicle
					{
						Vehicle vehicle = GET_VEHICLE_PED_IS_IN(GET_PLAYER_PED(Menu_SelectedPlayer));
						if (vehicle != 0)
						{
							SET_VEHICLE_ENGINE_HEALTH(vehicle, 0.0f);
							_PRINT_TEXT("Destroyed ~b~%s~w~'s Vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
						}
						else
							_PRINT_TEXT("~b~%s ~w~is not in a vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 1) // Knock Off Bike
					{
						Ped ped = GET_PLAYER_PED(Menu_SelectedPlayer);
						Vehicle vehicle = GET_VEHICLE_PED_IS_IN(ped);
						if (vehicle != 0)
						{
							SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 0);
							KNOCK_PED_OFF_VEHICLE(ped);
							_PRINT_TEXT("Knocked ~b~%s ~w~off their bike", GET_PLAYER_NAME(Menu_SelectedPlayer));
						}
						else
							_PRINT_TEXT("~b~%s ~w~is not in a vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 2) // Launch Vehicle
					{
						Vehicle vehicle = GET_VEHICLE_PED_IS_IN(GET_PLAYER_PED(Menu_SelectedPlayer));
						if (vehicle != 0)
						{
							APPLY_FORCE_TO_ENTITY(vehicle, 0, 0.0f, 0.0f, 200.0f, 0.0f, 0.0f, -200.0f, false, true, true, false, false, true);
							_PRINT_TEXT("Launched ~b~%s~w~'s Vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
						}
						else
							_PRINT_TEXT("~b~%s ~w~is not in a vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 3) // Lock Doors
					{
						Vehicle vehicle = GET_VEHICLE_PED_IS_IN(GET_PLAYER_PED(Menu_SelectedPlayer));
						if (vehicle != 0)
						{
							SET_VEHICLE_DOORS_LOCKED(vehicle, true);
							_PRINT_TEXT("Locked ~b~%s~w~'s Vehicle Doors", GET_PLAYER_NAME(Menu_SelectedPlayer));
						}
						else
							_PRINT_TEXT("~b~%s ~w~is not in a vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
					else if (Menu_Scroll == 4) // Unlock Doors
					{
						Vehicle vehicle = GET_VEHICLE_PED_IS_IN(GET_PLAYER_PED(Menu_SelectedPlayer));
						if (vehicle != 0)
						{
							SET_VEHICLE_DOORS_LOCKED(vehicle, false);
							_PRINT_TEXT("Unlocked ~b~%s~w~'s Vehicle Doors", GET_PLAYER_NAME(Menu_SelectedPlayer));
						}
						else
							_PRINT_TEXT("~b~%s ~w~is not in a vehicle", GET_PLAYER_NAME(Menu_SelectedPlayer));
					}
				}
				else if (Menu_Index == SubMenus::WeaponMods)
				{
					if (Menu_Scroll == 0) // Cannon Options
					{
						Menu_Index = SubMenus::WeaponMods_CannonOptions;
					}
					else if (Menu_Scroll == 1) // Activate Ped War
					{
						std::vector<Ped> nearbypeds = _GET_NEARBY_PEDS(400.0f, 100);
						int size = nearbypeds.size();
						for (int i = 0; i < size; i++)
						{
							if (nearbypeds[i] != _Ped &&
								!IS_PED_DEAD(nearbypeds[i]) &&
								!IS_PED_A_PLAYER(nearbypeds[i]) &&
								IS_PED_HUMAN(nearbypeds[i]))
							{
								srand(i);
								REMOVE_ALL_PED_WEAPONS(nearbypeds[i]);
								GIVE_DELAYED_WEAPON_TO_PED(nearbypeds[i], ((rand() % 10) == 0 ? Assets_Weapons::minigun : Assets_Weapons::advancedrifle), 9999, 9999);
							}
						}
						nearbypeds.clear();
						_PRINT_TEXT("Armed %i Peds", size);
					}
					else if (Menu_Scroll == 2) // Activate Poltergeist Gun
					{
						//Toggle_PoltergeistGun = !Toggle_PoltergeistGun;
						//_PRINT_TEXT("Poltergeist Gun %s", Toggle_PoltergeistGun ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 3) // Give All Weapons
					{
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::advancedrifle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::appistol, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::assaultrifle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::assaultshotgun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::assaultsmg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::ball, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::bat, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::bottle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::briefcase, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::briefcase_02, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::bullpuprifle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::bullpupshotgun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::bzgas, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::carbinerifle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::combatmg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::combatpdw, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::combatpistol, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::crowbar, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::dagger, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::digiscanner, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::fireextinguisher, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::firework, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::flare, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::flaregun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::garbagebag, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::golfclub, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::grenade, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::grenadelauncher, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::grenadelauncher_smoke, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::gusenberg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::hammer, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::handcuffs, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::heavypistol, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::heavyshotgun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::heavysniper, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::hominglauncher, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::knife, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::marskmanrifle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::mg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::microsmg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::minigun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::molotov, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::musket, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::nightstick, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::petrolcan, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::pistol, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::pistol50, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::proxmine, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::pumpshotgun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::remotesniper, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::rpg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::sawnoffshotgun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::smg, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::smokegrenade, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::sniperrifle, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::snowball, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::snspistol, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::specialcarbine, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::stickybomb, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::stinger_, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::stungun, 9999);
						GIVE_WEAPON_TO_PED(_Ped, Assets_Weapons::vintagepistol, 9999);
						_PRINT_TEXT("All Weapons ~g~Given");
					}
					else if (Menu_Scroll == 4) // Take All Weapons
					{
						REMOVE_ALL_PED_WEAPONS(_Ped);
						_PRINT_TEXT("All Weapons ~r~Taken");
					}
					else if (Menu_Scroll == 5) // Toggle Aimbot
					{
						Aimbot_Client = 0;
						Aimbot_Toggle = !Aimbot_Toggle;
						_PRINT_TEXT("Aimbot %s", Aimbot_Toggle ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 6) // Toggle Explosive Ammo
					{
						GrapplingHook_Toggle = false;
						Toggle_FireAmmo = false;
						Toggle_FireworkAmmo = false;
						Toggle_SharkOMatic = false;
						Toggle_TeleportGun = false;
						WeaponCannon_Toggle = false;

						Toggle_ExplosiveAmmo = !Toggle_ExplosiveAmmo;
						_PRINT_TEXT("Explosive Ammo %s", Toggle_ExplosiveAmmo ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 7) // Toggle Fire Ammo
					{
						GrapplingHook_Toggle = false;
						Toggle_ExplosiveAmmo = false;
						Toggle_FireworkAmmo = false;
						Toggle_SharkOMatic = false;
						Toggle_TeleportGun = false;
						WeaponCannon_Toggle = false;

						Toggle_FireAmmo = !Toggle_FireAmmo;
						_PRINT_TEXT("Fire Ammo %s", Toggle_FireAmmo ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 8) // Toggle Firework Ammo
					{
						GrapplingHook_Toggle = false;
						Toggle_ExplosiveAmmo = false;
						Toggle_FireAmmo = false;
						Toggle_SharkOMatic = false;
						Toggle_TeleportGun = false;
						WeaponCannon_Toggle = false;

						Toggle_FireworkAmmo = !Toggle_FireworkAmmo;
						_PRINT_TEXT("Firework Ammo %s", Toggle_FireworkAmmo ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 9) // Toggle Laser Sight
					{
						Toggle_LaserSight = !Toggle_LaserSight;
						_PRINT_TEXT("Laser Sight %s", Toggle_LaserSight ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 10) // Toggle Teleport Gun
					{
						GrapplingHook_Toggle = false;
						Toggle_ExplosiveAmmo = false;
						Toggle_FireAmmo = false;
						Toggle_FireworkAmmo = false;
						Toggle_SharkOMatic = false;
						WeaponCannon_Toggle = false;

						Toggle_TeleportGun = !Toggle_TeleportGun;
						_PRINT_TEXT("Teleport Gun %s", Toggle_TeleportGun ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 11) // Toggle Unlimited Ammo
					{
						Toggle_UnlimitedAmmo = !Toggle_UnlimitedAmmo;
						SET_PED_INFINITE_AMMO_CLIP(_Ped, Toggle_UnlimitedAmmo);
						_PRINT_TEXT("Unlimited Ammo %s", Toggle_UnlimitedAmmo ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 12) // Toggle Weapon Cannon
					{
						GrapplingHook_Toggle = false;
						Toggle_ExplosiveAmmo = false;
						Toggle_FireAmmo = false;
						Toggle_FireworkAmmo = false;
						Toggle_SharkOMatic = false;
						Toggle_TeleportGun = false;

						WeaponCannon_Toggle = !WeaponCannon_Toggle;
						_PRINT_TEXT("Weapon Cannon %s", WeaponCannon_Toggle ? "~g~Enabled" : "~r~Disabled");
					}
				}
				else if (Menu_Index == SubMenus::WeaponMods_CannonOptions)
				{
					if (Menu_Scroll == 0) // Animal Cannon
					{
						WeaponCannon_Type = WeaponCannon_Projectiles::Animal;
						_PRINT_TEXT("Weapon Cannon set to ~g~Animal");
					}
					else if (Menu_Scroll == 1) // Object Cannon
					{
						WeaponCannon_Type = WeaponCannon_Projectiles::Object;
						_PRINT_TEXT("Weapon Cannon set to ~g~Object");
					}
					else if (Menu_Scroll == 2) // Pedestrian Cannon
					{
						WeaponCannon_Type = WeaponCannon_Projectiles::Pedestrian;
						_PRINT_TEXT("Weapon Cannon set to ~g~Pedestrian");
					}
					else if (Menu_Scroll == 3) // Vehicle Cannon
					{
						WeaponCannon_Type = WeaponCannon_Projectiles::Vehicles;
						_PRINT_TEXT("Weapon Cannon set to ~g~Vehicle");
					}
				}
				else if (Menu_Index == SubMenus::VehicleMods)
				{
					if (Menu_Scroll == 0) // Vehicle Editor
					{
						Menu_Index = SubMenus::VehicleMods_VehicleEditor;
					}
					else if (Menu_Scroll == 1) // Vehicle Spawner
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner;
					}
					else if (Menu_Scroll == 2) // Fix and Wash Vehicle
					{
						if (_VehicleExists)
						{
							SET_VEHICLE_FIXED(_Vehicle);
							SET_VEHICLE_DEFORMATION_FIXED(_Vehicle);
							SET_VEHICLE_DIRT_LEVEL(_Vehicle, 0.0);
							_PRINT_TEXT("Vehicle has been ~g~fixed ~w~and ~g~washed");
						}
						else
							_PRINT_TEXT("~r~You must be in a vehicle to use this option");
					}
					else if (Menu_Scroll == 3) // Lock Vehicle
					{
						if (_VehicleExists)
						{
							SET_VEHICLE_DOORS_LOCKED(_Vehicle, true);
							_PRINT_TEXT("Vehicle ~r~Locked");
						}
						else
							_PRINT_TEXT("~r~You must be in a vehicle to use this option");
					}
					else if (Menu_Scroll == 4) // Unlock Vehicle
					{
						if (_VehicleExists)
						{
							SET_VEHICLE_DOORS_LOCKED(_Vehicle, false);
							_PRINT_TEXT("Vehicle ~g~Unlocked");
						}
						else
							_PRINT_TEXT("~r~You must be in a vehicle to use this option");
					}
					else if (Menu_Scroll == 5) // Toggle Collision
					{
						if (_VehicleExists)
						{
							Toggle_VehicleCollision = !Toggle_VehicleCollision;
							SET_ENTITY_COLLISION(_Vehicle, Toggle_VehicleCollision);
							_PRINT_TEXT("Vehicle Collision %s", Toggle_VehicleCollision ? "~g~Enabled" : "~r~Disabled");
						}
						else
							_PRINT_TEXT("~r~You must be in a vehicle to use this option");
					}
					else if (Menu_Scroll == 6) // Toggle Flying Bikes
					{
						Toggle_FlyingBikes = !Toggle_FlyingBikes;
						_PRINT_TEXT("Flying Bikes %s", Toggle_FlyingBikes ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 7) // Toggle Handbrake Drift
					{
						Toggle_HandbrakeDrift = !Toggle_HandbrakeDrift;
						_PRINT_TEXT("Handbrake Drift %s", Toggle_HandbrakeDrift ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 8) // Toggle Hydraulics
					{
						Toggle_Hydraulics = !Toggle_Hydraulics;
						_PRINT_TEXT("Hydraulics %s", Toggle_Hydraulics ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 9) // Toggle Invincibility
					{
						if (_VehicleExists)
						{
							Toggle_VehicleInvincibility = !Toggle_VehicleInvincibility;
							SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(_Vehicle, !Toggle_VehicleInvincibility);
							SET_VEHICLE_TYRES_CAN_BURST(_Vehicle, !Toggle_VehicleInvincibility);
							_PRINT_TEXT("Vehicle Invincibility %s", Toggle_VehicleInvincibility ? "~g~Enabled" : "~r~Disabled");
						}
						else
							_PRINT_TEXT("~r~You must be in a vehicle to use this option");
					}
					else if (Menu_Scroll == 10) // Toggle Invisibility
					{
						bool invisibility = !IS_ENTITY_VISIBLE(_Vehicle);
						SET_ENTITY_VISIBLE(_Vehicle, invisibility);
						_PRINT_TEXT("Vehicle Invisibility %s", invisibility ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 11) // Toggle NOS
					{
						Toggle_NOS = !Toggle_NOS;
						_PRINT_TEXT("NOS %s", Toggle_NOS ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 12) // Toggle Parachute Eject
					{
						Toggle_ParachuteEject = !Toggle_ParachuteEject;
						_PRINT_TEXT("Parachute Eject %s", Toggle_ParachuteEject ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 13) // Toggle Rainbow Paint
					{
						Toggle_RainbowPaint = !Toggle_RainbowPaint;
						_PRINT_TEXT("Rainbow Paint %s", Toggle_RainbowPaint ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 14) // Toggle Rainbow Tire Smoke
					{
						Toggle_RainbowSmoke = !Toggle_RainbowSmoke;
						_PRINT_TEXT("Rainbow Tire Smoke %s", Toggle_RainbowSmoke ? "~g~Enabled" : "~r~Disabled");
					}
					else if (Menu_Scroll == 15) // Toggle Speedometer
					{
						Toggle_Speedometer = !Toggle_Speedometer;
						_PRINT_TEXT("Speedometer %s", Toggle_Speedometer ? "~g~Enabled" : "~r~Disabled");
					}
				}
				else if (Menu_Index == SubMenus::VehicleMods_VehicleEditor)
				{
					if (Menu_Scroll == 0) // Armor
					{
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Armor;
					}
					else if (Menu_Scroll == 1) // Brakes
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Brakes;
					}
					else if (Menu_Scroll == 2) // Bumpers
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Bumpers;
					}
					else if (Menu_Scroll == 3) // Engine
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Engine;
					}
					else if (Menu_Scroll == 4) // Exhaust
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Exhaust;
					}
					else if (Menu_Scroll == 5) // Explosives
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Explosives;
					}
					else if (Menu_Scroll == 6) // Horn
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Horn;
					}
					else if (Menu_Scroll == 7) // Lights
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Lights;
					}
					else if (Menu_Scroll == 8) // Loss / Theft Protection
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_LossTheftProtection;
					}
					else if (Menu_Scroll == 9) // Plate
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Plate;
					}
					else if (Menu_Scroll == 10) // Respray
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Respray;
					}
					else if (Menu_Scroll == 11) // Skirts
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Skirts;
					}
					else if (Menu_Scroll == 12) // Spoiler
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Spoiler;
					}
					else if (Menu_Scroll == 13) // Suspension
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Suspension;
					}
					else if (Menu_Scroll == 14) // Transmission
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Transmission;
					}
					else if (Menu_Scroll == 15) // Turbo
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Turbo;
					}
					else if (Menu_Scroll == 16) // Wheels
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Wheels;
					}
					else if (Menu_Scroll == 17) // Windows
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleEditor_Windows;
					}
				}
				else if (Menu_Index == SubMenus::VehicleMods_VehicleSpawner)
				{
					if (Menu_Scroll == 0) // Boats
					{
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Boats;
					}
					else if (Menu_Scroll == 1) // Commercial
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Commercial;
					}
					else if (Menu_Scroll == 2) // Compacts
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Compacts;
					}
					else if (Menu_Scroll == 3) // Coupes
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Coupes;
					}
					else if (Menu_Scroll == 4) // Cycles
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Cycles;
					}
					else if (Menu_Scroll == 5) // Emergency
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Emergency;
					}
					else if (Menu_Scroll == 6) // Helicopters
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Helicopters;
					}
					else if (Menu_Scroll == 7) // Industrial
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Industrial;
					}
					else if (Menu_Scroll == 8) // Military
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Military;
					}
					else if (Menu_Scroll == 9) // Motorcycles
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Motorcycles;
					}
					else if (Menu_Scroll == 10) // Muscle
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Muscle;
					}
					else if (Menu_Scroll == 11) // OffRoad
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_OffRoad;
					}
					else if (Menu_Scroll == 12) // Planes
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Planes;
					}
					else if (Menu_Scroll == 13) // Sedans
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Sedans;
					}
					else if (Menu_Scroll == 14) // Service
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Service;
					}
					else if (Menu_Scroll == 15) // Sports
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Sports;
					}
					else if (Menu_Scroll == 16) // Sports Classics
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_SportsClassics;
					}
					else if (Menu_Scroll == 17) // SUV
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Super;
					}
					else if (Menu_Scroll == 17) // Utility
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Utility;
					}
					else if (Menu_Scroll == 17) // Vans
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::VehicleMods_VehicleSpawner_Vans;
					}
				}
				else if (Menu_Index == SubMenus::WorldEditor)
				{
					if (Menu_Scroll == 0) // Forge
					{
						Menu_Index = SubMenus::WorldEditor_Forge;
					}
					else if (Menu_Scroll == 1) // Gravity
					{
						char* szGravity = XKeyboard(VKBD_LATIN_NUMERIC, L"", L"Gravity Editor", L"The value you pass through this dialog will directly change the ingame gravity.\nThe default value for gravity is 9.8, use that information as you will.");
						if (strcmp(szGravity, "") != 0)
						{
							float fGravity = (float)atof(szGravity);
							SET_GRAVITY_LEVEL(fGravity);
							_PRINT_TEXT("Gravity set to %f", fGravity);
						}
					}
					else if (Menu_Scroll == 2) // Timescale
					{
						char* szTimescale = XKeyboard(VKBD_LATIN_NUMERIC, L"", L"Timescale Editor", L"The value you pass through this dialog will directly change the ingame timescale.\nThe default value for gravity is 1.0, use that information as you will.");
						if (strcmp(szTimescale, "") != 0)
						{
							float fTimescale = (float)atof(szTimescale);
							SET_TIME_SCALE(fTimescale);
							_PRINT_TEXT("Timescale set to %f", fTimescale);
						}
					}
					else if (Menu_Scroll == 3) // Weather
					{
						Menu_Scroll = 0;
						Menu_Index = SubMenus::WorldEditor_Weather;
					}
				}
				else if (Menu_Index == SubMenus::WorldEditor_Forge)
				{
					if (Menu_Scroll == 0)
					{
						Menu_Index = SubMenus::WorldEditor_Forge_ObjectEditor;
					}
					else if (Menu_Scroll == 1)
					{
						char* szObject = XKeyboard(VKBD_LATIN_NUMERIC, L"", L"Object Spawner", L"The string you pass this through this dialog must be a valid GTA V object name (before hashing).\nYou can find a list of objects here: http://ecb2.biz/releases/GTAV/lists/objects.txt");
						DWORD dwObject = GET_HASH_KEY(szObject);
						if (IS_MODEL_IN_CDIMAGE(dwObject))
						{
							float fHeading = GET_ENTITY_HEADING(_Ped);
							Position position = _Position;
							position.x += 10 * sin(-1 * fHeading);
							position.y += 10 * cos(-1 * fHeading);
							CREATE_OBJECT(dwObject, position.x, position.y, position.z);
							_PRINT_TEXT("Spawned ~g~%s", szObject);
						}
					}
					else if (Menu_Scroll == 2)
					{
						Forge_Toggle = !Forge_Toggle;
						_PRINT_TEXT("Forge %s", Forge_Toggle ? "~g~Enabled" : "~r~Disabled");
					}
				}
				else if (Menu_Index == SubMenus::WorldEditor_Forge_ObjectEditor)
				{
					if (Menu_Scroll == 0) // Copy Object
					{
						Forge_Object = CREATE_OBJECT(Forge_ObjectHash, Forge_ObjectPosition.x, Forge_ObjectPosition.y, Forge_ObjectPosition.z);
						SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x, Forge_ObjectRotation.y, Forge_ObjectRotation.z);
						FREEZE_ENTITY_POSITION(Forge_Object, true);
						_PRINT_TEXT("~g~Copied ~w~Object");
					}
					else if (Menu_Scroll == 1) // Delete Object
					{
						DELETE_OBJECT(Forge_Object);
						_PRINT_TEXT("~r~Deleted ~w~Object");
					}
					else if (Menu_Scroll == 2) // Get Object Coordinates
					{
						_PRINT_TEXT("Coordinates: { ~g~%f~w~, ~g~%f~w~, ~g~%f ~w~}", Forge_ObjectPosition.x, Forge_ObjectPosition.y, Forge_ObjectPosition.z);
					}
					else if (Menu_Scroll == 3) // Get Object Hash
					{
						_PRINT_TEXT("Hash: ~g~0x%X", Forge_ObjectHash);
					}
					else if (Menu_Scroll == 4) // Increase Heading
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x, Forge_ObjectRotation.y, Forge_ObjectRotation.z += 2.0);
							_PRINT_TEXT("~g~Increased ~w~Heading");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 5) // Decrease Heading
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x, Forge_ObjectRotation.y, Forge_ObjectRotation.z -= 2.0);
							_PRINT_TEXT("~g~Decreased ~w~Heading");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 6) // Increase Pitch
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x += 2.0, Forge_ObjectRotation.y, Forge_ObjectRotation.z);
							_PRINT_TEXT("~g~Increased ~w~Pitch");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 7) // Decrease Pitch
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x -= 2.0, Forge_ObjectRotation.y, Forge_ObjectRotation.z);
							_PRINT_TEXT("~g~Decreased ~w~Pitch");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 8) // Increase Roll
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x, Forge_ObjectRotation.y += 2.0, Forge_ObjectRotation.z);
							_PRINT_TEXT("~g~Increased ~w~Roll");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 9) // Decrease Roll
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, Forge_ObjectRotation.x, Forge_ObjectRotation.y -= 2.0, Forge_ObjectRotation.z);
							_PRINT_TEXT("~g~Decreased ~w~Roll");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 10) // Move Up
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x += 2.0, Forge_ObjectPosition.y, Forge_ObjectPosition.z);
							_PRINT_TEXT("Moved ~g~Up");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 11) // Move Down
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x -= 2.0, Forge_ObjectPosition.y, Forge_ObjectPosition.z);
							_PRINT_TEXT("Moved ~g~Down");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 12) // Move Left
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x, Forge_ObjectPosition.y += 2.0, Forge_ObjectPosition.z);
							_PRINT_TEXT("Moved ~g~Left");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 13) // Move Right
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x, Forge_ObjectPosition.y -= 2.0, Forge_ObjectPosition.z);
							_PRINT_TEXT("Moved ~g~Right");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 14) // Move Forward
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x, Forge_ObjectPosition.y, Forge_ObjectPosition.z += 2.0);
							_PRINT_TEXT("Moved ~g~Forward");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 15) // Move Backward
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x, Forge_ObjectPosition.y, Forge_ObjectPosition.z -= 2.0);
							_PRINT_TEXT("Moved ~g~Backward");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 16) // Snap to the ground
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_COORDS(Forge_Object, Forge_ObjectPosition.x, Forge_ObjectPosition.y, Forge_ObjectPosition.z = GET_GROUND_Z_FOR_3D_COORD(Forge_ObjectPosition.x, Forge_ObjectPosition.y, Forge_ObjectPosition.z));
							_PRINT_TEXT("~g~Snapped to the ground");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
					else if (Menu_Scroll == 17) // Straighten Object
					{
						if (NETWORK_REQUEST_CONTROL_OF_ENTITY(Forge_Object))
						{
							SET_ENTITY_ROTATION(Forge_Object, 0.0f, 0.0f, 0.0f);
							_PRINT_TEXT("~g~Straightened object");
						}
						else
							_PRINT_TEXT("~r~You don't have control over this object");
					}
				}
				else if (Menu_Index == SubMenus::WorldEditor_Weather)
				{
					if (Menu_Scroll == 0) // Blizzard
					{
						SET_OVERRIDE_WEATHER("BLIZZARD");
						_PRINT_TEXT("Changed weather to ~g~Blizzard");
					}
					else if (Menu_Scroll == 1) // Clear
					{
						SET_OVERRIDE_WEATHER("CLEAR");
						_PRINT_TEXT("Changed weather to ~g~Clear");
					}
					else if (Menu_Scroll == 2) // Clearing
					{
						SET_OVERRIDE_WEATHER("CLEARING");
						_PRINT_TEXT("Changed weather to ~g~Clearing");
					}
					else if (Menu_Scroll == 3) // Clouds
					{
						SET_OVERRIDE_WEATHER("CLOUDS");
						_PRINT_TEXT("Changed weather to ~g~Clouds");
					}
					else if (Menu_Scroll == 4) // Extra Sunny
					{
						SET_OVERRIDE_WEATHER("EXTRASUNNY");
						_PRINT_TEXT("Changed weather to ~g~Extra Sunny");
					}
					else if (Menu_Scroll == 5) // Foggy
					{
						SET_OVERRIDE_WEATHER("FOGGY");
						_PRINT_TEXT("Changed weather to ~g~Foggy");
					}
					else if (Menu_Scroll == 6) // Neutral
					{
						SET_OVERRIDE_WEATHER("NEUTRAL");
						_PRINT_TEXT("Changed weather to ~g~Neutral");
					}
					else if (Menu_Scroll == 7) // Overcast
					{
						SET_OVERRIDE_WEATHER("OVERCAST");
						_PRINT_TEXT("Changed weather to ~g~Overcast");
					}
					else if (Menu_Scroll == 8) // Rain
					{
						SET_OVERRIDE_WEATHER("RAIN");
						_PRINT_TEXT("Changed weather to ~g~Rain");
					}
					else if (Menu_Scroll == 9) // Smog
					{
						SET_OVERRIDE_WEATHER("SMOG");
						_PRINT_TEXT("Changed weather to ~g~Smog");
					}
					else if (Menu_Scroll == 10) // Snow
					{
						SET_OVERRIDE_WEATHER("SNOW");
						_PRINT_TEXT("Changed weather to ~g~Snow");
					}
					else if (Menu_Scroll == 11) // Snow Light
					{
						SET_OVERRIDE_WEATHER("SNOWLIGHT");
						_PRINT_TEXT("Changed weather to ~g~Snow Light");
					}
					else if (Menu_Scroll == 12) // Thunder
					{
						SET_OVERRIDE_WEATHER("THUNDER");
						_PRINT_TEXT("Changed weather to ~g~Thunder");
					}
				}
				else if (Menu_Index == SubMenus::TeleportMenu)
				{
					if (Menu_Scroll == 0)
					{

					}
				}
			}
			else if (Controller->MAIN_B_JP)
			{
				if (Menu_Index == SubMenus::MainMods ||
					Menu_Index == SubMenus::PlayerMods ||
					Menu_Index == SubMenus::WeaponMods ||
					Menu_Index == SubMenus::VehicleMods ||
					Menu_Index == SubMenus::WorldEditor ||
					Menu_Index == SubMenus::TeleportMenu)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::MainMenu;
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::PlayerMods;
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::PlayerMods_OnlinePlayers;
				}
				else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions ||
					Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions ||
					Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions ||
					Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions ||
					Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer;
				}
				else if (Menu_Index == SubMenus::WeaponMods_CannonOptions)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::WeaponMods;
				}
				else if (Menu_Index == SubMenus::VehicleMods_VehicleEditor ||
					Menu_Index == SubMenus::VehicleMods_VehicleSpawner)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::VehicleMods;
				}
				else if (Menu_Index == SubMenus::WorldEditor_Forge ||
					Menu_Index == SubMenus::WorldEditor_Weather)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::WorldEditor;
				}
				else if (Menu_Index == SubMenus::WorldEditor_Forge_ObjectEditor)
				{
					Menu_Scroll = 0;
					Menu_Index = SubMenus::WorldEditor_Forge;
				}
			}
		}
	}
}
void TheSaiyanProject_Reset()
{
	// GTA Vars
	_Entity = 0;
	_Ped = 0;
	_Player = 0;
	_Position = Position();
	_Vehicle = 0;

	// Menu Vars
	Menu_CanOpen = true;
	Menu_Index = -1;
	Menu_Open = false;
	Menu_Scroll = 0;
	Menu_SelectedPlayer = 0;
	Toggle_ESP = false;
	Toggle_ExplosiveAmmo = false;
	Toggle_FireAmmo = false;
	Toggle_FireworkAmmo = false;
	Toggle_FlyingBikes = false;
	Toggle_FPSCounter = false;
	Toggle_HandbrakeDrift = false;
	Toggle_Hydraulics = false;
	Toggle_LaserSight = false;
	Toggle_Moses = false;
	Toggle_NoWanted = false;
	Toggle_Noclip = false;
	Toggle_NOS = false;
	Toggle_ParachuteEject = false;
	Toggle_RainbowPaint = false;
	Toggle_RainbowSmoke = false;
	Toggle_SharkOMatic = false;
	Toggle_Speedometer = false;
	Toggle_SuperJump = false;
	Toggle_SuperSpeed = false;
	Toggle_SupermanPunch = false;
	Toggle_TeleportGun = false;
	Toggle_UnlimitedAmmo = false;
	Toggle_VehicleCollision = false;
	Toggle_VehicleInvincibility = false;
	Toggle_Whiplash = false;

	// Native Hook
	NativeHitCount = 0;
	Node* curNode = NativePoolHead;
	NativePoolHead = NULL;
	NativePool = NULL;
	while (curNode != NULL)
	{
		Node* next = curNode->getNext();
		delete curNode;
		curNode = next;
	}

	// Scripting
	Aimbot_Arrest = false;
	Aimbot_ArrestHandle = 0;
	Aimbot_ArrestHit = 0;
	Aimbot_Client = 0;
	Aimbot_HasAimed = false;
	Aimbot_HasAimedAtPlayer = false;
	Aimbot_Toggle = false;
	FirstPerson_Camera = 0;
	FirstPerson_Check = false;
	FirstPerson_Load = false;
	FirstPerson_Toggle = false;
	Forge_Object = 0;
	Forge_ObjectHash = 0;
	Forge_ObjectPosition = Position(0, 0, 0);
	Forge_ObjectRotation = Position(0, 0, 0);
	Forge_Toggle = false;
	GrapplingHook_MovingToTarget = false;
	GrapplingHook_Rope = 0;
	GrapplingHook_Toggle = false;
	WeaponCannon_Toggle = false;
	WeaponCannon_Type = WeaponCannon_Projectiles::Object;
	Zombies_Toggle = false;

	// Virtual Machine
	Check_Airstrike = -1;
	Check_SetModel = 0;
	Check_SpawnVeh_Fulfilled = false;
	Check_SpawnVeh_Handle = 0;
	Check_SpawnVeh_Hash = 0;
	Check_SpawnVeh_Position = Position(0, 0, 0);
	memset(Request_AnimDict, '\0', sizeof(Request_AnimDict));
	memset(Request_ClipSet, '\0', sizeof(Request_ClipSet));
	Request_Hash = 0;

	// XUID Spoof
	memset(XUIDSpoof_Gamertag, '\0', sizeof(XUIDSpoof_Gamertag));
	XUIDSpoof_Spoofing = false;
	XUIDSpoof_XUID = 0;

	// Other
	bInitialized = false;
	bListInitialized = false;
}
void TheSaiyanProject_SpoofyCheck()
{
	BYTE HV_0xFF90[0x10];
	HvPeekBytes(0xFF90, &HV_0xFF90, 0x10);
	
	bool flag = false;
	for (int i = 0; i < 0x10; i++)
	{
		if (HV_0xFF90[i] != 0)
		{
			flag = true;
			break;
		}
	}

	if (flag)
	{
		// user is trying to spoof their CPU key
	}
}
void TheSaiyanProject_Start()
{
	TheSaiyanProject_CreateMenu();

	DWORD ActiveTitle;
	while (true)
	{
		if (bPlayingGTAV = ((ActiveTitle = XamGetCurrentTitleId()) == ID_GTAV))
		{
			if (!bInitialized)
			{
				// Patch GTA V (instruction/data patches)
				*(DWORD*)0x827D2164 = 0x60000000; // Breakpoint
				*(DWORD*)0x82D5C960 = 0x60000000; // Vehicle Creation Bypass
				ZeroMemory((PVOID)0x8204DF70, 11); // MAC Bypass

				// Patch GTA V (asm patches)
				memcpy((PVOID)0x82C8B3D8, &TheSaiyanProject_DevMode, 0x60); // Dev Mode

#ifdef STAFF
				// Patch GTA V (staff patches)
				*(DWORD*)0x82CF7704 = 0x480000F8; // Money Drop Bypass
				*(DWORD*)0x83288A30 = 0x48000104; // Update.rpf Bypass
				memcpy((PVOID)0x82FDB57C, &TheSaiyanProject_ScriptRPFBypass, 0x18); // Script.rpf Bypass
#endif
				
				// Patch GTA V (hooks)
				HookFunctionStart((PDWORD)0x83524A28, (PDWORD)PushNative_Stub, (DWORD)PushNative); // Native Hook
				HookFunctionStart((PDWORD)0x83525E00, (PDWORD)ParseScript_Stub, (DWORD)ParseScript); // Script Hook
				PatchInJump((PDWORD)0x83525A10, (DWORD)Timer, false); // TIMERA Hook

				// Create handler
				HANDLE hThread;
				DWORD dwThread;
				ExCreateThread(&hThread, 0, &dwThread, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_InputHandler, 0, 2);
				XSetThreadProcessor(hThread, 4);
				ResumeThread(hThread);
				CloseHandle(hThread);

				// Let ourselves know that we've initialized
				bInitialized = true;

				// Display
				XMessageBox(L"The Saiyan Project", L"Welcome to The Saiyan Project\nVersion: 0.8\n\nGo to http://www.extforum.net/ for the latest and greatest in Xbox 360 hacks! Want to buy some GTA V source code? I'm selling this source along with my GTA V menu base on ExT!\n\nCredits:\n- XeClutch: Script Hook, Dev Mode, Ban Bypass, and most scripts\n- proditaki: Vehicle Spawning", XMB_NOICON, true);
				
				// Spoof XUID
				LPCWSTR pwszButtons[] = { L"Yes", L"No" };
				MESSAGEBOX_RESULT Result = XMessageBox(L"The Saiyan Project", L"Would you like to spoof your XUID?", 2, pwszButtons, XMB_ALERTICON);
				if (Result.dwButtonPressed == 1)
				{
					XUID xuid;
					XUserGetXUID(0, &xuid);

					char* szGamertag = XKeyboard(VKBD_LATIN_GAMERTAG, L"", L"The Saiyan Project", L"Enter the gamertag of the user you want to spoof your XUID to.");
					FIND_USER_INFO findUserInfo;
					findUserInfo.qwUserId = 0;
					strcpy((char*)findUserInfo.szGamerTag, szGamertag);
					FIND_USERS_RESPONSE findUsersResponse;

					XOnlineStartup();
					if (!XUserFindUsers(xuid, 1, &findUserInfo, XUserFindUsersResponseSize(1), &findUsersResponse, 0))
					{
						strcpy(XUIDSpoof_Gamertag, findUsersResponse.pUsers->szGamerTag);
						XUIDSpoof_Spoofing = true;
						XUIDSpoof_XUID = findUsersResponse.pUsers->qwUserId;
						PatchModuleImport("default.xex", "xam.xex", 526, (DWORD)XamUserGetNameHook);
						PatchModuleImport("default.xex", "xam.xex", 522, (DWORD)XamUserGetXUIDHook);
						PatchModuleImport("default.xex", "xam.xex", 551, (DWORD)XamUserGetSigninInfoHook);

						char* szNotify = va("Gamertag: %s\nXUID: %16X", szGamertag, xuid);
						PWCHAR wszNotify;
						mbstowcs(wszNotify, szNotify, strlen(szNotify));
						XNotify(wszNotify);
					}
					else
					{
						char* szNotify = va("Unable to calculate %s's XUID", szGamertag);
						PWCHAR wszNotify;
						mbstowcs(wszNotify, szNotify, strlen(szNotify));
						XNotify(wszNotify);
					}
				}
			}
		}
		else if (ActiveTitle == ID_Destiny)
		{
			if (bInitialized)
				TheSaiyanProject_Reset();

			// CurrentMods Destiny Bypass (ripped from .xex because Domo is a skid)
			*(DWORD*)0xD95A05AF = 1;
			*(DWORD*)0xD95A1488 = 1;
			*(DWORD*)0xD95B1988 = 1;
			*(DWORD*)0xD95A05AC = 1;
			*(DWORD*)0xD95A1487 = 1;
			Sleep(50);
		}
		else
		{
			if (bInitialized)
			{
				TheSaiyanProject_Reset();
			}
		}
	}
}

//// Entry Point
BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Main
		HANDLE hThread;
		DWORD dwThread;
		ExCreateThread(&hThread, 0, &dwThread, (PVOID)XapiThreadStartup, (LPTHREAD_START_ROUTINE)TheSaiyanProject_Start, 0, 2);
		XSetThreadProcessor(hThread, 4);
		ResumeThread(hThread);
		CloseHandle(hThread);
	}
	return TRUE;
}