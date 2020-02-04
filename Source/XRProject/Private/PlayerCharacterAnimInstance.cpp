// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "AccountManager.h"
#include "PlayerCharacter.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	CharacterSpeed = 0.0f;
	bIsAttack = false;
	bIsRolling = false;
	bIsSprint = false;
	bIsCharacterDead = false;
	bIsHit = false;


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontage.AttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE_ONLYPLAY(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontageOnlyPlay.AttackMontageOnlyPlay'"));
	if (ATTACK_MONTAGE_ONLYPLAY.Succeeded())
	{
		AttackMontageOnlyPlay = ATTACK_MONTAGE_ONLYPLAY.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/HitMontage.HitMontage'"));
	if (HIT_MONTAGE.Succeeded())
		HitMontage = HIT_MONTAGE.Object;




}

UPlayerCharacterAnimInstance::~UPlayerCharacterAnimInstance()
{

}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	MyCharacter = AccountManager::GetInstance().GetCurrentPlayerCharacter();

	if (MyCharacter)
	{
		//CharacterSpeed = MyCharacter->GetCharacterMovement()->Velocity.Size(); //���� : velocity crash
		bIsMove = MyCharacter->bIsMove;
		bIsRolling = MyCharacter->bIsRolling;
		bIsSprint = MyCharacter->bIsSprint;
		bIsCharacterDead = MyCharacter->bIsCharacterDead;
		bIsHit = MyCharacter->bIsHit;
	}

}

void UPlayerCharacterAnimInstance::AnimNotify_CheckNextComboValid()
{
	if (Delegate_CheckNextCombo.IsBound())
		Delegate_CheckNextCombo.Execute();

}

void UPlayerCharacterAnimInstance::AnimNotify_RollingEnd()
{
	if (IsValid(MyCharacter))
	{
		MyCharacter->bIsRolling = false;
	}
}

void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	float ret = Montage_Play(AttackMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::PlayAttackOnlyPlayMontage()
{
	float ret = Montage_Play(AttackMontageOnlyPlay, 1.f);
}

void UPlayerCharacterAnimInstance::PlayHitMontage()
{
	float ret = Montage_Play(HitMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::StopAttackMontage()
{

}

void UPlayerCharacterAnimInstance::JumpToComboMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("combo%d"), Section)));
}

void UPlayerCharacterAnimInstance::JumpToReloadMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("combo%dR"), Section)));
}
