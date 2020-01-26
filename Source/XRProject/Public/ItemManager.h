// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "EngineMinimal.h"
#include "CustomSingleton.h"
#include "ItemEquipment.h"
#include "ItemConsumption.h"
#include "ItemETC.h"
#include "Engine/DataTable.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"
#include "XRAssetMgr.h"
/**
 * 
 */
class XRPROJECT_API ItemManager : public CustomSingleton<ItemManager>
{
public:
	friend class CustomSingleton<ItemManager>;
private:
	ItemManager();

public:
	~ItemManager();

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
	TOptional<UItem*> GetItemFromId(int32 Type, int32 ID);
	void BuildItem(int32 ID, UWorld* World);
	bool SetPlayerCharacter(APlayerCharacter* Character);
	void LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath);
};

#define ITEM_MGR ItemManager::GetInstance()