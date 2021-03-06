// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "InventoryWidget.h"
#include "XRGameInstance.h"
#include "XRProject.h"

UItemManager::UItemManager()
{

	FString EquipmentDataPath = TEXT("DataTable'/Game/Resources/DataTable/EquipTable.EquipTable'");
	FString ConsumptionDataPath = TEXT("DataTable'/Game/Resources/DataTable/ConsumptionItemTable.ConsumptionItemTable'");
	FString ETCDataPath = TEXT("DataTable'/Game/Resources/DataTable/ETCItemTable.ETCItemTable'");
	FString PartsDataPath = TEXT("DataTable'/Game/Resources/DataTable/PartsTable.PartsTable'");

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIPITEM(*EquipmentDataPath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CONSUMPTIONITEM(*ConsumptionDataPath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ETCITEM(*ETCDataPath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PARTS(*PartsDataPath);



	if (DT_EQUIPITEM.Succeeded())		EquipmentItemDataTable = DT_EQUIPITEM.Object;
	if (DT_CONSUMPTIONITEM.Succeeded())	ConsumptionItemDataTable = DT_CONSUMPTIONITEM.Object;
	if (DT_ETCITEM.Succeeded())			ETCItemDataTable = DT_ETCITEM.Object;
	if (DT_PARTS.Succeeded())			PartsDataTable = DT_PARTS.Object;

}

UItemManager::~UItemManager()
{
}

TOptional<UItem*> UItemManager::CreateItem(InputStream & input)
{
	EItemType Type = (EItemType)input.ReadInt32();
	if (EquipmentItemDataTable == nullptr)
		check(false);
	switch (Type)
	{
	case EItemType::NONE:
	{
		break;
	}
	case EItemType::ETC:
	{
		int ID = input.ReadInt32();
		int Count = input.ReadInt32();
		FETCTableResource* Table = ETCItemDataTable->FindRow<FETCTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));
		if (Table == nullptr)
			check(false);
		UItemETC* Item = NewObject<UItemETC>();
		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.IconResourceID = Table->IconID;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		Item->SetCount(Count);
		Item->ItemType = EItemType::ETC;
		Item->AddToRoot();
		return Item;
		break;
	}
	case EItemType::CONSUMPTION:
	{
		int ID = input.ReadInt32();
		int Count = input.ReadInt32();
		FConsumptionTableResource* Table = ConsumptionItemDataTable->FindRow<FConsumptionTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));
		if (Table == nullptr)
			check(false);
		UItemConsumption* Item = NewObject<UItemConsumption>();
		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.IconResourceID = Table->IconID;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.RecoveryHP = Table->RecoveryHP;
		Item->DefaultInfo.RecoveryStamina = Table->RecoveryStamina;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		Item->SetCount(Count);
		Item->ItemType = EItemType::CONSUMPTION;
		Item->AddToRoot();
		return Item;
		break;
	}
	case EItemType::EQUIPMENT:
	{
		int ID = input.ReadInt32();
		FEquipmentTableResource* Table = EquipmentItemDataTable->FindRow<FEquipmentTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));

		if (Table == nullptr)
			check(false);

		UItemEquipment* Item = NewObject<UItemEquipment>();

		Item->AddInfo.AddATK = input.ReadInt32();
		Item->AddInfo.AddDEF = input.ReadInt32();
		Item->AddInfo.AddSTR = input.ReadInt32();
		Item->AddInfo.AddDEX = input.ReadInt32();
		Item->AddInfo.AddINT = input.ReadInt32();

		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshId;
		Item->DefaultInfo.FemaleMeshResourceID = Table->FemaleMeshId;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.IconResourceID = Table->IconID;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.SubType = Table->SubType;
		Item->DefaultInfo.ReqLEV = Table->RequiredLevel;
		Item->DefaultInfo.ATK = Table->ATK;
		Item->DefaultInfo.DEF = Table->DEF;
		Item->DefaultInfo.STR = Table->STR;
		Item->DefaultInfo.DEX = Table->DEX;
		Item->DefaultInfo.INT = Table->INT;
		Item->DefaultInfo.ReqSTR = Table->RequiredSTR;
		Item->DefaultInfo.ReqDEX = Table->RequiredDEX;
		Item->DefaultInfo.ReqINT = Table->RequiredINT;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		Item->ItemType = EItemType::EQUIPMENT;
		Item->AddToRoot();
		int Count = input.ReadInt32();
		return Item;
		break;
	}
	default:
		break;
	}
	return nullptr;
}

void UItemManager::GetIcon(UImage* Target, int ID)
{
	FSoftObjectPath AssetPath;
	auto GI = Cast<UXRGameInstance>(Target->GetGameInstance());
	AssetPath = GI->GetXRAssetMgr()->FindResourceFromDataTable(ID);
	FStreamableDelegate ResultCallback;
	ResultCallback = FStreamableDelegate::CreateLambda([AssetPath, Target, this]()
	{
		TSoftObjectPtr<UTexture2D> Loaded(AssetPath);
		Target->SetBrushFromTexture(Loaded.Get());
		XRLOG(Warning, TEXT("IconLoadComplete"));
	});
	GI->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);
}

