// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;


DECLARE_DELEGATE(FCheckNextComboValid)

/**
 *
 */
UCLASS()
class XRPROJECT_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerCharacterAnimInstance();
	virtual ~UPlayerCharacterAnimInstance();

public:
	FCheckNextComboValid Delegate_CheckNextCombo;


private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		float CharacterSpeed;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsAttack;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsMove;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsRolling;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsSprint;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsCharacterDead;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsHit;
	APlayerCharacter* MyCharacter;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontageOnlyPlay;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage;
	
	void SetOwnerCharacter(APlayerCharacter* MyCharacter);

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
		void AnimNotify_CheckNextComboValid();
	UFUNCTION()
		void AnimNotify_RollingEnd();

	void PlayAttackMontage();
	void PlayAttackOnlyPlayMontage();
	void StopAttackMontage();
	void PlayHitMontage();

	void JumpToComboMontageSection(int32 Section);
	void JumpToReloadMontageSection(int32 Section);


};
