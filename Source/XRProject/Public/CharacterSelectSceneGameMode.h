// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <string>
#include "UserCharacter.h"
#include "CharacterSelectWidget.h"
#include "CoreMinimal.h"
#include "XRProjectGameModeBase.h"
#include "CharacterSelectSceneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API ACharacterSelectSceneGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()
public:
	ACharacterSelectSceneGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		TSubclassOf<UCharacterSelectWidget> LoginWidget;
	UPROPERTY()
		UCharacterSelectWidget* CurrentWidget;
public:
	std::vector<AUserCharacter> CharacterList; // ���� ������ ������ ĳ������ ����Ʈ(��Ŷ���� �޾ƿ;� �Ѵ�.)
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
public:
	void HandleCharacterCreateFail(class InputStream& input);
	void HandleCharacterList(class InputStream& input);
	void HandleCharacterSlot(class InputStream& input);
};