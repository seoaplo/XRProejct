// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "NetworkOpcode.h"
#include "PlayerCharacter.h"
#include "MapManager.h"
#include "XRProjectGameModeBase.h"
#include "InGameMainWidget.h"
#include "IngameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AIngameGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()
	
public:
	AIngameGameMode();
	virtual ~AIngameGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		TSubclassOf<UInGameMainWidget> MainWidget;
	UPROPERTY()
		UInGameMainWidget* CurrentWidget;

	UMapManager& GetMapMgr() 
	{
		return *MapManager; 
	}

public:
	/*���� �̻�� Dummy �ڵ��̸�, �ΰ��ӿ��� ĳ���Ͱ� ������ �� ��Ŷ�� ���� ���, HP���� �ʱ�ȭ�ϰ����Ҷ� �����ؼ� ����� ����. */
	void PlayerCharacterInitializeFromServer(InputStream& input);
	/*���� �̻�� Dummy �ڵ��̸�, �ΰ��ӿ��� �κ��丮 �������� Ŭ���Ͽ� ��� ��ü�� �� ����� ����(�����ʿ����)*/
	void PlayerCharacterItemChange(InputStream& input);
	/*Hair�� Face���� �������� �ƴ� �������� ��ü�� �� ����ϴ� �ڵ�. ��Ŷ�� ���� �����*/
	void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);
private:
	void SendConfirmRequest(); /*�� ���� Ȯ�ο�*/
	void HandleEnterZone(class InputStream& input); /*ù ����� �ʱ�ȭ ��Ŷ*/
	/* �Ʒ� 4���� �Լ��� HandleEnterZone���� ��� �� �Լ��� ���� ���� ���*/
	void ReadBaseCharacterInfo(class InputStream& input); /*ĳ���� ���� �б�*/
	void ReadInventoryInfo(class InputStream& input); /*�κ��丮 ���� �б�*/
	void ReadQuickSlot(class InputStream& input); /*�� ���� ���� �б�*/
	void ReadMapData(class InputStream& input); /*�� ������ ���� �б�*/
	void SpawnCharacterFromServer(class InputStream& input);
	void UpdateCharacterPosition(class InputStream& input);
public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
private:
	UPROPERTY()
	UMapManager* MapManager;
};

#define MapMgr AIngameGameMode::GetMapMgr()