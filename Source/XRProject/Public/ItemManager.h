// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputStream.h"
#include "EngineMinimal.h"
#include "CustomSingleton.h"
#include "ItemEquipment.h"
#include "ItemConsumption.h"
#include "ItemETC.h"
#include "Engine/DataTable.h"
#include "PlayerCharacter.h"
#include "XRAssetMgr.h"
#include "AccountManager.h"
#include "UObject/NoExportTypes.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UItemManager : public UObject
{
	GENERATED_BODY()

public:
	UItemManager();
	~UItemManager();
public:
	class UDataTable* EquipmentItemDataTable;
	class UDataTable* ConsumptionItemDataTable;
	class UDataTable* ETCItemDataTable;
public:
	/*���� : Type�� ���� �������, �Һ����� ���̾�� �ϳ� Xrproject.h�� �ǵ���� �ϹǷ�
	�����Ͽ� ������ ��
	�Ϲ������� �ٵ� Ÿ���� ������ ����� �����ٵ�.. ������ Ÿ�Ե� �������� ������ ���,�Һ�,��Ÿ ���̺��� ���������Ƿ�
	���������� ����� �� ����
	*/
	void GetIcon(UTexture2D* OutTexture, int ID);
	TOptional<UItem*> CreateItem(class InputStream& input);
	TOptional<UItem*> GetItemFromId(EItemType Type, int32 ID);
	void BuildItem(EItemType Type, int32 ID, UWorld* World, APlayerCharacter* TargetCharacter = nullptr);
	void LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath, UItem* Item, APlayerCharacter* Character);
	
};





USTRUCT(BlueprintType)
struct FEquipmentTableResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 IconID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 MaleMeshId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 FemaleMeshId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 SubType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 ATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 DEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 STR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 DEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 INT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredSTR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredDEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredINT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ToolTip;
};

USTRUCT(BlueprintType)
struct FConsumptionTableResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 IconID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RecoveryHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RecoveryStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ToolTip;
};

USTRUCT(BlueprintType)
struct FETCTableResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 IconID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ToolTip;
};