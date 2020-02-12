// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "PlayerSkill.generated.h"

class APlayerCharacter;


/**
 * 
 */
UCLASS()
class XRPROJECT_API UPlayerSkill : public UObject
{
	GENERATED_BODY()

public:
	UPlayerSkill();
	~UPlayerSkill();


protected:
	int32			SkillID;
	FString			SkillName;
	float			DamageMagnification;
	int32			IsKnockBack;
	float			RequireStamina;
	float			CoolTime;
	int32			IconID;
	bool			bIsActiveMove;

public:
	virtual void Play(APlayerCharacter* Character);
	virtual bool End(APlayerCharacter* Character);
	virtual bool ConditionCheck(APlayerCharacter* Character);

public:
	void SetID(int32 ID) { SkillID = ID; }
	void SetSkillName(FString SkillName) { this->SkillName = SkillName; }
	void SetDamageMagnification(float Mag) { DamageMagnification = Mag; }
	void SetIsKnockBack(int32 IsKnockBack) { this->IsKnockBack = IsKnockBack; }
	void SetCoolTime(float CoolDown) { CoolTime = CoolDown; }
	void SetIconID(int32 ID) { IconID = ID; }
	void SetRequireStamina(float Stamina) { RequireStamina = Stamina; }

	int32 GetID() { return SkillID; }
	FString GetSkillName() { return SkillName; }
	float GetDamageMagnification() { return DamageMagnification; }
	int32 GetIsKnockBack() { return IsKnockBack; }
	float GetCoolTime() { return CoolTime; }
	int32 GetIconID() { return IconID; }
	float GetRequireStamina() { return RequireStamina; }
	inline bool GetIsActiveMove() { return bIsActiveMove; }

};


UCLASS()
class XRPROJECT_API USkill_GaiaCrush : public UPlayerSkill
{
	//ETC1 : ����Ÿ�(MoveDistance) || ETC2 : ��������(AffectRadius)
	GENERATED_BODY()

public:
	USkill_GaiaCrush();
	~USkill_GaiaCrush();

private:
	float MoveDistance; //���ߵ���� �̵��Ÿ�
	float AffectRadius; //������ ��ġ�� ���� ����

public:
	virtual void Play(APlayerCharacter* Character) override;
	virtual bool End(APlayerCharacter* Character) override;
	virtual bool ConditionCheck(APlayerCharacter* Character) override;
	//void Set();

public:
	void SetMoveDistance(float MoveDistance);
	void SetAffectRadius(float AffectRadius);
	float GetMoveDistance();
	float GetAffectRadius();
};


USTRUCT(BlueprintType)
struct FPlayerSkillResources : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString SkillName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString SkillNameKorean;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		float DamageMagnification;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		int32 IsKnockBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		float RequireStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		float CoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		int32 IconID;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString ToolTip;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString MoveDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString AffectRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString DMG_Increase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString DEF_Increase;
};
