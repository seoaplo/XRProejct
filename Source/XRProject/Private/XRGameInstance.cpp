// Fill out your copyright notice in the Description page of Project Settings.

#include "XRGameInstance.h"
#include "Inventory.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "Engine/Engine.h"
#include "EngineMinimal.h"

void UXRGameInstance::Init()
{
    NetworkManager = NewObject<UNetworkManager>();
    NetworkManager->StartThread();
    UNetworkManager::Instance = NetworkManager;
	XRAssetManager = NewObject<UXRAssetMgr>();
	ItemManager = NewObject<UItemManager>();
	MapManager = NewObject<UMapManager>();
	MapManager->Init();

	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUserEnterTheMap)->BindUObject(
		this, &UXRGameInstance::HandleEnterZone);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kSpawnCharacter)->BindUObject(
		this, &UXRGameInstance::SpawnCharacterFromServer);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateCharacterPosition)->BindUObject(
		this, &UXRGameInstance::UpdateCharacterPosition);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kSetMonsterController)->BindUObject(
		this, &UXRGameInstance::SetMonsterController);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateMonsterAction)->BindUObject(
		this, &UXRGameInstance::UpdateMonsterAction);


	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateCharacterPosition)->BindUObject(
		this, &UXRGameInstance::UpdateCharacterPosition);
		
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterAttack)->BindUObject(
		this, &UXRGameInstance::UpdateCharacterMotion);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kActorDamaged)->BindUObject(
		this, &UXRGameInstance::ActorDamaged);
	

	
}

void UXRGameInstance::Shutdown()
{
    NetworkManager->StopThread();
    UNetworkManager::Instance = nullptr;
}

void UXRGameInstance::ReqEnterZone()
{
	std::string Ip = AccountManager::GetInstance().GetInGameIP();
	int16 Port = AccountManager::GetInstance().GetInGamePort();
	GetNetMgr().Connect(Ip.c_str(), Port, std::bind(&UXRGameInstance::SendConfirmRequest, this));
}
void UXRGameInstance::SendConfirmRequest()
{
	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kZoneConrifmRequest);
	out.WriteCString(ID.c_str());
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

}

void UXRGameInstance::HandleEnterZone(InputStream & input)
{

	MapManager->Clear();
	MapManager->Init();

	ReadMapData(input);
	ReadBaseCharacterInfo(input);
	ReadInventoryInfo(input);
	ReadQuickSlot(input);
	MapManager->OpenMap(GetWorld());
}

void UXRGameInstance::ReadBaseCharacterInfo(InputStream & input)
{
	MapManager->ReadPossesPlayerFromServer(input);
}

void UXRGameInstance::ReadInventoryInfo(InputStream & input)
{
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
	UItemManager;
	int Glod = input.ReadInt32();
	Inventory::GetInstance().SetGold(Glod);
	for (int i = 0; i < Inventory::GetInstance().GetInventorySize(); i++)
	{
		UItem* newItem = GameInstance->ItemManager->CreateItem(input).GetValue();
		if (newItem)
		{
			Inventory::GetInstance().AddItem(newItem, i);
		}
	}
}

void UXRGameInstance::ReadQuickSlot(InputStream & input)
{
	for (int i = 0; i < 10; i++)
	{
		int Type = input.ReadInt8();
		if (Type)
		{
			int ID = input.ReadInt32();
		}
	}
}

void UXRGameInstance::ReadMapData(InputStream & input)
{
	MapManager->ReadMapDataFromServer(input);
}


void UXRGameInstance::SpawnCharacterFromServer(class InputStream& input)
{
	MapManager->ReadPlayerSpawnFromServer(input);
}
void UXRGameInstance::UpdateCharacterPosition(class InputStream& input)
{
	int64 Id = input.ReadInt64();
	int32 state = input.ReadInt32();

	FVector Location = input.ReadFVector();
	FRotator Rotation = input.ReadFRotator();

	APlayerCharacter* TargetPlayer = MapManager->FindPlayer(Id);
	if (TargetPlayer == nullptr)
	{
		XRLOG(Warning, TEXT("Player not found"));
		return;
	}

	AAIController*  aicon = Cast<AAIController>(TargetPlayer->GetController());
	if (aicon == nullptr)
	{
		XRLOG(Warning, TEXT("AICon not found"));
		return;
	}
	else aicon->MoveToLocation(Location, 2, false, false);

}


void UXRGameInstance::SetMonsterController(InputStream& input)
{
	bool IsMonsterController = input.ReadBool();
	IsSuper = IsMonsterController;

}

void UXRGameInstance::UpdateMonsterAction(InputStream& input)
{

	int64 ObjID = input.ReadInt64();
	ANonePlayerCharacter* npcCharacter = MapManager->FindMonster(ObjID);
	if (npcCharacter)
	{
		npcCharacter->ExcuteRecvNpcAction(input);
	}
}


