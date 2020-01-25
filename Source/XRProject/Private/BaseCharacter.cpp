// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Engine.h"
#include "TestGameInstance_AssetMgr.h"
#include "XRAssetMgr.h"
#include "EnermyAIController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStat"));

}

// Called when the game starts or when spawned 
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	StatComponent->OnHPZero.AddDynamic(this, &ABaseCharacter::OnDead);
	SetCharacterLoadState(ECharacterLoadState::PREINIT);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::SetCharacterLoadState(ECharacterLoadState NewLoadState)
{
	CurrentLoadState = NewLoadState;

	switch (CurrentLoadState)
	{
	case ECharacterLoadState::PREINIT:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : PREINIT")));
		SetCharacterLoadState(ECharacterLoadState::LOADING);
		break;
	}
	case ECharacterLoadState::LOADING:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : LOADING")));

		auto TestGameInstance = Cast<UTestGameInstance_AssetMgr>(GetGameInstance());

		FSoftObjectPath AssetPath =
			TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(1000001);
		FStreamableDelegate ResultCallback;
		ResultCallback.BindLambda([AssetPath, this]()
		{
			TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
			GetMesh()->SetSkeletalMesh(LoadedMesh.Get());
			XRLOG(Warning, TEXT("MeshLoadComplete"));
			SetCharacterLoadState(ECharacterLoadState::READY);
		});
		TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);


		FSoftObjectPath AssetPath2 =
			TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(3000001);
		FStreamableDelegate ResultCallback2;
		ResultCallback2.BindLambda([AssetPath2, this]()
		{
			//�ִϸ��̼� �������Ʈ�� ��쿣 TSoftClassPtr�� ��ȯ�Ѵ�
			TSoftClassPtr<UAnimInstance> LoadedAnim(AssetPath2);
			GetMesh()->SetAnimInstanceClass(LoadedAnim.Get());
			XRLOG(Warning, TEXT("AnimBlueprint Load Complete"));
			//*** Ŭ������ ����ϴ� ������ ��� ���� ��� ���� �ݵ��  _C�� �پ���Ѵ� ***///
		});
		TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath2, ResultCallback2);
		break;
	}
	case ECharacterLoadState::READY:
	{

		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : READY")));

		AIControllerClass = AEnermyAIController::StaticClass();
		GetController()->Possess(this);

		break;

	}
	default:
		break;
	}

}

void ABaseCharacter::SetCharacterLifeState(ECharacterLifeState NewLifeState)
{

	CurrentLifeState = NewLifeState;

	switch (CurrentLifeState)
	{
	case ECharacterLifeState::SPAWN:
	{
		GEngine->AddOnScreenDebugMessage(2, 50.0f, FColor::Blue, FString::Printf(TEXT("CurrentState : SPAWN")));
		
		break;
	}
	case ECharacterLifeState::PLAY:
	{	
		GEngine->AddOnScreenDebugMessage(2, 50.0f, FColor::Blue, FString::Printf(TEXT("CurrentState : Play")));
		
		break;
	}
	case ECharacterLifeState::DEAD:
	{	
		GEngine->AddOnScreenDebugMessage(2, 50.0f, FColor::Blue, FString::Printf(TEXT("CurrentState : Dead")));

		break;
	}
	default:
		break;
	}

}

