// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "XRGameInstance.h"
#include "XRPlayerController.h"

bool UMapManager::Init()
{
	PlayerID = -1;
	LevelID = -1;
	Spawn_Character.Unbind();
	Delete_Character.Unbind();

	MapList.Add(100, TEXT("LEVEL_Village"));
	MapList.Add(111, TEXT("LEVEL_Zone_1"));
	MapList.Add(112, TEXT("LEVEL_Zone_2"));
	MapList.Add(113, TEXT("LEVLE_Boss"));

	return true;
}
bool UMapManager::Clear()
{

	for (auto& Character : CharacterList)
	{
		Character.Value->Destroy();
	}
	for (auto& Monster : MonsterList)
	{
		Monster.Value->Destroy();
	}

	CharacterList.Reset();
	MonsterList.Reset();

	CharacterDataList.clear();
	MonsterDataList.clear();

	return true;
}

// �ʿ� ����


void UMapManager::ReadMapDataFromServer(InputStream& Input)
{
	int32_t characterlistsize = 0;
	int32_t monsterlistsize = 0;

	Input >> LevelID;

	Input >> characterlistsize;
	CharacterDataList.resize(characterlistsize + 1);
	ReadPlayerFromServer(Input);

	Input >> monsterlistsize;
	MonsterDataList.resize(monsterlistsize);
	ReadMosnterFromServer(Input);
}

void UMapManager::ReadMosnterFromServer(InputStream& Input)
{

	for (auto& CurrentData : MonsterDataList)
	{
		Input >> CurrentData.MonsterID;
		Input >> CurrentData.ObjectID;
		Input >> CurrentData.Location;
		Input >> CurrentData.Rotator;
		
		CurrentData.Rotator = FRotator(0, 0, 0);

		CurrentData.Current_HP = Input.ReadFloat32();
		CurrentData.Max_HP = Input.ReadFloat32();
		CurrentData.Attack_Min = Input.ReadFloat32();
		CurrentData.Attack_Max = Input.ReadFloat32();
		CurrentData.Attack_Range = Input.ReadFloat32();
		CurrentData.Attack_Speed = Input.ReadFloat32();
		CurrentData.Speed = Input.ReadFloat32();
		CurrentData.Defence = Input.ReadFloat32();
	}
}

void UMapManager::ReadPlayerFromServer(InputStream& Input)
{
	for (int iCount = 0; iCount < CharacterDataList.size() - 1; iCount++)
	{
		CharacterData& CurrentData = CharacterDataList[iCount];

		Input >> CurrentData.ObjectID;
		Input >> CurrentData.Location;
		Input >> CurrentData.Rotator;

		CurrentData.Current_HP = Input.ReadFloat32();
		CurrentData.Max_HP = Input.ReadFloat32();
		CurrentData.Attack_Min = Input.ReadFloat32();
		CurrentData.Attack_Max = Input.ReadFloat32();
		CurrentData.Attack_Range = Input.ReadFloat32();
		CurrentData.Attack_Speed = Input.ReadFloat32();
		CurrentData.Speed = Input.ReadFloat32();
		CurrentData.Defence = Input.ReadFloat32();

		CurrentData.Name = Input.ReadCString();
		CurrentData.Level = Input.ReadInt32();
		CurrentData.Gender = Input.ReadInt32();
		CurrentData.FaceID = Input.ReadInt32();
		CurrentData.HairID = Input.ReadInt32();
		CurrentData.STR = Input.ReadInt32();
		CurrentData.DEX = Input.ReadInt32();
		CurrentData.INT = Input.ReadInt32();
		CurrentData.CurrentStamina = Input.ReadFloat32();
		CurrentData.MaxStamina = Input.ReadFloat32();

		int EquipmentSize = 4;
		for (int iCount = 0; iCount < EquipmentSize; iCount++)
		{
			Equipment& CurrentEquip = CurrentData.EquipArray[iCount];
			CurrentEquip.Type = Input.ReadInt32();
			if (CurrentEquip.Type)
			{
				if (CurrentEquip.Type == 3)
				{
					CurrentEquip.ID = Input.ReadInt32();
					CurrentEquip.AddATK = Input.ReadInt32();
					CurrentEquip.AddDEF = Input.ReadInt32();
					CurrentEquip.AddSTR = Input.ReadInt32();
					CurrentEquip.AddDex = Input.ReadInt32();
					CurrentEquip.AddInt = Input.ReadInt32();
				}
				CurrentEquip.Count = Input.ReadInt32();
			}
		}
	}
}


