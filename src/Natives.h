#pragma once

//// Includes
#include "stdafx.h"
#include "Globals.h"
#include <vector>

//// Definitions
#define Camera DWORD
#define Entity DWORD
#define NetworkID DWORD
#define Ped DWORD
#define Player DWORD
#define Position Vector3
#define Prop DWORD
#define Rope DWORD
#define Team DWORD
#define Vehicle DWORD
#define Weapon DWORD

//// Methods
/// GLOBALS
PDWORD GLOBAL_GET(DWORD globalID);
DWORD GLOBAL_READ(DWORD globalID);
void GLOBAL_WRITE(DWORD globalID, DWORD value);
/// CUSTOM
void _APPLY_RELATIVE_FORCE(Entity entity, Position direction);
void _APPLY_RELATIVE_FORCE(Entity entity, Position direction, Position rotation);
Rope _ATTACH_ENTITIES_TO_ROPE(Rope rope, Entity ent1, Entity ent2, float x1, float y1, float z1, float x2, float y2, float z2, float distanceCoef = 1.0, float forcedDistance = 0.0, Position ent1Offset = Position(0.0, 0.0, 0.0), Position ent2Offset = Position(0.0, 0.0, 0.0));
void _ATTACH_ROPE_TO_ENTITY(Rope rope, Entity entity, float x, float y, float z, Position offset = Position(0, 0, 0), Position forcedPosition = Position(0, 0, 0));
void _ATTACH_TO_TARGET(Entity entity, Position forcedPosition = Position(0, 0, 0));
bool _ATTACH_VEHICLE_TO_VEHICLE(Vehicle toattach, Vehicle attachto);
void _CALL_AIRSTRIKE(float x, float y, float z);
Rope _CREATE_ROPE(float x1, float y1, float z1, float x2, float y2, float z2, float distanceCoef = 1.0, float forcedDistance = 0.0);
Rope _CREATE_ROPE_FOR_PLAYER(Entity entity, Position forcedPosition = Position(0, 0, 0), Position ropeOffset = Position(0, 0, 0));
float _DEGREE_TO_RADIAN(float degree);
Position _DIRECTION_TO_ROTATION(Position direction, float roll);
void _DISABLE_CONTROLS();
void _DISPLAY_TEXT(char* text, DWORD font, float x, float y, float width = 0.35, float height = 0.35, BYTE R = 0xFF, BYTE G = 0xFF, BYTE B = 0xFF, BYTE A = 0xFF, bool outline = true, bool center = false, float wrapWidth = 0.0, float wrapHeight = 1.0);
void _DROP_CASH(Position coords, DWORD amount);
void _DROP_HEALTH(Position coords);
DWORD _GET_ADDRESS(DWORD native);
Position _GET_CAMERA_DIRECTION();
Position _GET_CAMERA_DIRECTION(float distance);
float _GET_FPS();
std::vector<Ped> _GET_NEARBY_PEDS(float radius, DWORD maxAmount);
void _PLAY_ANIMATION(Entity entity, char* animSet, char* animName, float speed, DWORD duration, bool lastAnimation, float playbackRate);
void _PRINT_HELP_TEXT(char* text, ...);
void _PRINT_TEXT(char* text, ...);
float _RADIAN_TO_DEGREE(float radian);
bool _REQUEST_ANIM_DICT(char* dict);
bool _REQUEST_CLIP_SET(char* dict);
bool _REQUEST_NETWORK_CONTROL(Entity entity);
Position _ROTATION_TO_DIRECTION(Position rot);
Vehicle _SPAWN_VEHICLE(DWORD modelHash);
Vehicle _SPAWN_VEHICLE(DWORD modelHash, float x, float y, float z);
void _TELEPORT(float x, float y, float z);
void _TELEPORT(Position position);
/// PLAYER
Ped GET_PLAYER_PED(Player player);
void SET_PLAYER_MODEL(Player player, DWORD modelHash);
void CHANGE_PLAYER_PED(Player player, Ped ped, bool unk1 = false, bool unk2 = false);
Team GET_PLAYER_TEAM(Player player);
void SET_PLAYER_TEAM(Player player, Team team);
char* GET_PLAYER_NAME(Player player);
void CLEAR_PLAYER_WANTED_LEVEL(Player player);
bool IS_PLAYER_DEAD(Player player);
bool IS_PLAYER_PRESSING_HORN(Player player);
void SET_MAX_WANTED_LEVEL(DWORD wantedLevel);
DWORD GET_MAX_WANTED_LEVEL();
Prop GET_AIMED_ENTITY(Player player);
Player PLAYER_ID();
Ped PLAYER_PED_ID();
void SET_PLAYER_INVINCIBLE(Player player, bool invincible);
bool GET_PLAYER_INVINCIBLE(Player player);
void SET_SWIM_SPEED_MULTIPLIER(Player player, float multiplier);
void SET_MOVE_SPEED_MULTIPLIER(Player player, float multiplier);
/// ENTITY
bool DOES_ENTITY_EXIST(Entity entity);
bool HAS_ENTITY_CLEAR_LOS_TO_ENTITY(Entity ent1, Entity ent2, DWORD trace);
Entity GET_ENTITY_ATTACHED_TO(Entity entity);
Position GET_ENTITY_COORDS(Entity entity, bool alive = true);
float GET_ENTITY_HEADING(Entity entity);
DWORD GET_ENTITY_HEALTH(Entity entity);
Position GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(Entity entity, float x, float y, float z);
float GET_ENTITY_PITCH(Entity entity);
float GET_ENTITY_ROLL(Entity entity);
Position GET_ENTITY_ROTATION(Entity entity, DWORD unk1 = 2);
DWORD GET_ENTITY_MODEL(Entity entity);
float GET_ENTITY_SPEED(Entity entity);
bool IS_ENTITY_A_PED(Entity entity);
bool IS_ENTITY_A_MISSION_ENTITY(Entity entity);
bool IS_ENTITY_DEAD(Entity entity);
bool IS_ENTITY_IN_WATER(Entity entity);
bool IS_ENTITY_VISIBLE(Entity entity);
void APPLY_FORCE_TO_ENTITY(Entity entity, DWORD forceType, float x, float y, float z, float xRot, float yRot, float zRot, bool unk1, bool isRel, bool ignoreUpVec, bool unk2, bool unk3, bool unk4);
void ATTACH_ENTITY_TO_ENTITY(Entity ent1, Entity ent2, Prop bone, float x, float y, float z, float xRot, float yRot, float zRot, bool unk1, bool isRel, bool ignoreUpVec, bool allowRot, DWORD unk2, bool unk3);
void DETACH_ENTITY(Entity entity, bool unk1 = true, bool unk2 = true);
void FREEZE_ENTITY_POSITION(Entity entity, bool freeze);
void _0xD3850671(Entity entity, DWORD unk1 = 1);
void SET_ENTITY_AS_MISSION_ENTITY(Entity entity, bool missionEntity, bool unk1 = true);
void SET_ENTITY_COLLISION(Entity entity, bool collide);
void SET_ENTITY_COORDS(Entity entity, float x, float y, float z, DWORD unk1 = 0, DWORD unk2 = 0, DWORD unk3 = 0, DWORD unk4 = 1);
void SET_ENTITY_HEADING(Entity entity, float heading);
void SET_ENTITY_HEALTH(Entity entity, DWORD health);
void SET_ENTITY_PROOFS(Entity entity, bool bulletProof, bool fireProof, bool explosionProof, bool collisionProof, bool meleeProof, bool unk1, bool unk2, bool drownProof);
void SET_ENTITY_ROTATION(Entity entity, float pitch, float roll, float yaw, DWORD unk1 = 2, bool unk2 = true);
void SET_ENTITY_VISIBLE(Entity entity, bool visible);
/// PED
Ped CREATE_PED(DWORD type, DWORD modelHash, float x, float y, float z, bool unk1 = false, bool networkPed = true, bool createHandle = true);
void DELETE_PED(Ped ped);
bool IS_PED_IN_VEHICLE(Ped ped, Vehicle vehicle, bool atGetIn);
bool IS_PED_IN_ANY_VEHICLE(Ped ped, DWORD unk1 = 0);
bool IS_PED_DEAD(Ped ped, DWORD unk1 = 1);
bool IS_PED_A_PLAYER(Ped ped);
bool IS_PED_SHOOTING(Ped ped);
void SET_PED_INTO_VEHICLE(Ped ped, Vehicle vehicle, DWORD unk1 = -1);
bool IS_PED_HUMAN(Ped ped);
Vehicle GET_VEHICLE_PED_IS_IN(Ped ped, DWORD unk1 = 0);
void SET_PED_SUFFERS_CRITICAL_HITS(Ped ped, bool suffersCriticalHits);
bool IS_PED_ON_FOOT(Ped ped);
bool IS_PED_ON_ANY_BIKE(Ped ped);
bool IS_PED_IN_ANY_BOAT(Ped ped);
bool IS_PED_IN_ANY_SUB(Ped ped);
bool IS_PED_IN_ANY_HELI(Ped ped);
bool IS_PED_IN_ANY_PLANE(Ped ped);
void SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(Ped ped, DWORD state);
void KNOCK_PED_OFF_VEHICLE(Ped ped);
void SET_PED_KEEP_TASK(Ped ped, bool keepTask);
void SET_PED_AS_ENEMY(Ped ped, bool enemy);
bool IS_PED_GETTING_INTO_A_VEHICLE(Ped ped);
void SET_PED_MOVEMENT_CLIPSET(Ped ped, char* clipSet, DWORD unk1);
void SET_PED_RANDOM_COMPONENT_VARIATION(Ped ped, DWORD unk1 = 0);
void SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Ped ped, bool block);
void SET_PED_TO_RAGDOLL(Ped ped, DWORD time1, DWORD time2, DWORD type, bool unk1 = false, bool unk2 = false, bool unk3 = false);
void SET_PED_CAN_RAGDOLL(Ped ped, bool canRagdoll);
void REVIVE_INJURED_PED(Ped ped);
void SET_PED_DIES_WHEN_INJURED(Ped ped, bool diesWhenInjured);
Position GET_PED_BONE_COORDS(Ped ped, Prop bone = 0, float unk2 = 0.0, float unk3 = 0.0, float unk4 = 0.0);
void APPLY_PED_DAMAGE_PACK(Ped ped, char* damagePack, float unk1, float unk2);
void SET_PED_GENERATES_DEAD_BODY_EVENTS(Ped ped, bool generatesDeadBodyEvents);
void SET_PED_COMBAT_ATTRIBUTES(Ped ped, DWORD attribute, bool unk1);
void SET_PED_FLEE_ATTRIBUTES(Ped ped, DWORD attribute, bool unk1);
DWORD GET_PED_NEARBY_PEDS(Ped ped, DWORD* sizeAndPeds, DWORD unk1);
/// VEHICLE
Vehicle CREATE_VEHICLE(DWORD modelHash, float x, float y, float z, float heading, bool networkVehicle = true, bool createHandle = true);
void DELETE_VEHICLE(Vehicle vehicle);
void SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle vehicle);
void SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float densityMultiplier);
void SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(float densityMultiplier);
void SET_VEHICLE_DOORS_LOCKED(Vehicle vehicle, bool doorsLocked);
void SET_VEHICLE_DOORS_LOCKED_FOR_TEAM(Vehicle vehicle, Team team, bool lockedForTeam);
void SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle vehicle, BYTE R, BYTE G, BYTE B);
void SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle vehicle, BYTE R, BYTE G, BYTE B);
Ped GET_PED_IN_VEHICLE_SEAT(Vehicle vehicle, DWORD seatIndex);
void SET_VEHICLE_FORWARD_SPEED(Vehicle vehicle, float forwardSpeed);
void SET_VEHICLE_TYRES_CAN_BURST(Vehicle vehicle, bool tyresCanBurst);
void SET_VEHICLE_DOOR_OPEN(Vehicle vehicle, DWORD doorIndex, bool loose, bool openInstantly);
void SET_VEHICLE_LIGHTS(Vehicle vehicle, DWORD unk1 = 3);
void SET_VEHICLE_DIRT_LEVEL(Vehicle vehicle, float dirtLevel);
void SET_VEHICLE_ENGINE_ON(Vehicle vehicle, bool toggle, bool instantly);
Vehicle GET_CLOSEST_VEHICLE(float x, float y, float z, float radius, DWORD modelHash = 0, DWORD flags = 70);
bool IS_THIS_MODEL_A_PLANE(DWORD modelHash);
bool IS_THIS_MODEL_A_HELI(DWORD modelHash);
void SET_HELI_BLADES_FULL_SPEED(Vehicle vehicle);
void SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehicle vehicle, bool canBeVisiblyDamaged);
bool IS_VEHICLE_ON_ALL_WHEELS(Vehicle vehicle);
void SET_VEHICLE_FIXED(Vehicle vehicle);
void SET_VEHICLE_DEFORMATION_FIXED(Vehicle vehicle);
void SET_VEHICLE_ENGINE_HEALTH(Vehicle vehicle, float engineHealth);
void SET_VEHICLE_BOOST_ACTIVE(Vehicle vehicle, bool boostActive);
void SET_VEHICLE_REDUCE_GRIP(Vehicle vehicle, bool reduceGrip);
void SET_VEHICLE_TYRE_SMOKE_COLOR(Vehicle vehicle, BYTE r, BYTE g, BYTE b);
void SET_VEHICLE_IS_STOLEN(Vehicle vehicle, bool isStolen);
/// OBJECT
Prop CREATE_OBJECT(DWORD modelHash, float x, float y, float z, bool networkObject = true, bool createHandle = true, bool dynamic = false);
void DELETE_OBJECT(Prop object);
void CREATE_AMBIENT_PICKUP(DWORD modelHash, float x, float y, float z, bool unk1 = false, DWORD value = 0, DWORD unk2 = 0x113FD533, bool unk3 = false, bool unk4 = true);
/// AI
void TASK_PARACHUTE(Ped ped, bool unk1 = true);
void TASK_PARACHUTE_TO_TARGET(Ped ped, float x, float y, float z);
void TASK_FOLLOW_NAV_MESH_TO_COORD(Ped ped, float x, float y, float z, float speed, DWORD timeout, float unk1, DWORD unk2, float unk3);
void TASK_PLAY_ANIM(Ped ped, char* animDict, char* animName, float speed, float speedMultiplier, DWORD duration, DWORD flag, float playbackRate, bool lockX, bool lockY, bool lockZ);
void CLEAR_PED_TASKS(Ped ped);
void CLEAR_PED_TASKS_IMMEDIATELY(Ped ped);
void TASK_ARREST_PED(Ped ped, Ped target);
/// GAMEPLAY
void SET_TIME_SCALE(float timescale);
void SET_OVERRIDE_WEATHER(char* weather);
float GET_FRAME_TIME();
DWORD GET_RANDOM_INT_IN_RANGE(DWORD start, DWORD end);
float GET_GROUND_Z_FOR_3D_COORD(float x, float y, float z);
float GET_DISTANCE_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, bool unk1 = true);
DWORD GET_HASH_KEY(char* key);
void CLEAR_AREA_OF_VEHICLES(float x, float y, float z, float radius, bool unk1 = false, bool unk2 = false, bool unk3 = true, bool unk4 = true, bool unk5 = true);
void SHOOT_SINGLE_BULLET_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, DWORD damage, bool unk1, Weapon weapon, Ped ped, bool audible, bool unk2, float speed);
void SET_GRAVITY_LEVEL(float gravityLevel);
void SET_EXPLOSIVE_AMMO_THIS_FRAME(Player player);
void SET_FIRE_AMMO_THIS_FRAME(Player player);
void SET_EXPLOSIVE_MELEE_THIS_FRAME(Player player);
void SET_SUPER_JUMP_THIS_FRAME(Player player);
/// AUDIO
void STOP_PED_SPEAKING(Ped ped, bool speaking);
void DISABLE_PED_PAIN_AUDIO(Ped ped, bool disable);
/// CAM
void RENDER_SCRIPT_CAMS(bool render, bool unk1, DWORD unk2, bool unk3, bool unk4);
Camera CREATE_CAM(char* cameraName, bool unk1);
void DESTROY_CAM(Camera camera, bool destroy);
void SET_CAM_ACTIVE(Camera camera, bool active);
void SET_CAM_ROT(Camera camera, float x, float y, float z, DWORD unk1 = 2);
void ATTACH_CAM_TO_PED_BONE(Camera camera, Ped ped, Prop bone, float x, float y, float z, bool unk1 = true);
void DETACH_CAM(Camera camera);
void SHAKE_CAM(Camera camera, char* shakeType, float shakeAmplitude);
void SET_CAM_SHAKE_AMPLITUDE(Camera camera, float shakeAmplitude);
void STOP_CAM_SHAKING(Camera camera, bool stopShake);
Position GET_GAMEPLAY_CAM_ROT(DWORD unk1 = 0);
void SET_CINEMATIC_BUTTON_ACTIVE(bool cinematicButtonActive);
/// WEAPON
void SET_PED_INFINITE_AMMO_CLIP(Ped ped, bool infiniteAmmoClip);
void GIVE_WEAPON_TO_PED(Ped ped, Weapon weapon, DWORD ammo, bool unk1 = false, bool equipNow = true);
void GIVE_DELAYED_WEAPON_TO_PED(Ped ped, Weapon weapon, DWORD time, bool equipNow);
void REMOVE_ALL_PED_WEAPONS(Ped ped);
Position GET_PED_LAST_WEAPON_IMPACT_COORD(Ped ped);
void REQUEST_WEAPON_ASSET(Weapon weapon, DWORD unk1, bool unk2);
bool HAS_WEAPON_ASSET_LOADED(Weapon weapon);
/// STREAMING
void LOAD_ALL_OBJECTS_NOW();
void REQUEST_MODEL(DWORD modelHash);
bool HAS_MODEL_LOADED(DWORD modelHash);
void SET_MODEL_AS_NO_LONGER_NEEDED(DWORD modelHash);
bool IS_MODEL_IN_CDIMAGE(DWORD modelHash);
void REQUEST_ANIM_DICT(char* anim);
bool HAS_ANIM_DICT_LOADED(char* anim);
void REQUEST_CLIP_SET(char* clipSet);
bool HAS_CLIP_SET_LOADED(char* clipSet);
/// UI
void _0xF42C43C7(char* type);
void _0x38F82261(DWORD milliseconds, DWORD unk1 = 1);
void _0x3E35563E(char* text);
void _0x6F8350CE(float x, float y);
void _0x94B82066(char* text);
void _0xAA318785(float x, float y);
void SET_TEXT_COMPONENT_FORMAT(char* type);
void DISPLAY_HELP_TEXT_FROM_STRING_LABEL(DWORD unk1, bool loop, bool beep, DWORD shape);
void _0x27A244D8(char* text);
void CLEAR_PRINTS();
void SET_TEXT_SCALE(float x, float y);
void SET_TEXT_COLOUR(BYTE R, BYTE G, BYTE B, BYTE A = 0xFF);
void SET_TEXT_CENTRE(bool center);
void SET_TEXT_WRAP(float x, float y);
void SET_TEXT_FONT(DWORD font);
void SET_TEXT_DROPSHADOW(DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4, DWORD unk5);
void SET_TEXT_OUTLINE();
void SET_TEXT_EDGE(DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4, DWORD unk5);
DWORD GET_FIRST_BLIP_INFO_ID(DWORD blip);
Position GET_BLIP_COORDS(DWORD blip);
bool DOES_BLIP_EXIST(DWORD blip);
void HIDE_HELP_TEXT_THIS_FRAME();
void HIDE_HUD_COMPONENT_THIS_FRAME(DWORD component);
/// GRAPHICS
void DRAW_LINE(float x1, float y1, float z1, float x2, float y2, float z2, BYTE R, BYTE G, BYTE B, BYTE A);
void DRAW_RECT(float x, float y, float width, float height, BYTE R, BYTE G, BYTE B, BYTE A);
void SET_BLACKOUT(bool blackout);
/// STATS
void STAT_SET_INT(DWORD stat, DWORD value, bool save = true);
void STAT_SET_BOOL(DWORD stat, bool value, bool save = true);
DWORD STAT_GET_INT(DWORD stat, DWORD unk1 = -1);
bool STAT_GET_BOOL(DWORD stat, DWORD unk1 = -1);
/// TIME
DWORD GET_CLOCK_HOURS();
void NETWORK_OVERRIDE_CLOCK_TIME(DWORD hour, DWORD minute, DWORD second);
/// CONTROL
bool IS_CONTROL_PRESSED(DWORD index, DWORD control);
bool IS_CONTROL_JUST_PRESSED(DWORD index, DWORD control);
bool IS_CONTROL_JUST_RELEASED(DWORD index, DWORD control);
void SET_INPUT_EXCLUSIVE(DWORD index, DWORD control);
void DISABLE_CONTROL_ACTION(DWORD index, DWORD control, bool disable = true);
/// FIRE
void START_ENTITY_FIRE(Entity entity);
void STOP_ENTITY_FIRE(Entity entity);
bool IS_ENTITY_ON_FIRE(Entity entity);
void ADD_EXPLOSION(float x, float y, float z, DWORD explosion, float radius, bool noSound, bool visible, float cameraShake);
void ADD_OWNED_EXPLOSION(Player player, float x, float y, float z, DWORD explosion, float radius, bool noSound, bool visible, float cameraShake);
/// ROPE
Rope ADD_ROPE(float x, float y, float z, float angleX, float angleY, float angleZ, float len, DWORD type, float lenMax, float lenMin = 1.0, float unk1 = 0.0, bool unk2 = false, bool unk3 = false , bool unk4 = false, float unk5 = 10.0, bool breakable = false);
void DELETE_ROPE(Rope rope);
void PIN_ROPE_VERTEX(Rope rope, DWORD vertex, float x, float y, float z);
DWORD GET_ROPE_VERTEX_COUNT(Rope rope);
void ATTACH_ENTITIES_TO_ROPE(Rope rope, Entity ent1, Entity ent2, float x1, float y1, float z1, float x2, float y2, float z2, float distance, bool unk1 = false, bool unk2 = false, bool unk3 = false, bool unk4 = false);
void ATTACH_ROPE_TO_ENTITY(Rope rope, Entity entity, float x, float y, float z, bool unk1 = false);
void _0xFC0DB4C3(Rope rope);
void ROPE_LOAD_TEXTURES();
void ACTIVATE_PHYSICS(Rope rope);
/// WATER
void MODIFY_WATER(float x, float y, float z, float height);
/// NETWORK
bool NETWORK_IS_PLAYER_ACTIVE(Player player);
bool _0xD7F934F4(Entity entity);
bool NETWORK_GET_ENTITY_IS_LOCAL(Entity entity);
bool NETWORK_DOES_NETWORK_ID_EXIST(NetworkID networkId);
bool NETWORK_REQUEST_CONTROL_OF_ENTITY(Entity entity);
bool NETWORK_HAS_CONTROL_OF_ENTITY(Entity entity);
NetworkID VEH_TO_NET(Vehicle vehicle);
Vehicle NET_TO_VEH(NetworkID networkId);
bool NETWORK_IS_PLAYER_TALKING(Player player);
bool NETWORK_IS_PLAYER_MUTED_BY_ME(Player player);
void SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NetworkID networkId, bool existsOnAllMachines);
bool CAN_REGISTER_MISSION_VEHICLES(DWORD unk1 = 1);
/// DECORATOR
void _0xDB718B21(DWORD unk1, char* unk2, DWORD unk3);