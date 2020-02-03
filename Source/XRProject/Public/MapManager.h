// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "NetworkOpcode.h"
#include "MapPacketHelper.h"
#include "NetworkManager.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"
#include "UObject/NoExportTypes.h"
#include "MapManager.generated.h"

/**
 * �ۼ��� : ���¼�
 */
UCLASS()
class XRPROJECT_API UMapManager : public UObject
{
	GENERATED_BODY()
public:
	int64_t GetPlayerID() { return PlayerID; }
public:
	bool Init(UWorld* world, UNetworkManager& networkmanager);
	bool Clear();

	// �ʿ� ����
	void ReadMapDataFromServer(InputStream& input);
	bool SpawnPlayer(int64_t objectid, FVector position, FRotator rotator);
	bool PossessPlayer(int64_t objectid, FVector position, FRotator rotator);
	APlayerCharacter* FindPlayer(int64_t objectid);

	// ���� ���� �Լ�
	bool SpawnMonster(int32 MonsterID, int64_t objectid, FVector position, FRotator rotator);
private:
	int64_t PlayerID;
	UWorld* World;
	TMap<int64_t, APlayerCharacter*> CharacterList;
	TMap<int64_t, ANonePlayerCharacter*> MonsterList;
};