void UMapManager::ReadPossesPlayerFromServer(InputStream& Input)
{

	CharacterData& CurrentData = CharacterDataList[CharacterDataList.size() - 1];

	Input >> CurrentData.ObjectID;
	Input >> CurrentData.Location;
	Input >> CurrentData.Rotator;

	CurrentData.Current_HP = Input.ReadFloat32();
	CurrentData.Max_HP = Input.ReadFloat32();
	CurrentData.Attack_Min = Input.ReadFloat32();
	CurrentData.Attack_Max = Input.ReadFloat32();
	CurrentData.Attack_Range = Input.ReadFloat32();
	CurrentData.Attack_Speed = Input.ReadFloat32();
	CurrentData.Speed = Input.ReadFloat32();
	CurrentData.Defence = Input.ReadFloat32();

	CurrentData.Name = Input.ReadCString();
	CurrentData.Level = Input.ReadInt32();
	CurrentData.Gender = Input.ReadInt32();
	CurrentData.FaceID = Input.ReadInt32();
	CurrentData.HairID = Input.ReadInt32();
	CurrentData.STR = Input.ReadInt32();
	CurrentData.DEX = Input.ReadInt32();
	CurrentData.INT = Input.ReadInt32();
	CurrentData.CurrentStamina = Input.ReadFloat32();
	CurrentData.MaxStamina = Input.ReadFloat32();

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		Equipment& CurrentEquip = CurrentData.EquipArray[EquipmentSize];
		CurrentEquip.Type = Input.ReadInt32();
		if (CurrentEquip.Type)
		{
			if (CurrentEquip.Type == 3)
			{
				CurrentEquip.ID = Input.ReadInt32();
				CurrentEquip.AddATK = Input.ReadInt32();
				CurrentEquip.AddDEF = Input.ReadInt32();
				CurrentEquip.AddSTR = Input.ReadInt32();
				CurrentEquip.AddDex = Input.ReadInt32();
				CurrentEquip.AddInt = Input.ReadInt32();
			}
			CurrentEquip.Count = Input.ReadInt32();
		}
	}
	PlayerID = CurrentData.ObjectID;
}
bool UMapManager::ReadPlayerDeleteFromServer(InputStream& Input)
{
	Delete_Character.ExecuteIfBound();
	return true;
}

APlayerCharacter* UMapManager::FindPlayer(int64_t objectid)
{
	APlayerCharacter* FindedPlayer = CharacterList.FindRef(objectid);
	return FindedPlayer;
}
ANonePlayerCharacter* UMapManager::FindMonster(int64_t ObjectID)
{
	ANonePlayerCharacter* FindedMonster = MonsterList.FindRef(ObjectID);
	return FindedMonster;
}
bool UMapManager::ReadPlayerSpawnFromServer(InputStream& Input)
{
	CharacterData CurrentData;
	Input >> CurrentData.ObjectID;
	Input >> CurrentData.Location;
	Input >> CurrentData.Rotator;

	CurrentData.Current_HP = Input.ReadFloat32();
	CurrentData.Max_HP = Input.ReadFloat32();
	CurrentData.Attack_Min = Input.ReadFloat32();
	CurrentData.Attack_Max = Input.ReadFloat32();
	CurrentData.Attack_Range = Input.ReadFloat32();
	CurrentData.Attack_Speed = Input.ReadFloat32();
	CurrentData.Speed = Input.ReadFloat32();
	CurrentData.Defence = Input.ReadFloat32();

	CurrentData.Name = Input.ReadCString();
	CurrentData.Level = Input.ReadInt32();
	CurrentData.Gender = Input.ReadInt32();
	CurrentData.FaceID = Input.ReadInt32();
	CurrentData.HairID = Input.ReadInt32();
	CurrentData.STR = Input.ReadInt32();
	CurrentData.DEX = Input.ReadInt32();
	CurrentData.INT = Input.ReadInt32();
	CurrentData.CurrentStamina = Input.ReadFloat32();
	CurrentData.MaxStamina = Input.ReadFloat32();

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		Equipment& CurrentEquip = CurrentData.EquipArray[EquipmentSize];
		CurrentEquip.Type = Input.ReadInt32();
		if (CurrentEquip.Type)
		{
			if (CurrentEquip.Type == 3)
			{
				CurrentEquip.ID = Input.ReadInt32();
				CurrentEquip.AddATK = Input.ReadInt32();
				CurrentEquip.AddDEF = Input.ReadInt32();
				CurrentEquip.AddSTR = Input.ReadInt32();
				CurrentEquip.AddDex = Input.ReadInt32();
				CurrentEquip.AddInt = Input.ReadInt32();
			}
			CurrentEquip.Count = Input.ReadInt32();
		}
	}
	CharacterDataList.push_back(CurrentData);
	Spawn_Character.ExecuteIfBound();
	return true;
}

