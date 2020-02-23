// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerSkillTimeDuration.generated.h"

class UXRGameInstance;

const float kRemainTimeDecreaseFrequency = 1.0f;

/**
 * 
 */
UCLASS()
class XRPROJECT_API UPlayerSkillTimeDuration : public UObject
{
	GENERATED_BODY()
	

public:
	UPlayerSkillTimeDuration();
	virtual ~UPlayerSkillTimeDuration();
	UXRGameInstance* GI;

private:
	UPROPERTY()
		FTimerHandle TimerHandle;
	UPROPERTY()
		int32 SkillID;

	float SkillRemainTime; //��ų max ���ӽð�
	float CurrentRemainTime; //���� ���������� ���� ���ӽð�
	bool bIsRemainingTime; //���� ���ӽð��� ���Ҵ���?
public:
	UFUNCTION()
		void SetTD(int32 TargetSkillID, float TimeDuration, UXRGameInstance* GI); //Set Time Duration
	UFUNCTION()
		void RepeatingFunc();
	UFUNCTION()
		void SetTimer();

	int32 GetID();
	bool GetbSkillRemainTime();
	float GetRemainTime();
};
