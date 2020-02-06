// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "NonePlayerCharacter.h"
#include "BossCharacter.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBossAttack
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* AttackAction;
	UPROPERTY(EditAnywhere)
		float AttackRange;
	UPROPERTY(EditAnywhere)
		float AttackAngle;

};

/**
 * �ۼ��� ������
 */
UCLASS()
class XRPROJECT_API ABossCharacter : public ANonePlayerCharacter
{
	GENERATED_BODY()


public:
	ABossCharacter();
protected:
private:




	//����� ����///
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FBossAttack> BossAttackList;

protected:
private:


};
