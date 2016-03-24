//// Includes
#include "stdafx.h"
#include "ButtonMonitor.h"
#include "Globals.h"
#include "Hooks.h"
#include "MenuDisplay.h"
#include "Natives.h"
#include "Tools.h"
#include <stdio.h>
using namespace std;

//// Variables
/// GTA Vars
extern Entity _Entity;
extern Ped _Ped;
extern Player _Player;
extern Position _Position;
extern Vehicle _Vehicle;
extern bool _VehicleExists;
/// Menu Display
extern MenuDisplay mdCurrentMenu;
extern MenuDisplay mdMainMenu;
extern MenuDisplay mdMainMods;
extern MenuDisplay mdPlayerMods;
extern MenuDisplay mdPlayerMods_OnlinePlayers;
extern MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer;
extern MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions;
extern MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions;
extern MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions;
extern MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions;
extern MenuDisplay mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions;
extern MenuDisplay mdWeaponMods;
extern MenuDisplay mdWeaponMods_CannonOptions;
extern MenuDisplay mdVehicleMods;
extern MenuDisplay mdVehicleMods_VehicleEditor;
extern MenuDisplay mdVehicleMods_VehicleSpawner;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Boat;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Commercial;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Compact;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Coupe;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Cycle;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Emergency;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Helicopter;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Industrial;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Military;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Motorcycle;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Muscle;
extern MenuDisplay mdVehicleMods_VehicleSpawner_OffRoad;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Plane;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Sedan;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Service;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Sport;
extern MenuDisplay mdVehicleMods_VehicleSpawner_SportClassic;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Super;
extern MenuDisplay mdVehicleMods_VehicleSpawner_SUV;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Utility;
extern MenuDisplay mdVehicleMods_VehicleSpawner_Van;
extern MenuDisplay mdWorldEditor;
extern MenuDisplay mdWorldEditor_Forge;
extern MenuDisplay mdWorldEditor_Forge_ObjectEditor;
extern MenuDisplay mdWorldEditor_Weather;
extern MenuDisplay mdTeleportMenu;
/// Menu Vars
extern DWORD Menu_Index;
extern bool Menu_Open;
extern DWORD Menu_Scroll;
extern DWORD Menu_SelectedPlayer;
extern bool Toggle_ESP;
extern bool Toggle_ExplosiveAmmo;
extern bool Toggle_FireAmmo;
extern bool Toggle_FlyingBikes;
extern bool Toggle_FPSCounter;
extern bool Toggle_HandbrakeDrift;
extern bool Toggle_Hydraulics;
extern bool Toggle_Moses;
extern bool Toggle_NoWanted;
extern bool Toggle_Noclip;
extern bool Toggle_NOS;
extern bool Toggle_RainbowPaint;
extern bool Toggle_RainbowSmoke;
extern bool Toggle_SuperJump;
extern bool Toggle_SupermanPunch;
extern bool Toggle_Speedometer;
extern bool Toggle_TeleportGun;
extern bool Toggle_Whiplash;
/// Native Hook
extern DWORD NativeHitCount;
extern Node* NativePool;
extern Node* NativePoolHead;
/// Scripting
// First Person
extern Camera FirstPerson_Camera;
extern bool FirstPerson_Check;
extern bool FirstPerson_Load;
extern bool FirstPerson_Toggle;
// Zombies
extern bool Zombies_Toggle;
/// Other
extern bool bListInitialized;
extern ButtonMonitor* Controller;
/// Virtual Machine
extern DWORD Check_Airstrike;
extern DWORD Check_SetModel;
extern Vehicle Check_SpawnVeh_Handle;
extern DWORD Check_SpawnVeh_Hash;
extern Position Check_SpawnVeh_Position;
extern char* Request_AnimDict;
extern char* Request_ClipSet;
extern DWORD Request_Hash;
/// XUID Spoof
extern char* XUIDSpoof_Gamertag;
extern bool XUIDSpoof_Spoofing;
extern XUID XUIDSpoof_XUID;