bool UMapManager::OpenMap(UWorld* World)
{
	if (World == nullptr) return false;
	FName* LevelName = MapList.Find(LevelID);
	if (LevelName == nullptr) return false;

	UGameplayStatics::OpenLevel(World, *LevelName);
	return true;
}
bool UMapManager::PlayerListSpawn(UWorld* World)
{
	if (World == nullptr) return false;
	for (auto& CurrentData : CharacterDataList)
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* actor =
			World->SpawnActor
			(APlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator, Param);
		APlayerCharacter* Player = Cast<APlayerCharacter>(actor); 
		auto GameInstance = Cast <UXRGameInstance>(Player->GetGameInstance());

		Player->ObjectID = CurrentData.ObjectID;
		Player->PlayerStatComp->SetCurrentHP(CurrentData.Current_HP);
		Player->PlayerStatComp->SetMaxHP(CurrentData.Max_HP);
		Player->PlayerStatComp->SetAttack_Min(CurrentData.Attack_Min);
		Player->PlayerStatComp->SetAttack_Max(CurrentData.Attack_Max);
		Player->PlayerStatComp->SetAttack_Range(CurrentData.Attack_Range);
		Player->PlayerStatComp->SetAttack_Speed(CurrentData.Attack_Speed);
		Player->PlayerStatComp->SetSpeed(CurrentData.Speed);
		Player->PlayerStatComp->SetDefence(CurrentData.Defence);
		Player->PlayerStatComp->SetLevel(CurrentData.Level);
		Player->PlayerStatComp->SetGender(CurrentData.Gender);
		Player->PlayerStatComp->SetSTR(CurrentData.STR);
		Player->PlayerStatComp->SetDEX(CurrentData.DEX);
		Player->PlayerStatComp->SetINT(CurrentData.INT);
		Player->PlayerStatComp->SetCurrentStamina(CurrentData.CurrentStamina);
		Player->PlayerStatComp->SetMaxStamina(CurrentData.MaxStamina);

		for (int ii = 0; ii < CurrentData.kEquipmentArraySize; ii++)
		{
			/*�Ǹ��� �� */
			if (CurrentData.EquipArray[ii].ID == -1)
			{
				//FEquipmentTableResource* EqTable;

				const int32 kMalePrimaryBody = 130;
				const int32 kMalePrimaryHand = 140;
				const int32 kMalePrimaryLeg = 150;
				const int32 kMalePrimaryWeapon = 3300001;

				switch (ii)
				{
					case 0:
						Player->ChangePartsById(EPartsType::NUDEBODY, kMalePrimaryBody);
						break;
					case 1:
						Player->ChangePartsById(EPartsType::NUDEHAND, kMalePrimaryHand);
						break;
					case 2:
						Player->ChangePartsById(EPartsType::NUDELEG, kMalePrimaryLeg);
						break;
					case 3:
						GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, kMalePrimaryWeapon,
							Player->GetWorld(), Player);
						break;
				}
			}
			else
				GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, CurrentData.EquipArray[ii].ID, World, Player);
		}

		if (Player)
		{
			APlayerCharacter** CheckPlayer = CharacterList.Find(CurrentData.ObjectID);
			if (CheckPlayer != nullptr)
			{
				Player->Destroy();
				continue;
			}
			if (Player->PlayerStatComp == nullptr) continue;
	
			//Player->PlayerStatComp->GetStatDataFromServer(Input);
			CharacterList.Add(CurrentData.ObjectID, Player);
		}
	}
	CharacterDataList.clear();
	return true;
}
bool UMapManager::PossessPlayer(UWorld* World)
{
	APlayerCharacter* Player = CharacterList.FindRef(PlayerID);
	if (Player == nullptr) return false;
	AXRPlayerController* MyPlayerController = Cast<AXRPlayerController>(World->GetPlayerControllerIterator()->Get());
	if (MyPlayerController == nullptr) return false;
	MyPlayerController->Possess(Player);
	PlayerCharacter = Player;
	return true;
}
bool UMapManager::MonsterListSpawn(UWorld* World)
{
	if (World == nullptr) return false;

	for (auto& CurrentData : MonsterDataList)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* actor =
			World->SpawnActor
			(ANonePlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator, param);

		ANonePlayerCharacter* Monster = Cast<ANonePlayerCharacter>(actor);
		if (Monster)
		{
			ANonePlayerCharacter** CheckMonster = MonsterList.Find(CurrentData.ObjectID);
			if (CheckMonster != nullptr)
			{
				Monster->Destroy();
				continue;
			}
			//if (Monster->PlayerStatComp == nullptr) return false;
			//Monster->PlayerStatComp->GetStatDataFromServer(Input);
			Monster->NpcLoadStart(CurrentData.MonsterID);
			Monster->ObjectID = CurrentData.ObjectID;
			MonsterList.Add(CurrentData.ObjectID, Monster);
		}
	}
	MonsterDataList.clear();
	return true;
}

bool UMapManager::RemotePlayerSpawn(UWorld* world)
{
	return PlayerListSpawn(world);
}
bool UMapManager::DeleteRemotePlayer(UWorld* World)
{
	return false;
}