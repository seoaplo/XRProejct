// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "Animation/AnimInstance.h"

#include "NonePlayerCharacterAnimInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillFireDelegate);

USTRUCT(Atomic, BlueprintType)
struct FNpcSkill
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAnimMontage* AttackAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 SkilID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CoolTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackRange;
};

/*
//�ۼ��� ������
*/
UCLASS()
class XRPROJECT_API UNonePlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()



public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
private:


	//����� ����//

public:
	FOnSkillFireDelegate OnSkillFire;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		float CurrentSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		bool bInBattle;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		TArray<FNpcSkill> NpcAttackMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NpcTakeDamageMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NpcDeadMontage;
	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetDeadMontage() { return NpcDeadMontage; }


	
	UFUNCTION()
		void AnimNotify_AttackColliOn();

	UFUNCTION()
		void AnimNotify_AttackColliOff();


	UFUNCTION()
		void AnimNotify_FireSkill();


protected:

private:



};
