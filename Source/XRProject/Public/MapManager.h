// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <list>
#include "NetworkOpcode.h"
#include "MapPacketHelper.h"
#include "NetworkManager.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"
#include "PotalTrigger.h"
#include "UObject/NoExportTypes.h"
#include "MapManager.generated.h"

/**
 * �ۼ��� : ���¼�
 */


struct PotalData
{
	int MapID = -1;
	FVector Location = FVector(0, 0, 0);
	float Distance = 0.0f;
};
DECLARE_DELEGATE(CharacterDataProcess)

UCLASS()
class XRPROJECT_API UMapManager : public UObject
{
	GENERATED_BODY()
public:
	UMapManager();
public:
	int64_t GetPlayerID() { return PlayerID; }
	APlayerCharacter* GetPlayer() { return PlayerCharacter; }
public:
	bool Init();
	bool Clear();

	// �ʿ� ����
	void ReadMapDataFromServer(InputStream& Input);
	void ReadPotalTriggerFromServer(InputStream& Input);
	void ReadPlayerFromServer(InputStream& Input);
	void ReadPossesPlayerFromServer(InputStream& Input);
	void ReadMosnterFromServer(InputStream& Input);
	bool ReadPlayerSpawnFromServer(InputStream& Input);
	bool ReadPlayerDeleteFromServer(InputStream& Input);

	// ������ ������ �۽�
	void SendChangeZoneFromClient();

	// ������Ʈ Ž��
	APlayerCharacter* FindPlayer(int64_t ObjectID);
	ANonePlayerCharacter* FindMonster(int64_t ObjectID);
	
	// ���� ����
	bool OpenMap(UWorld* World);
	// ���� �Լ���
	bool PotalTriggerSpawn(APotalTrigger* WorldPotalTrigger);
	bool PlayerListSpawn(UWorld* world);
	bool MonsterListSpawn(UWorld* world);
	//bool TriggerListSpawn(UWorld* world);
	bool RemotePlayerSpawn(UWorld* world);
	bool PossessPlayer(UWorld* World);
	// ��Ż ���� �Լ�
	void PotalInPlayer(AActor* Character);
	void PotalOutPlayer(AActor* Character);
	// ����
	bool DeleteRemotePlayer(UWorld* World);
public:
	CharacterDataProcess Spawn_Character;
	CharacterDataProcess Delete_Character;
private:
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
		APotalTrigger* MapPotal;

	PotalData MapPotalData;
	int32_t LevelID;
	int64_t PlayerID;
	std::vector<CharacterData> CharacterDataList;
	std::vector<MonsterData> MonsterDataList;

	TMap<int64_t, APlayerCharacter*> CharacterList;
	TMap<int64_t, ANonePlayerCharacter*> MonsterList;
	TMap<int32_t, FName> MapList;

	std::list<int64_t> InPotalPlayerList;
};