TOptional<UItem*> UItemManager::GetItemFromId(EItemType Type, int32 ID)
{
	if (EquipmentItemDataTable == nullptr)
		check(false);

	switch (Type)
	{
	case EItemType::EQUIPMENT:
	{
		FEquipmentTableResource* Table = EquipmentItemDataTable->FindRow<FEquipmentTableResource>(FName(*(FString::FromInt(ID))), TEXT(""));

		if (Table == nullptr) //�Ǹ�
			return nullptr;

		UItemEquipment* Item = NewObject<UItemEquipment>();

		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshId;
		Item->DefaultInfo.FemaleMeshResourceID = Table->FemaleMeshId;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.IconResourceID = Table->IconID;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.SubType = Table->SubType;
		Item->DefaultInfo.ReqLEV = Table->RequiredLevel;
		Item->DefaultInfo.ATK = Table->ATK;
		Item->DefaultInfo.DEF = Table->DEF;
		Item->DefaultInfo.STR = Table->STR;
		Item->DefaultInfo.DEX = Table->DEX;
		Item->DefaultInfo.INT = Table->INT;
		Item->DefaultInfo.ReqSTR = Table->RequiredSTR;
		Item->DefaultInfo.ReqDEX = Table->RequiredDEX;
		Item->DefaultInfo.ReqINT = Table->RequiredINT;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		Item->ItemType = EItemType::EQUIPMENT;
		Item->AddToRoot();
		return Item;
	}
	break;
	case EItemType::CONSUMPTION:
	{
		FConsumptionTableResource* Table = ConsumptionItemDataTable->FindRow<FConsumptionTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));
		if (Table == nullptr)
			check(false);
		UItemConsumption* Item = NewObject<UItemConsumption>();
		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.IconResourceID = Table->IconID;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.RecoveryHP = Table->RecoveryHP;
		Item->DefaultInfo.RecoveryStamina = Table->RecoveryStamina;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		Item->SetCount(0);
		Item->ItemType = EItemType::CONSUMPTION;
		Item->AddToRoot();
		return Item;
	}
	break;
	case EItemType::ETC:
		break;
	default:
		break;
	}
	return nullptr;
}

void UItemManager::BuildItem(EItemType Type, int32 ID, UWorld* World, APlayerCharacter* TargetCharacter)
{
	TOptional<UItem*> ItemOptional = GetItemFromId(Type, ID);
	//is valid


	if (ItemOptional == nullptr)
		return;

	UItem* RetItem = nullptr;

	if (ItemOptional.IsSet())
	{
		RetItem = ItemOptional.GetValue();
	}

	FSoftObjectPath AssetPath = nullptr;
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(World));

	if (RetItem->GetItemType() == EItemType::EQUIPMENT)
	{
		UItemEquipment* EquipmentItem = Cast<UItemEquipment>(RetItem);
		if (TargetCharacter->bIsMale)
			AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.MaleMeshResourceID);
		else
			AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.FemaleMeshResourceID);
	}

	TargetCharacter->PseudoChangeEquipmentWithoutMesh(RetItem);

	FStreamableDelegate AssetLoadDelegate;
	AssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &UItemManager::LoadItemMeshAssetComplete,
		AssetPath, RetItem, TargetCharacter);

	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, AssetLoadDelegate);
}

void UItemManager::BuildItem(UItem* Item, UWorld* World, APlayerCharacter* TargetCharacter)
{
	UItem* RetItem = Item;

	FSoftObjectPath AssetPath = nullptr;
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(World));

	if (RetItem->GetItemType() == EItemType::EQUIPMENT)
	{
		UItemEquipment* EquipmentItem = Cast<UItemEquipment>(RetItem);
		if (TargetCharacter->bIsMale)
			AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.MaleMeshResourceID);
		else
			AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.FemaleMeshResourceID);
	}
	
	TargetCharacter->PseudoChangeEquipmentWithoutMesh(RetItem);

	FStreamableDelegate AssetLoadDelegate;
	AssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &UItemManager::LoadItemMeshAssetComplete,
		AssetPath, RetItem, TargetCharacter);

	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, AssetLoadDelegate);
}


void UItemManager::LoadItemMeshAssetComplete(FSoftObjectPath AssetPath, UItem* Item, APlayerCharacter* Character)
{
	const int32 kWeaponTypeNumber = 3;
	UItemEquipment* EquipItem = nullptr;
	EquipItem = Cast<UItemEquipment>(Item);

	if (EquipItem->DefaultInfo.Type != kWeaponTypeNumber)
	{
		TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
		
		Character->ChangeEquipment(Item, LoadedMesh.Get());
	}
	else
	{
		TSoftObjectPtr<UStaticMesh> LoadedMesh(AssetPath);

		Character->ChangeEquipment(Item, LoadedMesh.Get());
	}

}


