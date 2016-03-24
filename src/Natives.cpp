//// Includes
#include "stdafx.h"
#include "Globals.h"
#include "Natives.h"
#include "Tools.h"
#include <stdio.h>

//// Variables
/// GTA Vars
extern Entity _Entity;
extern Ped _Ped;
extern Player _Player;
extern Position _Position;
extern Vehicle _Vehicle;
/// Native Hook
extern Node* NativePool;
extern Node* NativePoolHead;
/// Scripting
// Grappling Hook Gun
extern bool GrapplingHook_MovingToTarget;
extern Rope GrapplingHook_Rope;
/// Virtual Machine
extern char* Request_ClipSet;

//// Methods
/// GLOBALS
PDWORD GLOBAL_GET(DWORD globalID)
{
	PDWORD* globals = (PDWORD*)0x83DDD930;
	return &globals[globalID / 0x40000][globalID % 0x40000];
}
DWORD GLOBAL_READ(DWORD globalID)
{
	PDWORD* globals = (PDWORD*)0x83DDD930;
	return globals[globalID / 0x40000][globalID % 0x40000];
}
void GLOBAL_WRITE(DWORD globalID, DWORD value)
{
	PDWORD* globals = (PDWORD*)0x83DDD930;
	globals[globalID / 0x40000][globalID % 0x40000] = value;
}
/// CUSTOM
void _APPLY_RELATIVE_FORCE(Entity entity, Position direction)
{
	APPLY_FORCE_TO_ENTITY(entity, 3, direction.x, direction.y, direction.z, 0.0f, 0.0f, 0.0f, false, true, true, true, false, true);
}
void _APPLY_RELATIVE_FORCE(Entity entity, Position direction, Position rotation)
{
	APPLY_FORCE_TO_ENTITY(entity, 3, direction.x, direction.y, direction.z, rotation.z, rotation.y, rotation.z, false, true, true, true, false, true);
}
Rope _ATTACH_ENTITIES_TO_ROPE(Rope rope, Entity ent1, Entity ent2, float x1, float y1, float z1, float x2, float y2, float z2, float distanceCoef, float forcedDistance, Position ent1Offset, Position ent2Offset)
{
	Position position = Position(x1, y1, z1);
	Position destination = Position(x2, y2, z2);
	Position offsets[] = { ent1Offset, ent2Offset };
	Entity ents[] = { ent1, ent2 };
	float distance = forcedDistance < 0.1 ? (GET_DISTANCE_BETWEEN_COORDS(x1, y1, z1, x2, y2, z2) * distanceCoef) : forcedDistance;
	for (int i = 0; i < 2; i++)
	{
		if (IS_ENTITY_A_PED(ents[i]))
		{
			position = GET_PED_BONE_COORDS(ents[i]);
			SET_PED_TO_RAGDOLL(ents[i], -1, -1, 0, false, false, false);
			SET_PED_DIES_WHEN_INJURED(ents[i], false);
			SET_PED_SUFFERS_CRITICAL_HITS(ents[i], false);
			SET_ENTITY_AS_MISSION_ENTITY(ents[i], true, true);
			SET_PED_GENERATES_DEAD_BODY_EVENTS(ents[i], false);
			if (GET_ENTITY_HEALTH(ents[i]) < 100)
			{
				REVIVE_INJURED_PED(ents[i]);
				SET_ENTITY_HEALTH(ents[i], 200);
			}
		}
	}
	if (distance < 10.0)
		distance = 10.0;
	for (int i = 0; i < 2; i++)
	{
		if (!offsets[i].isNull())
		{
			offsets[i] = (Position)(offsets[i] * 0.5);
			position = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent1, offsets[i].x, offsets[i].y, offsets[i].z);
		}
	}
	ATTACH_ENTITIES_TO_ROPE(rope, ent1, ent2, position.x, position.y, position.z, destination.x, destination.y, destination.z, distance, false, false, false, false);
	return rope;
}
void _ATTACH_ROPE_TO_ENTITY(Rope rope, Entity entity, float x, float y, float z, Position offset, Position forcedPosition)
{
	Position position = Position(x, y, z);
	if (!forcedPosition.isNull())
		position = forcedPosition;
	if (!offset.isNull())
		position += offset;
	ATTACH_ROPE_TO_ENTITY(rope, entity, x, y, z, false);
	_0xFC0DB4C3(rope);
}
void _ATTACH_TO_TARGET(Entity entity, Position forcedPosition)
{
	if (forcedPosition.isNull())
	{
		Position vec = Position(0, 0, 1.15);
		GrapplingHook_Rope = _CREATE_ROPE_FOR_PLAYER(entity, forcedPosition, vec);
		GrapplingHook_MovingToTarget = true;
	}
	else
	{
		Position vec = forcedPosition;
		Position vec2 = vec + (vec * 0.05);
		Position direction = _ROTATION_TO_DIRECTION(GET_GAMEPLAY_CAM_ROT());
		while (GET_DISTANCE_BETWEEN_COORDS(vec2.x, vec2.y, 0.0, forcedPosition.x, forcedPosition.y, 0.0) < 3.1)
		{
			Position buffer = vec2 + (Position().WorldUp() * 100.0);
			vec.z = GET_GROUND_Z_FOR_3D_COORD(buffer.x, buffer.y, buffer.z);
			if ((vec.z >= forcedPosition.z) && (vec.DistanceTo(forcedPosition) < 5.0))
			{
				forcedPosition = vec;
				break;
			}
			vec = forcedPosition;
			vec2 += (Position)(direction * 0.05);
		}
		vec += (Position().WorldUp() * 0.8);
		GrapplingHook_Rope = _CREATE_ROPE_FOR_PLAYER(entity, vec, Position(-0.05, -0.05, 1.1));
		GrapplingHook_MovingToTarget = true;
	}
}
bool _ATTACH_VEHICLE_TO_VEHICLE(Vehicle toattach, Vehicle attachto)
{
	bool control = false;
	for (int i = 0; i < 310; i++)
	{
		if (control = _REQUEST_NETWORK_CONTROL(toattach))
			break;
	}

	if (control)
	{
		SET_VEHICLE_DOORS_LOCKED(toattach, true);
		ATTACH_ENTITY_TO_ENTITY(toattach, attachto, 0, 0, -3.9, 0, 0, 0, 0, false, false, false, false, 2, true);
	}
	return control;
}
void _CALL_AIRSTRIKE(float x, float y, float z)
{
	for (int i = 0; i < 10; i++)
	{
		if (!HAS_WEAPON_ASSET_LOADED(0x13579279))
			REQUEST_WEAPON_ASSET(0x13579279, 31, false);

		if (i == 0)
			SHOOT_SINGLE_BULLET_BETWEEN_COORDS(x, y, z + 100.0f, x, y, z, 200, true, 0x13579279, _Ped, true, false, 3.212837E+09f);
		else
			SHOOT_SINGLE_BULLET_BETWEEN_COORDS(x + (float)(rand() % 5), y + (float)(rand() % 5), z + 100.0f, x, y, z, 200, true, 0x13579279, _Ped, true, false, 3.212837E+09f);
	}
}
template<typename T> T _CALL_NATIVE(DWORD dwNativeNameHash, DWORD dwParamCount, ...)
{
	DWORD NativeCall_Arguments[16];
	DWORD NativeCall_Return[3];

	NativeParams NativeCall_Struct;
	NativeCall_Struct.dwParamCount = dwParamCount;
	NativeCall_Struct.ptrParams = (DWORD)NativeCall_Arguments;
	NativeCall_Struct.ptrReturn = (DWORD)NativeCall_Return;
	
	va_list va;
	va_start(va, dwParamCount);
	for (int i = 0; i < 16; i++)
	{
		if (i < dwParamCount)
			NativeCall_Arguments[i] = va_arg(va, DWORD);
		else
			NativeCall_Arguments[i] = 0;
	}
	va_end(va);
	
	((void(*)(void*))_GET_ADDRESS(dwNativeNameHash))(&NativeCall_Struct);
	return *(T*)&NativeCall_Return[0];
}
Rope _CREATE_ROPE(float x1, float y1, float z1, float x2, float y2, float z2, float distanceCoef, float forcedDistance)
{
	Position origin = Position(x1, y1, z1);
	Position destination = Position(x2, y2, z2);
	float distance = forcedDistance < 0.1 ? (GET_DISTANCE_BETWEEN_COORDS(x1, y1, z1, x2, y2, z2) * distanceCoef) : forcedDistance;
	ROPE_LOAD_TEXTURES();
	Position angle = _DIRECTION_TO_ROTATION(Position().Normalize(destination - origin), 0.0);
	Rope rope = ADD_ROPE(x1, y1, z1, angle.x, angle.y, angle.z, distance, 4, distance);
	Sleep(10);
	ACTIVATE_PHYSICS(rope);
	return rope;
}
Rope _CREATE_ROPE_FOR_PLAYER(Entity entity, Position forcedPosition, Position ropeOffset)
{
	if (forcedPosition.isNull())
	{
		Position position[] = { GET_ENTITY_COORDS(entity), _Position };
		Rope rope = _CREATE_ROPE(position[0].x, position[0].y, position[0].z, position[1].x, position[1].y, position[1].z, 0.4, 0.0);
		Position bones = GET_PED_BONE_COORDS(_Ped, 0x6F06, false, false, false);
		bones.z += 0.01;
		_ATTACH_ROPE_TO_ENTITY(rope, _Ped, position[1].x, position[1].y, position[1].z, Position(0, 0, 0), bones);
		return rope;
	}
	Position position = _Position;
	Rope rope = _CREATE_ROPE(position.x, position.y, position.z, forcedPosition.x, forcedPosition.y, forcedPosition.z, 0.4, 0.0);
	DWORD vertexCount = GET_ROPE_VERTEX_COUNT(rope) - 1;
	Position bones = GET_PED_BONE_COORDS(_Ped, 0x6F06, false, false, false);
	bones.z += 0.01;
	_ATTACH_ROPE_TO_ENTITY(rope, _Ped, position.x, position.y, position.z, Position(0, 0, 0), bones);
	PIN_ROPE_VERTEX(rope, vertexCount, forcedPosition.x, forcedPosition.y, forcedPosition.z);
	return rope;
}
float _DEGREE_TO_RADIAN(float degree)
{
	return (degree * (float)0.017453292519943295);
}
Position _DIRECTION_TO_ROTATION(Position direction, float roll)
{
	Position vec;
	direction.Normalize();
	vec.z = _RADIAN_TO_DEGREE(atan2(direction.x, direction.y));
	Position buffer = Position(direction.x, direction.y, 0.0);
	buffer = Position(direction.z, buffer.Distance(), 0.0);
	buffer.Normalize();
	vec.x = _RADIAN_TO_DEGREE(atan2(direction.x, direction.y));
	vec.y = roll;
	return vec;
}
void _DISABLE_CONTROLS()
{
	SET_CINEMATIC_BUTTON_ACTIVE(false);
	/*SET_INPUT_EXCLUSIVE(2, 161);
	SET_INPUT_EXCLUSIVE(2, 163);
	SET_INPUT_EXCLUSIVE(2, 164);
	SET_INPUT_EXCLUSIVE(2, 165);
	SET_INPUT_EXCLUSIVE(2, 176);*/
	DISABLE_CONTROL_ACTION(0, 0);
	DISABLE_CONTROL_ACTION(0, 92);
	DISABLE_CONTROL_ACTION(0, 71);
	DISABLE_CONTROL_ACTION(0, 75);
	SET_INPUT_EXCLUSIVE(2, 179);
	SET_INPUT_EXCLUSIVE(2, 177);
	SET_INPUT_EXCLUSIVE(2, 178);
	DISABLE_CONTROL_ACTION(0, 48);
	SET_INPUT_EXCLUSIVE(2, 164);
	SET_INPUT_EXCLUSIVE(2, 165);
	DISABLE_CONTROL_ACTION(2, 177);
	DISABLE_CONTROL_ACTION(2, 178);
	DISABLE_CONTROL_ACTION(2, 166);
	DISABLE_CONTROL_ACTION(2, 167);
	DISABLE_CONTROL_ACTION(2, 164);
	DISABLE_CONTROL_ACTION(2, 165);
	HIDE_HUD_COMPONENT_THIS_FRAME(10);
	HIDE_HUD_COMPONENT_THIS_FRAME(6);
	HIDE_HUD_COMPONENT_THIS_FRAME(7);
	HIDE_HUD_COMPONENT_THIS_FRAME(9);
	HIDE_HUD_COMPONENT_THIS_FRAME(8);
	SET_INPUT_EXCLUSIVE(2, 166);
	SET_INPUT_EXCLUSIVE(2, 167);
	DISABLE_CONTROL_ACTION(0, 37);
	DISABLE_CONTROL_ACTION(0, 139);
	DISABLE_CONTROL_ACTION(0, 140);
	DISABLE_CONTROL_ACTION(0, 141);
	DISABLE_CONTROL_ACTION(0, 142);
	DISABLE_CONTROL_ACTION(0, 143);
	DISABLE_CONTROL_ACTION(0, 144);
	DISABLE_CONTROL_ACTION(0, 145);
	DISABLE_CONTROL_ACTION(0, 146);
	DISABLE_CONTROL_ACTION(0, 147);
	DISABLE_CONTROL_ACTION(0, 14);
	DISABLE_CONTROL_ACTION(0, 15);
	DISABLE_CONTROL_ACTION(0, 52);
	DISABLE_CONTROL_ACTION(0, 53);
	DISABLE_CONTROL_ACTION(0, 123);
	DISABLE_CONTROL_ACTION(0, 124);
	DISABLE_CONTROL_ACTION(0, 126);
	DISABLE_CONTROL_ACTION(0, 47);
	DISABLE_CONTROL_ACTION(0, 69);
	DISABLE_CONTROL_ACTION(0, 78);
	DISABLE_CONTROL_ACTION(0, 51);
	DISABLE_CONTROL_ACTION(0, 45);
	DISABLE_CONTROL_ACTION(0, 54);
	DISABLE_CONTROL_ACTION(0, 75);
	DISABLE_CONTROL_ACTION(0, 22);
	DISABLE_CONTROL_ACTION(0, 92);
	DISABLE_CONTROL_ACTION(0, 106);
	DISABLE_CONTROL_ACTION(0, 149);
	DISABLE_CONTROL_ACTION(0, 148);
	DISABLE_CONTROL_ACTION(0, 150);
	DISABLE_CONTROL_ACTION(0, 151);
	DISABLE_CONTROL_ACTION(0, 19);
}
void _DISPLAY_TEXT(char* text, DWORD font, float x, float y, float width, float height, BYTE R, BYTE G, BYTE B, BYTE A, bool outline, bool center, float wrapWidth, float wrapHeight)
{
	SET_TEXT_FONT(font);
	SET_TEXT_SCALE(width, height);
	SET_TEXT_COLOUR(R, G, B, A);
	if (outline) SET_TEXT_OUTLINE();
	SET_TEXT_WRAP(wrapWidth, wrapHeight);
	SET_TEXT_CENTRE(center);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	SET_TEXT_EDGE(0, 0, 0, 0, 0);
	_0x3E35563E("STRING");
	//_0x94B82066("STRING");
	_0x27A244D8(text);
	_0x6F8350CE(x, y);
	//_0xAA318785(x, y);
}
void _DROP_CASH(Position coords, DWORD amount)
{
	DWORD buffer = amount;
	while (buffer > 40000)
	{
		CREATE_AMBIENT_PICKUP(0xCE6FDD6B, coords.x, coords.y, coords.z, false, 40000, 0x113FD533, false, true);
		buffer -= 40000;
	}
	if (buffer > 0)
		CREATE_AMBIENT_PICKUP(0xCE6FDD6B, coords.x, coords.y, coords.z, false, buffer, 0x113FD533, false, true);
}
void _DROP_HEALTH(Position coords)
{
	CREATE_AMBIENT_PICKUP(0x8F707C18, coords.x, coords.y, coords.z, false, 100000, 1, false, true);
}
DWORD _GET_ADDRESS(DWORD native)
{
	Node* n = NativePoolHead;
	while (n != NULL)
	{
		if (n->hash == native)
			return n->addr;
		n = n->getNext();
	}
	return 0;
}
Position _GET_CAMERA_DIRECTION()
{
	Position rot = GET_GAMEPLAY_CAM_ROT();
	float tX = rot.x * 0.0174532924;
	float tZ = rot.z * 0.0174532924;
	float num = abs(cos(tX));
	return Position(((-sin(tZ)) * num), (cos(tZ) * num), sin(tX));
}
Position _GET_CAMERA_DIRECTION(float distance)
{
	Position rot = GET_GAMEPLAY_CAM_ROT();
	float tX = rot.x * 0.0174532924;
	float tZ = rot.z * 0.0174532924;
	float num = abs(cos(tX));
	return Position(((-sin(tZ)) * (num + distance)), (cos(tZ) * (num + distance)), (sin(tX) * 8));
}
float _GET_FPS()
{
	return (1.0 / GET_FRAME_TIME());
}
std::vector<Ped> _GET_NEARBY_PEDS(float radius, DWORD maxAmount)
{
	Position position = _Position;
	std::vector<Ped> result;
	PDWORD handles = new DWORD[maxAmount * 2 + 2];

	handles[0] = maxAmount;
	DWORD amount = GET_PED_NEARBY_PEDS(_Ped, handles, -1);
	for (int i = 0; i < amount; i++)
	{
		DWORD index = i * 2 + 2;
		if (handles[index] != 0 && DOES_ENTITY_EXIST(handles[index]))
		{
			Ped ped = handles[index];
			if (position.Distance(GET_ENTITY_COORDS(ped)) < radius)
				result.push_back(ped);
		}
	}
	delete[] handles;

	return result;
}
void _PLAY_ANIMATION(Entity entity, char* animSet, char* animName, float speed, DWORD duration, bool lastAnimation, float playbackRate)
{
	while (!_REQUEST_ANIM_DICT(animSet)) {}
	TASK_PLAY_ANIM(entity, animSet, animName, speed, -8.0, duration, lastAnimation, playbackRate, false, false, false);
}
void _PRINT_HELP_TEXT(char* text, ...)
{
	char Buffer[0x400];
	va_list VA_ARGS;
	va_start(VA_ARGS, text);
	vsprintf(Buffer, text, VA_ARGS);

	SET_TEXT_COMPONENT_FORMAT("STRING");
	_0x27A244D8(Buffer);
	DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, false, false, -1);
}
void _PRINT_TEXT(char* text, ...)
{
	CLEAR_PRINTS();

	char Buffer[0x400];
	va_list VA_ARGS;
	va_start(VA_ARGS, text);
	vsprintf(Buffer, text, VA_ARGS);

	_0xF42C43C7("STRING");
	_0x27A244D8(Buffer);
	_0x38F82261(2000);
}
float _RADIAN_TO_DEGREE(float radian)
{
	return (radian * (float)57.295779513082323);
}
bool _REQUEST_ANIM_DICT(char* dict)
{
	if (!HAS_ANIM_DICT_LOADED(dict))
		REQUEST_ANIM_DICT(dict);
	else
		return true;
	return HAS_ANIM_DICT_LOADED(dict);
}
bool _REQUEST_CLIP_SET(char* clipSet)
{
	if (!HAS_CLIP_SET_LOADED(clipSet))
		REQUEST_CLIP_SET(clipSet);
	else
		return true;
	return HAS_CLIP_SET_LOADED(clipSet);
}
bool _REQUEST_NETWORK_CONTROL(Entity entity)
{
	NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
	return NETWORK_HAS_CONTROL_OF_ENTITY(entity);
}
Position _ROTATION_TO_DIRECTION(Position rot)
{
	Position direction;
	float a = _DEGREE_TO_RADIAN(rot.z);
	float d = _DEGREE_TO_RADIAN(rot.x);
	float num = (float)abs(cos(d));
	direction.x = (float)(-sin(a) * num);
	direction.y = (float)(cos(a) * num);
	direction.z = (float)sin(d);
	return direction;
}
Vehicle _SPAWN_VEHICLE(DWORD modelHash)
{
	return _SPAWN_VEHICLE(modelHash, _Position.x, _Position.y, _Position.z);
}
Vehicle _SPAWN_VEHICLE(DWORD modelHash, float x, float y, float z)
{
	REQUEST_MODEL(modelHash);

	float speed;
	if (_Vehicle != 0)
	{
		speed = GET_ENTITY_SPEED(_Vehicle);
		SET_ENTITY_AS_MISSION_ENTITY(_Vehicle, false, true);
		DELETE_VEHICLE(_Vehicle);
	}
	else
		speed = GET_ENTITY_SPEED(_Ped);

	if (HAS_MODEL_LOADED(modelHash))
	{
		CLEAR_AREA_OF_VEHICLES(x, y, z, 5.0f);
		Vehicle vehicle = CREATE_VEHICLE(modelHash, x, y, z, GET_ENTITY_HEADING(_Ped), true, false);
		if (DOES_ENTITY_EXIST(vehicle))
		{
			SET_PED_INTO_VEHICLE(_Ped, vehicle, -1);
			SET_VEHICLE_ENGINE_ON(vehicle, true, true);
			SET_HELI_BLADES_FULL_SPEED(vehicle);
			SET_VEHICLE_FORWARD_SPEED(vehicle, speed);
			SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
			return vehicle;
		}
	}

	if (CAN_REGISTER_MISSION_VEHICLES())
	{
		CLEAR_AREA_OF_VEHICLES(x, y, z, 1.0);
		Vehicle vehicle = CREATE_VEHICLE(modelHash, x, y, z, GET_ENTITY_HEADING(_Ped));
		NetworkID networkId = VEH_TO_NET(vehicle);
		if (NETWORK_DOES_NETWORK_ID_EXIST(networkId))
		{
			_0xD3850671(vehicle);
			if (_0xD7F934F4(vehicle))
				SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
			SET_VEHICLE_IS_STOLEN(vehicle, false);
			vehicle = NET_TO_VEH(networkId);
			DOES_ENTITY_EXIST(vehicle);
			if (!IS_ENTITY_A_MISSION_ENTITY(vehicle) && NETWORK_GET_ENTITY_IS_LOCAL(vehicle))
				SET_ENTITY_AS_MISSION_ENTITY(vehicle, false, true);
			SET_VEHICLE_FIXED(vehicle);
			SET_VEHICLE_ENGINE_ON(vehicle, true, true);
			SET_VEHICLE_LIGHTS(vehicle, 3);
			if (IS_THIS_MODEL_A_PLANE(modelHash) || IS_THIS_MODEL_A_HELI(modelHash))
				SET_HELI_BLADES_FULL_SPEED(modelHash);
			Team team = GET_PLAYER_TEAM(_Player);
			if (team > -1)
				SET_VEHICLE_DOORS_LOCKED_FOR_TEAM(vehicle, team, false);
			if (!IS_PED_IN_VEHICLE(_Ped, vehicle, false))
				SET_PED_INTO_VEHICLE(_Ped, vehicle);
			SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);
		}
	}
	return 0;
}
void _TELEPORT(float x, float y, float z)
{
	SET_ENTITY_COORDS(IS_PED_IN_ANY_VEHICLE(_Ped) ? _Vehicle : _Ped, x, y, z);
	LOAD_ALL_OBJECTS_NOW();
}
void _TELEPORT(Position position)
{
	_TELEPORT(position.x, position.y, position.z);
}
/// PLAYER
Ped GET_PLAYER_PED(Player player)
{
	return _CALL_NATIVE<Ped>(0x6E31E993, 1, player);
}
void SET_PLAYER_MODEL(Player player, DWORD modelHash)
{
	_CALL_NATIVE<DWORD>(0x774A4C54, 2, player, modelHash);
}
void CHANGE_PLAYER_PED(Player player, Ped ped, bool unk1, bool unk2)
{
	_CALL_NATIVE<DWORD>(0xBE515485, 4, player, ped, unk1, unk2);
}
Team GET_PLAYER_TEAM(Player player)
{
	return _CALL_NATIVE<Team>(0x9873E404, 1, player);
}
void SET_PLAYER_TEAM(Player player, Team team)
{
	_CALL_NATIVE<DWORD>(0x725ADCF2, 2, player, team);
}
char* GET_PLAYER_NAME(Player player)
{
	return _CALL_NATIVE<char*>(0x406B4B20, 1, player);
}
void CLEAR_PLAYER_WANTED_LEVEL(Player player)
{
	_CALL_NATIVE<DWORD>(0x54EA5BCC, 1, player);
}
bool IS_PLAYER_DEAD(Player player)
{
	return _CALL_NATIVE<BOOL>(0x140CA5A8, 1, player);
}
bool IS_PLAYER_PRESSING_HORN(Player player)
{
	return _CALL_NATIVE<BOOL>(0xED1D1662, 1, player);
}
void SET_MAX_WANTED_LEVEL(DWORD wantedLevel)
{
	_CALL_NATIVE<DWORD>(0x665A06F5, 1, wantedLevel);
}
DWORD GET_MAX_WANTED_LEVEL()
{
	return _CALL_NATIVE<DWORD>(0xBDCDD163, 0);
}
Prop GET_AIMED_ENTITY(Player player)
{
	DWORD entity;
	_CALL_NATIVE<DWORD>(0x8866D9D0, 2, player, &entity);
	return entity;
}
Player PLAYER_ID()
{
	return _CALL_NATIVE<Player>(0x8AEA886C, 0);
}
Ped PLAYER_PED_ID()
{
	return _CALL_NATIVE<Ped>(0xFA92E226, 0);
}
void SET_PLAYER_INVINCIBLE(Player player, bool invincible)
{
	_CALL_NATIVE<DWORD>(0xDFB9A2A2, 2, player, invincible);
}
bool GET_PLAYER_INVINCIBLE(Player player)
{
	return _CALL_NATIVE<BOOL>(0x680C90EE, 1, player);
}
void SET_SWIM_SPEED_MULTIPLIER(Player player, float multiplier)
{
	_CALL_NATIVE<DWORD>(0xB986FF47, 2, player, *(DWORD*)&multiplier);
}
void SET_MOVE_SPEED_MULTIPLIER(Player player, float multiplier)
{
	_CALL_NATIVE<DWORD>(0x825423C2, 2, player, *(DWORD*)&multiplier);
}
/// ENTITY
bool DOES_ENTITY_EXIST(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x3AC90869, 1, entity);
}
bool HAS_ENTITY_CLEAR_LOS_TO_ENTITY(Entity ent1, Entity ent2, DWORD trace)
{
	return _CALL_NATIVE<BOOL>(0x53576FA7, 3, ent1, ent2, trace);
}
Entity GET_ENTITY_ATTACHED_TO(Entity entity)
{
	return _CALL_NATIVE<Entity>(0xFE1589F9, 1, entity);
}
Position GET_ENTITY_COORDS(Entity entity, bool alive)
{
	return _CALL_NATIVE<Position>(0x1647F1CB, 2, entity, alive);
}
float GET_ENTITY_HEADING(Entity entity)
{
	return _CALL_NATIVE<float>(0x972CC383, 1, entity);
}
DWORD GET_ENTITY_HEALTH(Entity entity)
{
	return _CALL_NATIVE<DWORD>(0x8E3222B7, 1, entity);
}
Position GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Entity entity, float x, float y, float z)
{
	return _CALL_NATIVE<Position>(0xABCF043A, 4, entity, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z);
}
float GET_ENTITY_PITCH(Entity entity)
{
	return _CALL_NATIVE<float>(0xFCE6ECE5, 1, entity);
}
float GET_ENTITY_ROLL(Entity entity)
{
	return _CALL_NATIVE<float>(0x36610842, 1, entity);
}
Position GET_ENTITY_ROTATION(Entity entity, DWORD unk1)
{
	return _CALL_NATIVE<Position>(0x8FF45B04, 2, entity, unk1);
}
DWORD GET_ENTITY_MODEL(Entity entity)
{
	return _CALL_NATIVE<DWORD>(0xDAFCB3EC, 1, entity);
}
float GET_ENTITY_SPEED(Entity entity)
{
	return _CALL_NATIVE<float>(0x9E1E4798, 1, entity);
}
bool IS_ENTITY_A_PED(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x55D33EAB, 1, entity);
}
bool IS_ENTITY_A_MISSION_ENTITY(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x2632E124, 1, entity);
}
bool IS_ENTITY_DEAD(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0xB6F7CBAC, 1, entity);
}
bool IS_ENTITY_IN_WATER(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x4C3C2508, 1, entity);
}
bool IS_ENTITY_VISIBLE(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x120B4ED5, 1, entity);
}
void APPLY_FORCE_TO_ENTITY(Entity entity, DWORD forceType, float x, float y, float z, float xRot, float yRot, float zRot, bool unk1, bool isRel, bool ignoreUpVec, bool unk2, bool unk3, bool unk4)
{
	_CALL_NATIVE<DWORD>(0xC1C0855A, 14, entity, forceType, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&xRot, *(DWORD*)&yRot, *(DWORD*)&zRot, unk1, isRel, ignoreUpVec, unk2, unk3, unk4);
}
void ATTACH_ENTITY_TO_ENTITY(Entity ent1, Entity ent2, Prop bone, float x, float y, float z, float xRot, float yRot, float zRot, bool unk1, bool isRel, bool ignoreUpVec, bool allowRot, DWORD unk2, bool unk3)
{
	_CALL_NATIVE<DWORD>(0xEC024237, 15, ent1, ent2, bone, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&xRot, *(DWORD*)&yRot, *(DWORD*)&zRot, unk1, isRel, ignoreUpVec, allowRot, unk2, unk3);
}
void DETACH_ENTITY(Entity entity, bool unk1, bool unk2)
{
	_CALL_NATIVE<DWORD>(0xC8EFCB41, 3, entity, unk1, unk2);
}
void FREEZE_ENTITY_POSITION(Entity entity, bool freeze)
{
	_CALL_NATIVE<DWORD>(0x65C16D57, 2, entity, freeze);
}
void _0xD3850671(Entity entity, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0xD3850671, 2, entity, unk1);
}
void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, bool missionEntity, bool unk1)
{
	_CALL_NATIVE<DWORD>(0x5D1F9E0F, 3, entity, missionEntity, unk1);
}
void SET_ENTITY_COLLISION(Entity entity, bool collide)
{
	_CALL_NATIVE<DWORD>(0x139FD37D, 2, entity, collide);
}
void SET_ENTITY_COORDS(Entity entity, float x, float y, float z, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4)
{
	_CALL_NATIVE<DWORD>(0xDF70B41B, 8, entity, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, unk1, unk2, unk3, unk4);
}
void SET_ENTITY_HEADING(Entity entity, float heading)
{
	_CALL_NATIVE<DWORD>(0xE0FF064D, 2, entity, *(DWORD*)&heading);
}
void SET_ENTITY_HEALTH(Entity entity, DWORD health)
{
	_CALL_NATIVE<DWORD>(0xFBCD1831, 2, entity, health);
}
void SET_ENTITY_PROOFS(Entity entity, bool bulletProof, bool fireProof, bool explosionProof, bool collisionProof, bool meleeProof, bool unk1, bool unk2, bool drownProof)
{
	_CALL_NATIVE<DWORD>(0x7E9EAB66, 9, entity, bulletProof, fireProof, explosionProof, collisionProof, meleeProof, unk1, unk2, drownProof);
}
void SET_ENTITY_ROTATION(Entity entity, float pitch, float roll, float yaw, DWORD unk1, bool unk2)
{
	_CALL_NATIVE<DWORD>(0x0A345EFE, 6, entity, *(DWORD*)&pitch, *(DWORD*)&roll, *(DWORD*)&yaw, unk1, unk2);
}
void SET_ENTITY_VISIBLE(Entity entity, bool visible)
{
	_CALL_NATIVE<DWORD>(0xD043E8E1, 2, entity, visible);
}
/// PED
Ped CREATE_PED(DWORD type, DWORD modelHash, float x, float y, float z, bool unk1, bool networkPed, bool createHandle)
{
	return _CALL_NATIVE<Ped>(0x0389EF71, 8, type, modelHash, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, unk1, networkPed, createHandle);
}
void DELETE_PED(Ped ped)
{
	_CALL_NATIVE<DWORD>(0x13EFB9A0, 1, ped);
}
bool IS_PED_IN_VEHICLE(Ped ped, Vehicle vehicle, bool atGetIn)
{
	return _CALL_NATIVE<BOOL>(0x7DA6BC83, 3, ped, vehicle, atGetIn);
}
bool IS_PED_IN_ANY_VEHICLE(Ped ped, DWORD unk1)
{
	return _CALL_NATIVE<BOOL>(0x3B0171EE, 2, ped, unk1);
}
bool IS_PED_DEAD(Ped ped, DWORD unk1)
{
	return _CALL_NATIVE<BOOL>(0xCBDB7739, 2, ped, unk1);
}
bool IS_PED_A_PLAYER(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x404794CA, 1, ped);
}
bool IS_PED_SHOOTING(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0xE7C3405E, 1, ped);
}
void SET_PED_INTO_VEHICLE(Ped ped, Vehicle vehicle, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0x07500C79, 3, ped, vehicle, unk1);
}
bool IS_PED_HUMAN(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x194BB7B0, 1, ped);
}
Vehicle GET_VEHICLE_PED_IS_IN(Ped ped, DWORD unk1)
{
	return _CALL_NATIVE<Vehicle>(0xAFE92319, 2, ped, unk1);
}
void SET_PED_SUFFERS_CRITICAL_HITS(Ped ped, bool suffersCriticalHits)
{
	_CALL_NATIVE<DWORD>(0x6F6FC7E6, 2, ped, suffersCriticalHits);
}
bool IS_PED_ON_FOOT(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0xC60D0785, 1, ped);
}
bool IS_PED_ON_ANY_BIKE(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x4D885B2E, 1, ped);
}
bool IS_PED_IN_ANY_BOAT(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x1118A947, 1, ped);
}
bool IS_PED_IN_ANY_SUB(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0xE65F8059, 1, ped);
}
bool IS_PED_IN_ANY_HELI(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x7AB5523B, 1, ped);
}
bool IS_PED_IN_ANY_PLANE(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x51BBCE7E, 1, ped);
}
void SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Ped ped, DWORD state)
{
	_CALL_NATIVE<DWORD>(0x8A251612, 2, ped, state);
}
void KNOCK_PED_OFF_VEHICLE(Ped ped)
{
	_CALL_NATIVE<DWORD>(0xACDD0674, 1, ped);
}
void SET_PED_KEEP_TASK(Ped ped, bool keepTask)
{
	_CALL_NATIVE<DWORD>(0xA7EC79CE, 2, ped, keepTask);
}
void SET_PED_AS_ENEMY(Ped ped, bool enemy)
{
	_CALL_NATIVE<DWORD>(0xAE620A1B, 2, ped, enemy);
}
bool IS_PED_GETTING_INTO_A_VEHICLE(Ped ped)
{
	return _CALL_NATIVE<BOOL>(0x90E805AC, 1, ped);
}
void SET_PED_MOVEMENT_CLIPSET(Ped ped, char* clipSet, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0xA817CDEB, 3, ped, clipSet, unk1);
}
void SET_PED_RANDOM_COMPONENT_VARIATION(Ped ped, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0x4111BA46, 2, ped, unk1);
}
void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Ped ped, bool block)
{
	_CALL_NATIVE<DWORD>(0xDFE34E4A, 2, ped, block);
}
void SET_PED_TO_RAGDOLL(Ped ped, DWORD time1, DWORD time2, DWORD type, bool unk1, bool unk2, bool unk3)
{
	_CALL_NATIVE<DWORD>(0x83CB5052, 7, ped, time1, time2, type, unk1, unk2, unk3);
}
void SET_PED_CAN_RAGDOLL(Ped ped, bool canRagdoll)
{
	_CALL_NATIVE<DWORD>(0xCF1384C4, 2, ped, canRagdoll);
}
void REVIVE_INJURED_PED(Ped ped)
{
	_CALL_NATIVE<DWORD>(0x14D3E6E3, 1, ped);
}
void SET_PED_DIES_WHEN_INJURED(Ped ped, bool diesWhenInjured)
{
	_CALL_NATIVE<DWORD>(0xE94E24D4, 2, ped, diesWhenInjured);
}
Position GET_PED_BONE_COORDS(Ped ped, Prop bone, float unk2, float unk3, float unk4)
{
	return _CALL_NATIVE<Position>(0x4579CAB1, 5, ped, bone, unk2, unk3, unk4);
}
void APPLY_PED_DAMAGE_PACK(Ped ped, char* damagePack, float unk1, float unk2)
{
	_CALL_NATIVE<DWORD>(0x208D0CB8, 4, ped, damagePack, *(DWORD*)&unk1, *(DWORD*)&unk2);
}
void SET_PED_GENERATES_DEAD_BODY_EVENTS(Ped ped, bool generatesDeadBodyEvents)
{
	_CALL_NATIVE<DWORD>(0xE9B97A2B, 2, ped, generatesDeadBodyEvents);
}
void SET_PED_COMBAT_ATTRIBUTES(Ped ped, DWORD attribute, bool unk1)
{
	_CALL_NATIVE<DWORD>(0x81D64248, 3, ped, attribute, unk1);
}
void SET_PED_FLEE_ATTRIBUTES(Ped ped, DWORD attribute, bool unk1)
{
	_CALL_NATIVE<DWORD>(0xA717A875, 3, ped, attribute, unk1);
}
DWORD GET_PED_NEARBY_PEDS(Ped ped, DWORD* sizeAndPeds, DWORD unk1)
{
	return _CALL_NATIVE<DWORD>(0x4D3325F4, 3, ped, sizeAndPeds, unk1);
}
/// VEHICLE
Vehicle CREATE_VEHICLE(DWORD modelHash, float x, float y, float z, float heading, bool networkVehicle, bool createHandle)
{
	return _CALL_NATIVE<Vehicle>(0xDD75460A, 7, modelHash, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&heading, networkVehicle, createHandle);
}
void DELETE_VEHICLE(Vehicle vehicle)
{
	_CALL_NATIVE<DWORD>(0x9803AF60, 1, &vehicle);
}
void SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle vehicle)
{
	_CALL_NATIVE<DWORD>(0xE14FDBA6, 1, vehicle);
}
void SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float densityMultiplier)
{
	_CALL_NATIVE<DWORD>(0xF4187E51, 1, *(DWORD*)&densityMultiplier);
}
void SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float densityMultiplier)
{
	_CALL_NATIVE<DWORD>(0x543F712B, 1, *(DWORD*)&densityMultiplier);
}
void SET_VEHICLE_DOORS_LOCKED(Vehicle vehicle, bool doorsLocked)
{
	_CALL_NATIVE<DWORD>(0x4CDD35D0, 2, vehicle, doorsLocked);
}
void SET_VEHICLE_DOORS_LOCKED_FOR_TEAM(Vehicle vehicle, Team team, bool lockedForTeam)
{
	_CALL_NATIVE<DWORD>(0x4F85E783, 3, vehicle, team, lockedForTeam);
}
void SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle, BYTE r, BYTE g, BYTE b)
{
	_CALL_NATIVE<DWORD>(0x8DF9F9BC, 4, vehicle, r, g, b);
}
void SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle, BYTE r, BYTE g, BYTE b)
{
	_CALL_NATIVE<DWORD>(0x9D77259E, 4, vehicle, r, g, b);
}
Ped GET_PED_IN_VEHICLE_SEAT(Vehicle vehicle, DWORD seatIndex)
{
	return _CALL_NATIVE<Ped>(0x388FDE9A, 2, vehicle, seatIndex);
}
void SET_VEHICLE_FORWARD_SPEED(Vehicle vehicle, float forwardSpeed)
{
	_CALL_NATIVE<DWORD>(0x69880D14, 2, vehicle, *(DWORD*)&forwardSpeed);
}
void SET_VEHICLE_TYRES_CAN_BURST(Vehicle vehicle, bool tyresCanBurst)
{
	_CALL_NATIVE<DWORD>(0xA198DB54, 2, vehicle, tyresCanBurst);
}
void SET_VEHICLE_DOOR_OPEN(Vehicle vehicle, DWORD doorIndex, bool loose, bool openInstantly)
{
	_CALL_NATIVE<DWORD>(0xBB75D38B, 4, vehicle, doorIndex, loose, openInstantly);
}
void SET_VEHICLE_LIGHTS(Vehicle vehicle, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0xE8930226, 2, vehicle, unk1);
}
void SET_VEHICLE_DIRT_LEVEL(Vehicle vehicle, float dirtLevel)
{
	_CALL_NATIVE<DWORD>(0x2B39128B, 2, vehicle, *(DWORD*)&dirtLevel);
}
void SET_VEHICLE_ENGINE_ON(Vehicle vehicle, bool toggle, bool instantly)
{
	_CALL_NATIVE<DWORD>(0x7FBC86F1, 3, vehicle, toggle, instantly);
}
Vehicle GET_CLOSEST_VEHICLE(float x, float y, float z, float radius, DWORD modelHash, DWORD flags)
{
	return _CALL_NATIVE<Vehicle>(0xD7E26B2C, 6, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&radius, modelHash, flags);
}
bool IS_THIS_MODEL_A_PLANE(DWORD modelHash)
{
	return _CALL_NATIVE<BOOL>(0x3B3907BB, 1, modelHash);
}
bool IS_THIS_MODEL_A_HELI(DWORD modelHash)
{
	return _CALL_NATIVE<BOOL>(0x8AF7F568, 1, modelHash);
}
void SET_HELI_BLADES_FULL_SPEED(Vehicle vehicle)
{
	_CALL_NATIVE<DWORD>(0x033A9408, 1, vehicle);
}
void SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehicle vehicle, bool canBeVisiblyDamaged)
{
	_CALL_NATIVE<DWORD>(0xC5D94017, 2, vehicle, canBeVisiblyDamaged);
}
bool IS_VEHICLE_ON_ALL_WHEELS(Vehicle vehicle)
{
	return _CALL_NATIVE<BOOL>(0x10089F8E, 1, vehicle);
}
void SET_VEHICLE_FIXED(Vehicle vehicle)
{
	_CALL_NATIVE<DWORD>(0x17469AA1, 1, vehicle);
}
void SET_VEHICLE_DEFORMATION_FIXED(Vehicle vehicle)
{
	_CALL_NATIVE<DWORD>(0xDD2920C8, 1, vehicle);
}
void SET_VEHICLE_ENGINE_HEALTH(Vehicle vehicle, float engineHealth)
{
	_CALL_NATIVE<DWORD>(0x1B760FB5, 2, vehicle, *(DWORD*)&engineHealth);
}
void SET_VEHICLE_BOOST_ACTIVE(Vehicle vehicle, bool boostActive)
{
	_CALL_NATIVE<DWORD>(0x072F15F2, 2, vehicle, boostActive);
}
void SET_VEHICLE_REDUCE_GRIP(Vehicle vehicle, bool reduceGrip)
{
	_CALL_NATIVE<DWORD>(0x90D3A0D9, 2, vehicle, reduceGrip);
}
void SET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle vehicle, BYTE r, BYTE g, BYTE b)
{
	_CALL_NATIVE<DWORD>(0x3EDEC0DB, 4, vehicle, r, g, b);
}
void SET_VEHICLE_IS_STOLEN(Vehicle vehicle, bool isStolen)
{
	_CALL_NATIVE<DWORD>(0x70912E42, 2, vehicle, isStolen);
}
/// OBJECT
Prop CREATE_OBJECT(DWORD modelHash, float x, float y, float z, bool networkObject, bool createHandle, bool dynamic)
{
	return _CALL_NATIVE<Prop>(0x2F7AA05C, 7, modelHash, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, networkObject, createHandle, dynamic);
}
void DELETE_OBJECT(Prop object)
{
	_CALL_NATIVE<DWORD>(0x65C16D57, 1, &object);
}
void CREATE_AMBIENT_PICKUP(DWORD modelHash, float x, float y, float z, bool unk1, DWORD value, DWORD unk2, bool unk3, bool unk4)
{
	_CALL_NATIVE<DWORD>(0x17B99CE7, 9, modelHash, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, unk1, value, unk2, unk3, unk4);
}
/// AI
void TASK_PARACHUTE(Ped ped, bool unk1)
{
	_CALL_NATIVE<DWORD>(0xEC3060A2, 2, ped, unk1);
}
void TASK_PARACHUTE_TO_TARGET(Ped ped, float x, float y, float z)
{
	_CALL_NATIVE<DWORD>(0xE0104D6C, 4, ped, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z);
}
void TASK_FOLLOW_NAV_MESH_TO_COORD(Ped ped, float x, float y, float z, float speed, DWORD timeout, float unk1, DWORD unk2, float unk3)
{
	_CALL_NATIVE<DWORD>(0xFE4A10D9, 9, ped, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&speed, timeout, *(DWORD*)&unk1, unk2, *(DWORD*)&unk3);
}
void TASK_PLAY_ANIM(Ped ped, char* animDict, char* animName, float speed, float speedMultiplier, DWORD duration, DWORD flag, float playbackRate, bool lockX, bool lockY, bool lockZ)
{
	_CALL_NATIVE<DWORD>(0x5AB552C6, 11, ped, animDict, animName, *(DWORD*)&speed, *(DWORD*)&speedMultiplier, duration, flag, *(DWORD*)&playbackRate, lockX, lockY, lockZ);
}
void CLEAR_PED_TASKS(Ped ped)
{
	_CALL_NATIVE<DWORD>(0xDE3316AB, 1, ped);
}
void CLEAR_PED_TASKS_IMMEDIATELY(Ped ped)
{
	_CALL_NATIVE<DWORD>(0xBC045625, 1, ped);
}
void TASK_ARREST_PED(Ped ped, Ped target)
{
	_CALL_NATIVE<DWORD>(0xBC0F153D, 2, ped, target);
}
/// GAMEPLAY
void SET_TIME_SCALE(float timescale)
{
	_CALL_NATIVE<DWORD>(0xA7F84694, 1, *(DWORD*)&timescale);
}
void SET_OVERRIDE_WEATHER(char* weather)
{
	_CALL_NATIVE<DWORD>(0xD9082BB5, 1, weather);
}
float GET_FRAME_TIME()
{
	return _CALL_NATIVE<float>(0x96374262, 0);
}
DWORD GET_RANDOM_INT_IN_RANGE(DWORD start, DWORD end)
{
	return _CALL_NATIVE<DWORD>(0x4051115B, 2, start, end);
}
float GET_GROUND_Z_FOR_3D_COORD(float x, float y, float z)
{
	float ret;
	_CALL_NATIVE<DWORD>(0xA1BFD5E0, 4, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, &ret);
	return ret;
}
float GET_DISTANCE_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, bool unk1)
{
	return _CALL_NATIVE<float>(0xF698765E, 7, *(DWORD*)&x1, *(DWORD*)&y1, *(DWORD*)&z1, *(DWORD*)&x2, *(DWORD*)&y2, *(DWORD*)&z2, unk1);
}
DWORD GET_HASH_KEY(char* key)
{
	size_t len = strlen(key);
	DWORD hash, i;
	for (hash = i = 0; i < len; ++i)
	{
		hash += tolower(key[i]);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}
void CLEAR_AREA_OF_VEHICLES(float x, float y, float z, float radius, bool unk1, bool unk2, bool unk3, bool unk4, bool unk5)
{
	_CALL_NATIVE<DWORD>(0x63320F3C, 9, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&radius, unk1, unk2, unk3, unk4, unk5);
}
void SHOOT_SINGLE_BULLET_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, DWORD damage, bool unk1, Weapon weapon, Ped ped, bool audible, bool unk2, float speed)
{
	_CALL_NATIVE<DWORD>(0xCB7415AC, 13, *(DWORD*)&x1, *(DWORD*)&y1, *(DWORD*)&z1, *(DWORD*)&x2, *(DWORD*)&y2, *(DWORD*)&z2, damage, unk1, weapon, ped, audible, unk2, *(DWORD*)&speed);
}
void SET_GRAVITY_LEVEL(float gravityLevel)
{
	_CALL_NATIVE<DWORD>(0x2D833F4A, 1, *(DWORD*)&gravityLevel);
}
void SET_EXPLOSIVE_AMMO_THIS_FRAME(Player player)
{
	_CALL_NATIVE<DWORD>(0x2EAFA1D1, 1, player);
}
void SET_FIRE_AMMO_THIS_FRAME(Player player)
{
	_CALL_NATIVE<DWORD>(0x7C18FC8A, 1, player);
}
void SET_EXPLOSIVE_MELEE_THIS_FRAME(Player player)
{
	_CALL_NATIVE<DWORD>(0x96663D56, 1, player);
}
void SET_SUPER_JUMP_THIS_FRAME(Player player)
{
	_CALL_NATIVE<DWORD>(0x86745EF3, 1, player);
}
/// AUDIO
void STOP_PED_SPEAKING(Ped ped, bool speaking)
{
	_CALL_NATIVE<DWORD>(0xFF92B49D, 2, ped, speaking);
}
void DISABLE_PED_PAIN_AUDIO(Ped ped, bool disable)
{
	_CALL_NATIVE<DWORD>(0x3B8E2D5F, 2, ped, disable);
}
/// CAM
void RENDER_SCRIPT_CAMS(bool render, bool unk1, DWORD unk2, bool unk3, bool unk4)
{
	_CALL_NATIVE<DWORD>(0x74337969, 5, render, unk1, unk2, unk3, unk4);
}
Camera CREATE_CAM(char* cameraName, bool unk1)
{
	return _CALL_NATIVE<Camera>(0xE9BF2A7D, 2, cameraName, unk1);
}
void DESTROY_CAM(Camera camera, bool destroy)
{
	_CALL_NATIVE<DWORD>(0xC39302BD, 2, camera, destroy);
}
void SET_CAM_ACTIVE(Camera camera, bool active)
{
	_CALL_NATIVE<DWORD>(0x064659C2, 2, camera, active);
}
void SET_CAM_ROT(Camera camera, float x, float y, float z, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0xEE38B3C1, 5, camera, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, unk1);
}
void ATTACH_CAM_TO_PED_BONE(Camera camera, Ped ped, Prop bone, float x, float y, float z, bool unk1)
{
	_CALL_NATIVE<DWORD>(0x506BB35C, 7, camera, ped, bone, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, unk1);
}
void DETACH_CAM(Camera camera)
{
	_CALL_NATIVE<DWORD>(0xF4FBF14A, 1, camera);
}
void SHAKE_CAM(Camera camera, char* shakeType, float shakeAmplitude)
{
	_CALL_NATIVE<DWORD>(0x1D4211B0, 3, camera, shakeType, *(DWORD*)&shakeAmplitude);
}
void SET_CAM_SHAKE_AMPLITUDE(Camera camera, float shakeAmplitude)
{
	_CALL_NATIVE<DWORD>(0x60FF6382, 2, camera, *(DWORD*)&shakeAmplitude);
}
void STOP_CAM_SHAKING(Camera camera, bool stopShake)
{
	_CALL_NATIVE<DWORD>(0x40D0EB87, 2, camera, stopShake);
}
Position GET_GAMEPLAY_CAM_ROT(DWORD unk1)
{
	return _CALL_NATIVE<Position>(0x13A010B5, 1, unk1);
}
void SET_CINEMATIC_BUTTON_ACTIVE(bool cinematicButtonActive)
{
	_CALL_NATIVE<DWORD>(0x3FBC5D00, 1, cinematicButtonActive);
}
/// WEAPON
void SET_PED_INFINITE_AMMO_CLIP(Ped ped, bool infiniteAmmoClip)
{
	_CALL_NATIVE<DWORD>(0x5A5E3B67, 2, ped, infiniteAmmoClip);
}
void GIVE_WEAPON_TO_PED(Ped ped, Weapon weapon, DWORD ammo, bool unk1, bool equipNow)
{
	_CALL_NATIVE<DWORD>(0xC4D88A85, 5, ped, weapon, ammo, unk1, equipNow);
}
void GIVE_DELAYED_WEAPON_TO_PED(Ped ped, Weapon weapon, DWORD time, bool equipNow)
{
	_CALL_NATIVE<DWORD>(0x5868D20D, 4, ped, weapon, time, equipNow);
}
void REMOVE_ALL_PED_WEAPONS(Ped ped)
{
	_CALL_NATIVE<DWORD>(0xA44CE817, 1, ped);
}
Position GET_PED_LAST_WEAPON_IMPACT_COORD(Ped ped)
{
	Position buffer;
	if (!_CALL_NATIVE<BOOL>(0x9B266079, 2, ped, &buffer))
		return _Position;
	return buffer;
}
void REQUEST_WEAPON_ASSET(Weapon weapon, DWORD unk1, bool unk2)
{
	_CALL_NATIVE<DWORD>(0x65D139A5, 3, weapon, unk1, unk2);
}
bool HAS_WEAPON_ASSET_LOADED(Weapon weapon)
{
	return _CALL_NATIVE<BOOL>(0x1891D5BB, 1, weapon);
}
/// STREAMING
void LOAD_ALL_OBJECTS_NOW()
{
	_CALL_NATIVE<DWORD>(0xC9DBDA90, 0);
}
void REQUEST_MODEL(DWORD modelHash)
{
	_CALL_NATIVE<DWORD>(0xFFF1B500, 1, modelHash);
}
bool HAS_MODEL_LOADED(DWORD modelHash)
{
	return _CALL_NATIVE<BOOL>(0x62BFDB37, 1, modelHash);
}
void SET_MODEL_AS_NO_LONGER_NEEDED(DWORD modelHash)
{
	_CALL_NATIVE<DWORD>(0xAE0F069E, 1, modelHash);
}
bool IS_MODEL_IN_CDIMAGE(DWORD modelHash)
{
	return _CALL_NATIVE<BOOL>(0x1094782F, 1, modelHash);
}
void REQUEST_ANIM_DICT(char* anim)
{
	_CALL_NATIVE<DWORD>(0xDCA96950, 1, anim);
}
bool HAS_ANIM_DICT_LOADED(char* anim)
{
	return _CALL_NATIVE<BOOL>(0x05E6763C, 1, anim);
}
void REQUEST_CLIP_SET(char* clipSet)
{
	_CALL_NATIVE<DWORD>(0x546C627A, 1, clipSet);
}
bool HAS_CLIP_SET_LOADED(char* clipSet)
{
	return _CALL_NATIVE<BOOL>(0x230D5455, 1, clipSet);
}
/// UI
void _0xF42C43C7(char* type)
{
	_CALL_NATIVE<DWORD>(0xF42C43C7, 1, type);
}
void _0x38F82261(DWORD milliseconds, DWORD unk1)
{
	_CALL_NATIVE<DWORD>(0x38F82261, 2, milliseconds, unk1);
}
void _0x3E35563E(char* text)
{
	_CALL_NATIVE<DWORD>(0x3E35563E, 1, text);
}
void _0x6F8350CE(float x, float y)
{
	_CALL_NATIVE<DWORD>(0x6F8350CE, 2, *(DWORD*)&x, *(DWORD*)&y);
}
void _0x94B82066(char* text)
{
	_CALL_NATIVE<DWORD>(0x94B82066, 1, text);
}
void _0xAA318785(float x, float y)
{
	_CALL_NATIVE<DWORD>(0xAA318785, 2, *(DWORD*)&x, *(DWORD*)&y);
}
void SET_TEXT_COMPONENT_FORMAT(char* type)
{
	_CALL_NATIVE<DWORD>(0xB245FC10, 1, type);
}
void DISPLAY_HELP_TEXT_FROM_STRING_LABEL(DWORD unk1, bool loop, bool beep, DWORD shape)
{
	_CALL_NATIVE<DWORD>(0xB59B530D, 4, unk1, loop, beep, shape);
}
void _0x27A244D8(char* text)
{
	_CALL_NATIVE<DWORD>(0x27A244D8, 1, text);
}
void CLEAR_PRINTS()
{
	_CALL_NATIVE<DWORD>(0x216CB1C5, 0);
}
void SET_TEXT_SCALE(float x, float y)
{
	_CALL_NATIVE<DWORD>(0xB6E15B23, 2, *(DWORD*)&x, *(DWORD*)&y);
}
void SET_TEXT_COLOUR(BYTE R, BYTE G, BYTE B, BYTE A)
{
	_CALL_NATIVE<DWORD>(0xE54DD2C8, 4, R, G, B, A);
}
void SET_TEXT_CENTRE(bool center)
{
	_CALL_NATIVE<DWORD>(0xE26D39A1, 1, center);
}
void SET_TEXT_WRAP(float x, float y)
{
	_CALL_NATIVE<DWORD>(0x6F60AB54, 2, *(DWORD*)&x, *(DWORD*)&y);
}
void SET_TEXT_FONT(DWORD font)
{
	_CALL_NATIVE<DWORD>(0x80BC530D, 1, font);
}
void SET_TEXT_DROPSHADOW(DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4, DWORD unk5)
{
	_CALL_NATIVE<DWORD>(0xE6587517, 5, unk1, unk2, unk3, unk4, unk5);
}
void SET_TEXT_OUTLINE()
{
	_CALL_NATIVE<DWORD>(0xC753412F, 0);
}
void SET_TEXT_EDGE(DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4, DWORD unk5)
{
	_CALL_NATIVE<DWORD>(0x3F1A5DAB, 5, unk1, unk2, unk3, unk4, unk5);
}
DWORD GET_FIRST_BLIP_INFO_ID(DWORD blip)
{
	return _CALL_NATIVE<DWORD>(0x64C0273D, 1, blip);
}
Position GET_BLIP_COORDS(DWORD blip)
{
	return _CALL_NATIVE<Position>(0xEF6FF47B, 1, blip);
}
bool DOES_BLIP_EXIST(DWORD blip)
{
	return _CALL_NATIVE<BOOL>(0xAE92DD96, 1, blip);
}
void HIDE_HELP_TEXT_THIS_FRAME()
{
	_CALL_NATIVE<DWORD>(0xF3807BED, 0);
}
void HIDE_HUD_COMPONENT_THIS_FRAME(DWORD component)
{
	_CALL_NATIVE<DWORD>(0xDB2D0762, 1, component);
}
/// GRAPHICS
void DRAW_LINE(float x1, float y1, float z1, float x2, float y2, float z2, BYTE R, BYTE G, BYTE B, BYTE A)
{
	_CALL_NATIVE<DWORD>(0xB3426BCC, 10, *(DWORD*)&x1, *(DWORD*)&y1, *(DWORD*)&z1, *(DWORD*)&x2, *(DWORD*)&y2, *(DWORD*)&z2, R, G, B, A);
}
void DRAW_RECT(float x, float y, float width, float height, BYTE R, BYTE G, BYTE B, BYTE A)
{
	_CALL_NATIVE<DWORD>(0xDD2BFC77, 8, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&width, *(DWORD*)&height, R, G, B, A);
}
void SET_BLACKOUT(bool blackout)
{
	_CALL_NATIVE<DWORD>(0xAA2A0EAF, 1, blackout);
}
/// STATS
void STAT_SET_INT(DWORD stat, DWORD value, bool save)
{
	_CALL_NATIVE<DWORD>(0xC9CC1C5C, 3, stat, value, save);
}
void STAT_SET_BOOL(DWORD stat, bool value, bool save)
{
	_CALL_NATIVE<DWORD>(0x55D79DFB, 3, stat, value, save);
}
DWORD STAT_GET_INT(DWORD stat, DWORD unk1)
{
	DWORD ret;
	_CALL_NATIVE<DWORD>(0x1C6FE43E, 3, stat, &ret, unk1);
	return ret;
}
bool STAT_GET_BOOL(DWORD stat, DWORD unk1)
{
	DWORD ret;
	_CALL_NATIVE<DWORD>(0x28A3DD2B, 3, stat, &ret, unk1);
	return ret != 0;
}
/// TIME
DWORD GET_CLOCK_HOURS()
{
	return _CALL_NATIVE<DWORD>(0x7EF8316F, 0);
}
void NETWORK_OVERRIDE_CLOCK_TIME(DWORD hour, DWORD minute, DWORD second)
{
	_CALL_NATIVE<DWORD>(0xC077BCD6, 3, hour, minute, second);
}
/// CONTROLS
bool IS_CONTROL_PRESSED(DWORD index, DWORD control)
{
	return _CALL_NATIVE<BOOL>(0x517A4384, 2, index, control);
}
bool IS_CONTROL_JUST_PRESSED(DWORD index, DWORD control)
{
	return _CALL_NATIVE<BOOL>(0x4487F579, 2, index, control);
}
bool IS_CONTROL_JUST_RELEASED(DWORD index, DWORD control)
{
	return _CALL_NATIVE<BOOL>(0x2314444B, 2, index, control);
}
void SET_INPUT_EXCLUSIVE(DWORD index, DWORD control)
{
	_CALL_NATIVE<DWORD>(0x4E8E29E6, 2, index, control);
}
void DISABLE_CONTROL_ACTION(DWORD index, DWORD control, bool disable)
{
	_CALL_NATIVE<DWORD>(0x3800C0DC, 3, index, control, disable);
}
/// FIRE
void START_ENTITY_FIRE(Entity entity)
{
	_CALL_NATIVE<DWORD>(0x8928428E, 1, entity);
}
void STOP_ENTITY_FIRE(Entity entity)
{
	_CALL_NATIVE<DWORD>(0xCE8C9066, 1, entity);
}
bool IS_ENTITY_ON_FIRE(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x8C73E64F, 1, entity);
}
void ADD_EXPLOSION(float x, float y, float z, DWORD explosion, float radius, bool noSound, bool visible, float cameraShake)
{
	_CALL_NATIVE<DWORD>(0x10AF5258, 8, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, explosion, *(DWORD*)&radius, noSound, visible, *(DWORD*)&cameraShake);
}
void ADD_OWNED_EXPLOSION(Player player, float x, float y, float z, DWORD explosion, float radius, bool noSound, bool visible, float cameraShake)
{
	_CALL_NATIVE<DWORD>(0x27EE0D67, 9, player, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, explosion, *(DWORD*)&radius, noSound, visible, *(DWORD*)&cameraShake);
}
/// ROPE
Rope ADD_ROPE(float x, float y, float z, float angleX, float angleY, float angleZ, float len, DWORD type, float lenMax, float lenMin, float unk1, bool unk2, bool unk3, bool unk4, float unk5, bool breakable)
{
	return _CALL_NATIVE<Rope>(0xA592EC74, 16, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&angleX, *(DWORD*)&angleY, *(DWORD*)&angleZ, *(DWORD*)&len, type, *(DWORD*)&lenMax, *(DWORD*)&lenMin, *(DWORD*)&unk1, unk2, unk3, unk4, *(DWORD*)&unk5, breakable);
}
void DELETE_ROPE(Rope rope)
{
	Rope buffer = rope;
	_CALL_NATIVE<DWORD>(0x748D72AF, 1, &buffer);
}
void PIN_ROPE_VERTEX(Rope rope, DWORD vertex, float x, float y, float z)
{
	_CALL_NATIVE<DWORD>(0xAE1D101B, 5, rope, vertex, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z);
}
DWORD GET_ROPE_VERTEX_COUNT(Rope rope)
{
	return _CALL_NATIVE<DWORD>(0x5131CD2C, 1, rope);
}
void ATTACH_ENTITIES_TO_ROPE(Rope rope, Entity ent1, Entity ent2, float x1, float y1, float z1, float x2, float y2, float z2, float distance, bool unk1, bool unk2, bool unk3, bool unk4)
{
	_CALL_NATIVE<DWORD>(0x7508668F, 14, rope, ent1, ent2, *(DWORD*)&x1, *(DWORD*)&y1, *(DWORD*)&z1, *(DWORD*)&x2, *(DWORD*)&y2, *(DWORD*)&z2, *(DWORD*)&distance, unk1, unk2, unk3, unk4);
}
void ATTACH_ROPE_TO_ENTITY(Rope rope, Entity entity, float x, float y, float z, bool unk1)
{
	_CALL_NATIVE<DWORD>(0xB25D9536, 6, rope, entity, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, unk1);
}
void _0xFC0DB4C3(Rope rope)
{
	_CALL_NATIVE<DWORD>(0xFC0DB4C3, 1, rope);
}
void ROPE_LOAD_TEXTURES()
{
	_CALL_NATIVE<DWORD>(0xBA97CE91, 0);
}
void ACTIVATE_PHYSICS(Rope rope)
{
	_CALL_NATIVE<DWORD>(0x031711B8, 1, rope);
}
/// WATER
void MODIFY_WATER(float x, float y, float z, float height)
{
	_CALL_NATIVE<DWORD>(0xC49E005A, 4, *(DWORD*)&x, *(DWORD*)&y, *(DWORD*)&z, *(DWORD*)&height);
}
/// NETWORK
bool NETWORK_IS_PLAYER_ACTIVE(Player player)
{
	return _CALL_NATIVE<BOOL>(0x43657B17, 1, player);
}
bool _0xD7F934F4(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0xD7F934F4, 1, entity);
}
bool NETWORK_GET_ENTITY_IS_LOCAL(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x813353ED, 1, entity);
}
bool NETWORK_DOES_NETWORK_ID_EXIST(NetworkID networkId)
{
	return _CALL_NATIVE<BOOL>(0xB8D2C99E, 1, networkId);
}
bool NETWORK_REQUEST_CONTROL_OF_ENTITY(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0xA05FEBD7, 1, entity);
}
bool NETWORK_HAS_CONTROL_OF_ENTITY(Entity entity)
{
	return _CALL_NATIVE<BOOL>(0x005FD797, 1, entity);
}
NetworkID VEH_TO_NET(Vehicle vehicle)
{
	return _CALL_NATIVE<NetworkID>(0xF17634EB, 1, vehicle);
}
Vehicle NET_TO_VEH(NetworkID networkId)
{
	return _CALL_NATIVE<Vehicle>(0x7E02FAEA, 1, networkId);
}
bool NETWORK_IS_PLAYER_TALKING(Player player)
{
	return _CALL_NATIVE<BOOL>(0xDA9FD9DB, 1, player);
}
bool NETWORK_IS_PLAYER_MUTED_BY_ME(Player player)
{
	return _CALL_NATIVE<BOOL>(0x7A21050E, 1, player);
}
void SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NetworkID networkId, bool existsOnAllMachines)
{
	_CALL_NATIVE<DWORD>(0x68D486B2, 2, networkId, existsOnAllMachines);
}
bool CAN_REGISTER_MISSION_VEHICLES(DWORD unk1)
{
	return _CALL_NATIVE<BOOL>(0x818B6830, 1, unk1);
}
/// DECORATOR
void _0xDB718B21(DWORD unk1, char* unk2, DWORD unk3)
{
	_CALL_NATIVE<DWORD>(0xDB718B21, 3, unk1, unk2, unk3);
}