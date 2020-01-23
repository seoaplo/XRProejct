// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPawn_AssetMgr.h"
#include "TestGameInstance_AssetMgr.h"

// Sets default values
ATestPawn_AssetMgr::ATestPawn_AssetMgr()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TestMash = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	SetRootComponent(TestMash);
}

// Called when the game starts or when spawned
void ATestPawn_AssetMgr::BeginPlay()
{
	Super::BeginPlay();
	auto TestGameInstance = Cast<UTestGameInstance_AssetMgr>(GetGameInstance());
	//���� �ν��Ͻ��� ���� �Ŵ����� �������ִ� �ν��Ͻ��� ĳ����

	FSoftObjectPath AssetPath =
		TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(1000001);
	//���ҽ� ID�� ������ ���̺��� ���� ��θ� �����´�


	FStreamableDelegate ResultCallback;
	ResultCallback.BindUObject(this, &ATestPawn_AssetMgr::ResourceASyncLoadComplete, AssetPath);
	//�ش� ����� ������ �ε尡 �Ϸ������, ���� �۾��� ������ �Լ��� ����Ѵ�

	//���� �Լ��� �ۼ����� �ʰ� ���ٷ� �ϴ� ���

	//ResultCallback.BindLambda([AssetPath,this]()
	//{ 
	//	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
	//	TestMash->SetSkeletalMesh(LoadedMesh.Get());
	//	XRLOG(Warning, TEXT("MeshLoadComplete")); 
	//});

	//�ش� ����� ������ �񵿱� �ε��ϰ� �Ϸ�� ResultCallback�� ��ϵ� �Լ��� ������ �����Ѵ�
	TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);





	FSoftObjectPath AssetPath2 =
		TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(3000001);

	FStreamableDelegate ResultCallback2;
	ResultCallback2.BindLambda([AssetPath2, this]()
	{
		//�ִϸ��̼� �������Ʈ�� ��쿣 TSoftClassPtr�� ��ȯ�Ѵ�
		TSoftClassPtr<UAnimInstance> LoadedAnim(AssetPath2);
		TestMash->SetAnimInstanceClass(LoadedAnim.Get());
		XRLOG(Warning, TEXT("AnimBlueprint Load Complete"));
		//*** Ŭ������ ����ϴ� ������ ��� ���� ��� ���� �ݵ��  _C�� �پ���Ѵ� ***///
	});

	TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath2, ResultCallback2);





}

// Called every frame
void ATestPawn_AssetMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPawn_AssetMgr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestPawn_AssetMgr::ResourceASyncLoadComplete(FSoftObjectPath AssetPath)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
	TestMash->SetSkeletalMesh(LoadedMesh.Get());
	//�񵿱� �ε��� �Ϸ�ǰ� ���ϴ� ������ Ŭ���� ptr�� ��ȯ�� ���ҽ��� �����Ѵ� 

	XRLOG(Warning, TEXT("MeshLoadComplete"));
}

