// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "NetworkOpcode.h"
#include "PlayerCharacter.h"
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

public:
	/*���� �̻�� Dummy �ڵ��̸�, �ΰ��ӿ��� ĳ���Ͱ� ������ �� ��Ŷ�� ���� ���, HP���� �ʱ�ȭ�ϰ����Ҷ� �����ؼ� ����� ����. */
	//void PlayerCharacterInitializeFromServer(InputStream& input);
	/*���� �̻�� Dummy �ڵ��̸�, �ΰ��ӿ��� �κ��丮 �������� Ŭ���Ͽ� ��� ��ü�� �� ����� ����(�����ʿ����)*/
	//void PlayerCharacterItemChange(InputStream& input);
	/*Hair�� Face���� �������� �ƴ� �������� ��ü�� �� ����ϴ� �ڵ�. ��Ŷ�� ���� �����*/
	//void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void SpawnRemotePlayer();
	void DeleteRemotePlayer();

	// widget data from server
	void NotifyMatchResult(class InputStream& input);
	void NotifyMatchCanceled(class InputStream& input);
private:
	FTimerHandle PacketExcuteTimerHandle;
};

