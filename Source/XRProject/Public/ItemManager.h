// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "CustomSingleton.h"
#include "ItemEquipment.h"
#include "ItemConsumption.h"
#include "ItemETC.h"
#include "Engine/DataTable.h"
#include "PlayerCharacter.h"
#include "XRAssetMgr.h"
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
	FEquipmentDefaultInfo* EquipmentItemData;

	//�Һ���

	//��Ÿ��
private:
	APlayerCharacter* CurrentPlayerCharacter;
	UItem* RetItem;
	int32 CurrentItemId;
	bool bItemLoaded;
public:
	/*���� : Type�� ���� �������, �Һ����� ���̾�� �ϳ� Xrproject.h�� �ǵ���� �ϹǷ�
	�����Ͽ� ������ ��
	�Ϲ������� �ٵ� Ÿ���� ������ ����� �����ٵ�.. ������ Ÿ�Ե� �������� ������ ���,�Һ�,��Ÿ ���̺��� ���������Ƿ�
	���������� ����� �� ����
	*/
	TOptional<UItem*> GetItemFromId(EItemType Type, int32 ID);
	void BuildItem(EItemType Type, int32 ID, UWorld* World);
	bool SetPlayerCharacter(APlayerCharacter* Character);
	void LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath,UItem* testitem);
	
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