//// Methods
DWORD NetworkPlayerIdToInt()
{
	DWORD Result = PLAYER_ID();

	#pragma region Spawn Vehicle
	if (Check_SpawnVeh_Hash != 0)
	{
		REQUEST_MODEL(Check_SpawnVeh_Hash);

		float speed;
		if (_Vehicle != 0)
		{
			speed = GET_ENTITY_SPEED(_Vehicle);
			SET_ENTITY_AS_MISSION_ENTITY(_Vehicle, false, true);
			DELETE_VEHICLE(_Vehicle);
		}
		else
			speed = GET_ENTITY_SPEED(_Ped);

		float x, y, z;
		if (Check_SpawnVeh_Position == Position())
		{
			x = _Position.x;
			y = _Position.y;
			z = _Position.z;
		}
		else
		{
			x = Check_SpawnVeh_Position.x;
			y = Check_SpawnVeh_Position.y;
			z = Check_SpawnVeh_Position.z;
		}

		if (HAS_MODEL_LOADED(Check_SpawnVeh_Hash))
		{
			Vehicle vehicle = CREATE_VEHICLE(Check_SpawnVeh_Hash, x, y, z, GET_ENTITY_HEADING(_Ped), true, false);
			if (DOES_ENTITY_EXIST(vehicle))
			{
				SET_PED_INTO_VEHICLE(_Ped, vehicle, -1);
				SET_VEHICLE_ENGINE_ON(vehicle, true, true);
				SET_HELI_BLADES_FULL_SPEED(vehicle);
				SET_VEHICLE_FORWARD_SPEED(vehicle, speed);
				SET_MODEL_AS_NO_LONGER_NEEDED(Check_SpawnVeh_Hash);

				Check_SpawnVeh_Hash = 0;
				Check_SpawnVeh_Handle = vehicle;
				Check_SpawnVeh_Position = Position();
			}
		}
	}
	#pragma endregion

	return Result;
}
QWORD ParseScript(DWORD dwStatics, DWORD dwGlobalVars, DWORD dwHeader, DWORD unk2, QWORD unk3, QWORD unk4)
{
	volatile QWORD Result = ParseScript_Stub(dwStatics, dwGlobalVars, dwHeader, unk2, unk3, unk4);

	#pragma region Check
	if (Check_Airstrike != -1)
	{
		Position pos = GET_ENTITY_COORDS(GET_PLAYER_PED(Check_Airstrike));
		_CALL_AIRSTRIKE(pos.x, pos.y, pos.z);
		Check_Airstrike = -1;
	}
	if (Check_SetModel != 0)
	{
		if (IS_MODEL_IN_CDIMAGE(Check_SetModel))
		{
			REQUEST_MODEL(Check_SetModel);
			if (HAS_MODEL_LOADED(Check_SetModel))
			{
				SET_PLAYER_MODEL(_Player, Check_SetModel);
				SET_MODEL_AS_NO_LONGER_NEEDED(Check_SetModel);
				SET_PED_RANDOM_COMPONENT_VARIATION(_Ped);
				Check_SetModel = 0;
			}
			else
				_PRINT_TEXT("~r~Please try again");
		}
		else
		{
			_PRINT_TEXT("~r~Invalid Model");
			Check_SetModel = 0;
		}
	}
	/*if (Check_SpawnVeh_Hash != 0)
	{
		if (Check_SpawnVeh_Position == Position())
			Check_SpawnVeh_Handle = _SPAWN_VEHICLE(Check_SpawnVeh_Hash);
		else
			Check_SpawnVeh_Handle = _SPAWN_VEHICLE(Check_SpawnVeh_Hash, Check_SpawnVeh_Position.x, Check_SpawnVeh_Position.y, Check_SpawnVeh_Position.z);

		Check_SpawnVeh_Hash = 0;
		Check_SpawnVeh_Position = Position();
	}*/
	#pragma endregion

	#pragma region Request
	if (Request_Hash != 0)
	{
		REQUEST_MODEL(Request_Hash);
		Request_Hash = 0;
	}
	if (strcmp(Request_AnimDict, "") != FALSE)
	{
		_REQUEST_ANIM_DICT(Request_AnimDict);
		memset(Request_AnimDict, '\0', sizeof(Request_AnimDict));
	}
	if (strcmp(Request_ClipSet, "") != FALSE)
	{
		_REQUEST_CLIP_SET(Request_ClipSet);
		memset(Request_ClipSet, '\0', sizeof(Request_ClipSet));
	}
	#pragma endregion

	#pragma region NativeHitCount
	NativeHitCount++;
	if (NativeHitCount != 1)
	{
		if (NativeHitCount > 40)
			NativeHitCount = 0;
		return Result;
	}
	#pragma endregion

	#pragma region User Staples
	if (Menu_Index != -1)
	{
		_Entity = _Ped = PLAYER_PED_ID();
		_Player = PLAYER_ID();
		_Position = GET_ENTITY_COORDS(_Vehicle == 0 ? _Ped : _Vehicle);
		_VehicleExists = IS_PED_IN_ANY_VEHICLE(_Ped);
		_Vehicle = _VehicleExists ? GET_VEHICLE_PED_IS_IN(_Ped) : 0;
	}
	#pragma endregion

	#pragma region First Person
	if (FirstPerson_Load)
	{
		FirstPerson_Camera = CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", true);
		ATTACH_CAM_TO_PED_BONE(FirstPerson_Camera, _Ped, Assets_Models_BoneIndex::SKEL_Head, 0.1, 0.07, 0.002, true);
		SET_CAM_ACTIVE(FirstPerson_Camera, true);
		RENDER_SCRIPT_CAMS(true, false, 3000, true, false);
		FirstPerson_Check = true;
		FirstPerson_Load = false;
	}
	if (FirstPerson_Check)
	{
		SET_CAM_ACTIVE(FirstPerson_Camera, true);
		Position rot = GET_GAMEPLAY_CAM_ROT(2);
		SET_CAM_ROT(FirstPerson_Camera, rot.x, rot.y, rot.z);
	}
	#pragma endregion

	NativeHitCount = 0;
	return Result;
}
void PushNative(DWORD dwNativeTable, DWORD dwNativeHash, DWORD dwNativeAddress)
{
	if (!bListInitialized)
	{
		NativePoolHead = new Node(dwNativeHash, dwNativeAddress);
		NativePool = NativePoolHead;
		bListInitialized = true;
	}
	else
	{
		Node* newNode = new Node(dwNativeHash, dwNativeHash == 0x8DD5B838 ? (DWORD)NetworkPlayerIdToInt : dwNativeAddress);
		NativePool->next = newNode;
		NativePool = newNode;
	}
	PushNative_Stub(dwNativeTable, dwNativeHash, dwNativeHash == 0x8DD5B838 ? (DWORD)NetworkPlayerIdToInt : dwNativeAddress);
}
void Timer()
{
	__asm // This is the TIMERA disassembly
	{
		lwz r11, 0(r13)
		li r10, 0x134
		lwz r9, 0(r3)
		lwzx r8, r10, r11
		lwz r7, 0x1C(r8)
		stw r7, 0(r9)
	}

	BYTE RGB[] = { 0, 0, 0 };
	for (int i = 0; i < 3; i++)
		RGB[i] = (BYTE)GET_RANDOM_INT_IN_RANGE(0, 254);

	#pragma region Player Cheats
	#pragma region Moses
	if (Toggle_Moses)
	{
		MODIFY_WATER(_Position.x, _Position.y, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y + 5, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y + 4, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y + 3, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y + 2, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y + 1, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y - 1, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y - 2, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y - 3, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y - 4, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y - 5, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y + 3, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y, _Position.z + 3, -FLT_MAX);
		MODIFY_WATER(_Position.x + 3, _Position.y, _Position.z, -FLT_MAX);
		MODIFY_WATER(_Position.x, _Position.y, _Position.z - 3, -FLT_MAX);
		MODIFY_WATER(_Position.x - 3, _Position.y, _Position.z, -FLT_MAX);
	}
	#pragma endregion
	#pragma region No Wanted
	if (Toggle_NoWanted)
		CLEAR_PLAYER_WANTED_LEVEL(_Player);
	#pragma endregion
	#pragma region Noclip
	if (Toggle_Noclip)
	{
		Position dir = _GET_CAMERA_DIRECTION();
		Position rot = GET_GAMEPLAY_CAM_ROT();
		float speed = 0.999999;

		if (Controller->TRIGGER_LEFT)
			speed -= 0.4;

		Entity ent = _VehicleExists ? _Ped : _Vehicle;
		Position pos = _Position;

		SET_ENTITY_COLLISION(ent, true);
		SET_ENTITY_ROTATION(ent, rot.x, rot.y, rot.z);
		SET_ENTITY_COORDS(ent, pos.x, pos.y, pos.z);
		LOAD_ALL_OBJECTS_NOW();
		if (Controller->THUMBSTICK_RIGHT)
		{
			if (ent == _Vehicle)
			{
				SET_ENTITY_COLLISION(ent, false);
				SET_ENTITY_ROTATION(ent, rot.x, rot.y, rot.z);
			}
			SET_ENTITY_COORDS(ent, pos.x + (dir.x * speed), pos.y + (dir.y * speed), pos.z + (dir.z * speed));
			LOAD_ALL_OBJECTS_NOW();
		}
	}
	#pragma endregion
	#pragma region Whiplash
	if (Toggle_Whiplash)
	{
		if (!_VehicleExists)
			_APPLY_RELATIVE_FORCE(_Ped, Position(0.0f, 0.0f, 10.0f));
	}
	#pragma endregion
	#pragma endregion

	#pragma region Vehicle Cheats
	if (_VehicleExists)
	{
		#pragma region Flying Bikes
		if (Toggle_FlyingBikes)
		{
			if (IS_PED_ON_ANY_BIKE(_Ped))
			{
				if (Controller->TRIGGER_LEFT)
					if (!IS_VEHICLE_ON_ALL_WHEELS(_Vehicle))
						SET_VEHICLE_FORWARD_SPEED(_Vehicle, (GET_ENTITY_SPEED(_Vehicle) / 1.05));

				if (Controller->TRIGGER_RIGHT)
					SET_VEHICLE_FORWARD_SPEED(_Vehicle, (GET_ENTITY_SPEED(_Vehicle) * 1.05));
			}
		}
		#pragma endregion
		#pragma region Handbrake Drift
		if (Toggle_HandbrakeDrift)
		{
			SET_VEHICLE_REDUCE_GRIP(_Vehicle, Controller->BUMPER_RIGHT);
		}
		#pragma endregion
		#pragma region Hydraulics
		if (Toggle_Hydraulics)
		{
			if (Controller->MAIN_X && IS_VEHICLE_ON_ALL_WHEELS(_Vehicle))
				APPLY_FORCE_TO_ENTITY(_Vehicle, 0, 0.0f, 0.0f, 70.0f, 0.0f, 0.0f, -70.0f, false, true, true, false, false, true);
		}
		#pragma endregion
		#pragma region NOS
		if (Toggle_NOS)
		{
			bool horn = IS_PLAYER_PRESSING_HORN(_Player);
			SET_VEHICLE_BOOST_ACTIVE(_Vehicle, horn);
			if (horn)
				APPLY_FORCE_TO_ENTITY(_Vehicle, 0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, false, true, true, true, false, true);
		}
		#pragma endregion
		#pragma region Rainbow Paint
		if (Toggle_RainbowPaint)
		{
			SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(_Vehicle, RGB[0], RGB[1], RGB[2]);
			SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(_Vehicle, RGB[1], RGB[2], RGB[0]);
		}
		#pragma endregion
		#pragma region Rainbow Smoke
		if (Toggle_RainbowSmoke)
		{
			SET_VEHICLE_TYRE_SMOKE_COLOR(_Vehicle, RGB[2], RGB[1], RGB[0]);
		}
		#pragma endregion
		#pragma region Speedometer
		if (Toggle_Speedometer)
		{
			float mps = GET_ENTITY_SPEED(_Vehicle);
			float mph = mps * 2.23694f;
			float kmh = mph * 1.609344;
			char* speedotext = va("MPH: %i\nKMH: %i", (int)mph, (int)kmh);
			_DISPLAY_TEXT(speedotext, Assets_Fonts::chalet, 0.08, 0.01);
		}
		#pragma endregion
	}
	#pragma endregion

	#pragma region Weapon Cheats
	#pragma region Explosive Ammo
	if (Toggle_ExplosiveAmmo)
		SET_EXPLOSIVE_AMMO_THIS_FRAME(_Player);
	#pragma endregion
	#pragma region Fire Ammo
	if (Toggle_FireAmmo)
		SET_FIRE_AMMO_THIS_FRAME(_Player);
	#pragma endregion
	#pragma region Super Jump
	if (Toggle_SuperJump)
		SET_SUPER_JUMP_THIS_FRAME(_Player);
	#pragma endregion
	#pragma region Superman Punch
	if (Toggle_SupermanPunch)
		SET_EXPLOSIVE_MELEE_THIS_FRAME(_Player);
	#pragma endregion
	#pragma region Teleport Gun
	if (Toggle_TeleportGun && IS_PED_SHOOTING(_Ped) && Controller->TRIGGER_RIGHT_JP)
		_TELEPORT(GET_PED_LAST_WEAPON_IMPACT_COORD(_Ped));
	#pragma endregion
	#pragma endregion

	#pragma region ESP
	if (Toggle_ESP)
	{
		for (int i = 0; i < 16; i++)
		{
			Player player = i;
			Ped ped = GET_PLAYER_PED(player);
			if (ped != 0)
			{
				Position position = GET_ENTITY_COORDS(ped);
				Position c1 = (position + Position(-0.5, -0.5, 1.0));
				Position c2 = (position + Position(-0.5, -0.5, -1.0));
				Position c3 = (position + Position(-0.5, 0.5, -1.0));
				Position c4 = (position + Position(-0.5, 0.5, 1.0));
				Position c5 = (position + Position(0.5, -0.5, 1.0));
				Position c6 = (position + Position(0.5, -0.5, -1.0));
				Position c7 = (position + Position(0.5, 0.5, -1.0));
				Position c8 = (position + Position(0.5, 0.5, 1.0));

				DRAW_LINE(c1.x, c1.y, c1.z, c2.x, c2.y, c2.z, 72, 255, 0, 255);
				DRAW_LINE(c2.x, c2.y, c2.z, c3.x, c3.y, c3.z, 72, 255, 0, 255);
				DRAW_LINE(c3.x, c3.y, c3.z, c4.x, c4.y, c4.z, 72, 255, 0, 255);
				DRAW_LINE(c4.x, c4.y, c4.z, c1.x, c1.y, c1.z, 72, 255, 0, 255);
				DRAW_LINE(c5.x, c5.y, c5.z, c6.x, c6.y, c6.z, 72, 255, 0, 255);
				DRAW_LINE(c6.x, c6.y, c6.z, c7.x, c7.y, c7.z, 72, 255, 0, 255);
				DRAW_LINE(c7.x, c7.y, c7.z, c8.x, c8.y, c8.z, 72, 255, 0, 255);
				DRAW_LINE(c8.x, c8.y, c8.z, c5.x, c5.y, c5.z, 72, 255, 0, 255);

				DRAW_LINE(c1.x, c1.y, c1.z, c5.x, c5.y, c5.z, 72, 255, 0, 255);
				DRAW_LINE(c2.x, c2.y, c2.z, c6.x, c6.y, c6.z, 72, 255, 0, 255);
				DRAW_LINE(c3.x, c3.y, c3.z, c7.x, c7.y, c7.z, 72, 255, 0, 255);
				DRAW_LINE(c4.x, c4.y, c4.z, c8.x, c8.y, c8.z, 72, 255, 0, 255);
			}
		}
	}
	#pragma endregion

	#pragma region FPS Counter
	if (Toggle_FPSCounter)
	{
		float fps = _GET_FPS();
		char* fpstext = va("FPS: %i", (int)fps);
		_DISPLAY_TEXT(fpstext, Assets_Fonts::chalet, 0.031f, 0.03f, 0.8f, 0.8f);
	}
	#pragma endregion
	
	#pragma region Menu Display
	if (Menu_Open)
	{
		mdCurrentMenu.Clear();
		if (Menu_Index == SubMenus::MainMenu) mdCurrentMenu.Copy(mdMainMenu);
		else if (Menu_Index == SubMenus::MainMods) mdCurrentMenu.Copy(mdMainMods);
		else if (Menu_Index == SubMenus::PlayerMods) mdCurrentMenu.Copy(mdPlayerMods);
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers) { mdPlayerMods_OnlinePlayers.Clear(); for (int i = 0; i < 16; i++) mdPlayerMods_OnlinePlayers.AddOption(GET_PLAYER_NAME(i)); for (int i = 16; i < 18; i++) mdPlayerMods_OnlinePlayers.AddOption(va("~r~SCTV~w~ %s", GET_PLAYER_NAME(i))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers); }
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer) { mdPlayerMods_OnlinePlayers_SelectedPlayer.SetTitle(va(mdPlayerMods_OnlinePlayers_SelectedPlayer.szMenuTitle, GET_PLAYER_NAME(Menu_SelectedPlayer))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers_SelectedPlayer); }
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions) { mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.SetTitle(va(mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions.szMenuTitle, GET_PLAYER_NAME(Menu_SelectedPlayer))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers_SelectedPlayer_CashDropOptions); }
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions) { mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.SetTitle(va(mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions.szMenuTitle, GET_PLAYER_NAME(Menu_SelectedPlayer))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers_SelectedPlayer_ExplosionOptions); }
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions) { mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions.SetTitle(va(mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions.szMenuTitle, GET_PLAYER_NAME(Menu_SelectedPlayer))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers_SelectedPlayer_WeaponOptions); }
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions) { mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.SetTitle(va(mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions.szMenuTitle, GET_PLAYER_NAME(Menu_SelectedPlayer))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers_SelectedPlayer_VehicleOptions); }
		else if (Menu_Index == SubMenus::PlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions) { mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions.SetTitle(va(mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions.szMenuTitle, GET_PLAYER_NAME(Menu_SelectedPlayer))); mdCurrentMenu.Copy(mdPlayerMods_OnlinePlayers_SelectedPlayer_TeleportOptions); }
		else if (Menu_Index == SubMenus::WeaponMods) mdCurrentMenu.Copy(mdWeaponMods);
		else if (Menu_Index == SubMenus::WeaponMods_CannonOptions) mdCurrentMenu.Copy(mdWeaponMods_CannonOptions);
		else if (Menu_Index == SubMenus::VehicleMods) mdCurrentMenu.Copy(mdVehicleMods);
		else if (Menu_Index == SubMenus::VehicleMods_VehicleEditor) mdCurrentMenu.Copy(mdVehicleMods_VehicleEditor);
		else if (Menu_Index == SubMenus::VehicleMods_VehicleSpawner) mdCurrentMenu.Copy(mdVehicleMods_VehicleSpawner);
		else if (Menu_Index == SubMenus::WorldEditor) mdCurrentMenu.Copy(mdWorldEditor);
		else if (Menu_Index == SubMenus::WorldEditor_Forge) mdCurrentMenu.Copy(mdWorldEditor_Forge);
		else if (Menu_Index == SubMenus::WorldEditor_Forge_ObjectEditor) mdCurrentMenu.Copy(mdWorldEditor_Forge_ObjectEditor);
		else if (Menu_Index == SubMenus::WorldEditor_Weather) mdCurrentMenu.Copy(mdWorldEditor_Weather);
		else if (Menu_Index == SubMenus::TeleportMenu) mdCurrentMenu.Copy(mdTeleportMenu);

		_DISABLE_CONTROLS();

		_DISPLAY_TEXT("The Saiyan Project", Assets_Fonts::sign_painter, 0.04, 0.1, 0.65, 0.65, RGB[2], RGB[0], RGB[1]);
		_DISPLAY_TEXT(mdCurrentMenu.szMenuTitle, Assets_Fonts::chalet, 0.06, 0.15, 0.5, 0.5);

		_DISPLAY_TEXT(">", Assets_Fonts::chalet, 0.08, 0.2 + (Menu_Scroll * 0.03), 0.35, 0.35, RGB[0], RGB[1], RGB[2]);
		for (int i = 0; i < mdCurrentMenu.dwOptions; i++)
			_DISPLAY_TEXT(mdCurrentMenu.vOptions[i], Assets_Fonts::chalet, 0.10, 0.20 + (i * 0.03));

		_PRINT_HELP_TEXT("The Saiyan Project v0.8");
	}
	#pragma endregion

	#pragma region Zombies
	if (Zombies_Toggle)
	{
		SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
		SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	}
	#pragma endregion
}
DWORD XamUserGetNameHook(DWORD dwUserIndex, LPSTR szUserName, DWORD cchUserName)
{
	if (dwUserIndex == 0)
	{
		strncpy(szUserName, XUIDSpoof_Gamertag, cchUserName);
		return ERROR_SUCCESS;
	}
	else
		return XamUserGetName(dwUserIndex, szUserName, cchUserName);
}
DWORD XamUserGetXUIDHook(DWORD dwUserIndex, DWORD unk, PXUID pxuid)
{
	if (dwUserIndex == 0)
	{
		*pxuid = XUIDSpoof_XUID;
		return ERROR_SUCCESS;
	}
	else
		return XamUserGetXUID(dwUserIndex, unk, pxuid);
}
DWORD XamUserGetSigninInfoHook(DWORD dwUserIndex, DWORD dwFlags, PXUSER_SIGNIN_INFO pSigninInfo)
{
	if (dwUserIndex == 0)
	{
		PXUSER_SIGNIN_INFO signinInfo;
		DWORD ret = XamUserGetSigninInfo(dwUserIndex, dwFlags, signinInfo);
		memset(signinInfo->szUserName, 0, sizeof(signinInfo->szUserName));
		strcpy(signinInfo->szUserName, XUIDSpoof_Gamertag);
		signinInfo->xuid = XUIDSpoof_XUID;
		pSigninInfo = signinInfo;
		return ret;
	}
	else
		return XamUserGetSigninInfo(dwUserIndex, dwFlags, pSigninInfo);
}