void UXRGameInstance::UpdateCharacterMotion(InputStream & input)
{
	int64 ObjectID = input.ReadInt64();
	int32 MotionID = input.ReadInt32();
	FVector TargetLocation = input.ReadFVector();
	FRotator TargetRotation = input.ReadFRotator();



	APlayerCharacter* TargetPlayer = nullptr;
	TargetPlayer = MapManager->FindPlayer(ObjectID);

	if (TargetPlayer)
	{
		TargetPlayer->MyAnimInstance->PlayAttackOnlyPlayMontage();
		TargetPlayer->MyAnimInstance->JumpToComboMontageSection(MotionID);

		UE_LOG(LogTemp,
			Warning,
			TEXT("CharacterMotionDebug ID : %d, MotionID : %d, TargetVec : %f %f %f, TargetRot : %f %f %f"),
			ObjectID, MotionID, TargetLocation.X, TargetLocation.Y, TargetLocation.Z,
			TargetRotation.Yaw, TargetRotation.Pitch, TargetRotation.Roll);
	}


}

//
//void AIngameGameMode::SpawnCharacterFromServer(class InputStream& input)
//{
//	int64 Id = input.ReadInt64();
//	FVector Location = input.ReadFVector();
//	FRotator Rotation = input.ReadFRotator();
//
//	MapMgr.SpawnPlayer(Id, Location, Rotation);
//
//	float HP = input.ReadFloat32();
//	float MAXHP = input.ReadFloat32();
//	float AttackMin = input.ReadFloat32();
//	float AttackMax = input.ReadFloat32();
//	float AttackRange = input.ReadFloat32();
//	float AttackSpeed = input.ReadFloat32();
//	float Defense = input.ReadFloat32();
//	float Speed = input.ReadFloat32();
//	std::string Name = input.ReadCString();
//	int Level = input.ReadInt32();
//	int Gender = input.ReadInt32();
//	int FaceID = input.ReadInt32();
//	int HairID = input.ReadInt32();
//	int Str = input.ReadInt32();
//	int Dex = input.ReadInt32();
//	int Intel = input.ReadInt32();
//	float Stamina = input.ReadFloat32();
//	float MaxStamina = input.ReadFloat32();
//
//	APlayerCharacter* Character = nullptr;
//	Character = GetMapMgr().FindPlayer(Id);
//	if (Character == nullptr)
//		check(false);
//
//	int EquipmentSize = 4;
//	for (int i = 0; i < EquipmentSize; i++)
//	{
//		int Type = input.ReadInt32();
//		if (Type)
//		{
//			if (Type == 3)
//			{
//				int ID = input.ReadInt32();
//				int AddATK = input.ReadInt32();
//				int AddDEF = input.ReadInt32();
//				int AddSTR = input.ReadInt32();
//				int AddDex = input.ReadInt32();
//				int AddInt = input.ReadInt32();
//			}
//			int Count = input.ReadInt32();
//		}
//	}
//}
//
//
//void AIngameGameMode::SetMonsterController(InputStream& input)
//{
//	bool IsMonsterController = input.ReadBool();
//	IsSuper = IsMonsterController;
//}
//
//void AIngameGameMode::UpdateMonsterAction(InputStream& input)
//{
//	auto firstPlayer = Cast<AXRPlayerController>(GetWorld()->GetFirstPlayerController());
//	if (firstPlayer)
//	{
//		if (!firstPlayer->IsSpuer())
//		{
//			int64 ObjID = input.ReadInt64();
//			int32 ActionID = input.ReadInt32();
//			FVector Location = input.ReadFVector();
//			FRotator Rotator = input.ReadFRotator();
//
//
//			GEngine->AddOnScreenDebugMessage(112, 5.f, FColor::Blue, FString::Printf(TEXT("Recv MonsterUpdate  ObjectID: %s, ActionID: %s, Location : %s, Rotator: %s"),
//				*FString::FromInt(ObjID), *FString::FromInt(ActionID),
//				*Location.ToString(), *Rotator.ToString()));
//		}
//	}
//
//}

void UXRGameInstance::ActorDamaged(InputStream& input)
{

	int32 AttackerType = input.ReadInt32();
	int64 AttackerID = input.ReadInt64();
	int32 AttackedType = input.ReadInt32();
	int64 AttackedID = input.ReadInt64();
	int32 AttackActionID = input.ReadInt32();
	float AttackSetHp = input.ReadFloat32();

	if (AttackerType == 1)
	{

		ANonePlayerCharacter* AttackerMonster = MapManager->FindMonster(AttackerID);
		APlayerCharacter* AttackedCharacter = MapManager->FindPlayer(AttackedID);

		//AttackedCharacter->TakeDamage(AttackSetHp,FDamageEvent(),AttackerMonster->GetController(), AttackerMonster);
	}
	




}
