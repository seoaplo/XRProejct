#pragma once
#include <cstdint>

enum class ENetworkCSOpcode : uint16_t
{
	// LoginServer
	kCreateAccountRequest = 0,
	kLoginRequest,

	// LobbyServer
	kLobbyConfirmRequest = 100,
	kCharacterListRequest,
	kCharacterCreateRequest,
	kCharacterDeleteRequest,
	kCharacterSelectionRequest,

	// ZoneServer
	kZoneConrifmRequest = 1000,
	kNotifyCurrentChrPosition,
	kNotifyMonsterAction,

	kInventoryUpdateRequest,
	kQuickSlotUseRequest,
	kEquipmentMountRequest,

	kMatchingRequest,
};


enum class ENetworkSCOpcode : uint16_t
{
	// LoginServer
	kCreateAccountResult = 0,
	kLoginResult,

	// LobbyServer
	kConfirmFailNotify = 100,
	kCharacterCreateNotify,
	kCharacterDeleteNotify,
	kCharacterListNotify,
	kMigrateZoneNotify,

	// ZoneServer
	kUserEnterTheMap = 1000,
	kSpawnCharacter,
	kSetMonsterController,

	kExitCharacter,
	kUpdateCharacterPosition,
	kUpdateMonsterAction,


	kSpawnMonster,


	kInventoryUpdate,
	kQuickSlotUpdate,
};