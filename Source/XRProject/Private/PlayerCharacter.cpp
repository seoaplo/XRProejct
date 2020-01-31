// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "ItemManager.h"
#include "XRGameInstance.h"
#include "Animation/AnimBlueprint.h"
#include "AccountManager.h"
#include "Components/InputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerStatComp = CreateDefaultSubobject<UPlayerCharacterStatComponent>(TEXT("CharacterStat"));
	PlayerStatComp->OnHPZero.AddDynamic(this, &ABaseCharacter::OnDead);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP
		(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_PlayerCharacter.ABP_PlayerCharacter_C'"));

  	if (AnimBP.Succeeded())
	{
		AnimInstance = AnimBP.Class;
	}
	else
		check(false);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = false;

	SpringArmLength = 300.0f;
	RotateSpeed = 1000.0f;
	MovementSpeed = 1000.0f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = SpringArmLength;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false; //��Ʈ�ѷ� �����̼ǿ� ���� ĳ���Ͱ� ȸ���ϴ� bool. �����ؾ� ���Ƿ� ȸ����ų �� ����.
	bUseControllerRotationPitch = false; //��Ʈ�ѷ� �����̼ǿ� ���� ĳ���Ͱ� ȸ���ϴ� bool. �����ؾ� ���Ƿ� ȸ����ų �� ����.
	bUseControllerRotationRoll = false; //��Ʈ�ѷ� �����̼ǿ� ���� ĳ���Ͱ� ȸ���ϴ� bool. �����ؾ� ���Ƿ� ȸ����ų �� ����.

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	CameraComponent->bUsePawnControlRotation = false;
	
	
	FName HairSocket("HairSocket");
	FName FaceSocket("FaceSocket");
	
	HairComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	FaceComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		INVISIBLE_MESH
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/CommonSkeleton/SK_Character_human_male_skeleton.SK_Character_human_male_skeleton'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TESTMESH
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_Character_human_male_body_common.SK_Character_human_male_body_common'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		FIRSTBODYMESH
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_Character_human_male_body_common.SK_Character_human_male_body_common'"));

	GetMesh()->SetSkeletalMesh(INVISIBLE_MESH.Object);
	FaceComponent->SetSkeletalMesh(FIRSTBODYMESH.Object);

	Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Equipments.LegsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Equipments.HandsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Equipments.BodyComponent->SetupAttachment(RootComponent);
	Equipments.LegsComponent->SetupAttachment(RootComponent);
	Equipments.HandsComponent->SetupAttachment(RootComponent);
	FaceComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, FaceSocket);
	HairComponent->AttachToComponent(FaceComponent, FAttachmentTransformRules::KeepRelativeTransform, HairSocket);

	Equipments.BodyComponent->SetSkeletalMesh(FIRSTBODYMESH.Object);
	Equipments.BodyComponent->SetAnimInstanceClass(AnimBP.Class);

	Equipments.LegsComponent->SetMasterPoseComponent(Equipments.BodyComponent);
	Equipments.HandsComponent->SetMasterPoseComponent(Equipments.BodyComponent);

	ComboCount = 0;
	bIsMove = false;

#pragma region TESTCODE

	/* ���� : �������� �����ϰ� �׽�Ʈ �� */
	bIsMale = true;

#pragma endregion
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::Tick(float deltatime)
{
	Super::Tick(deltatime);
	ABaseCharacter::Tick(deltatime);
#pragma region TESTCODE
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
#pragma endregion

	if (Cast<APlayerController>(GetController()))
	{
		SumSec += deltatime;
			if (SumSec >= 0.1f) {
				SumSec -= 0.1f;

					if (GetCharacterMovement()->Velocity.Size() > KINDA_SMALL_NUMBER)
					{
						OutputStream out;
							out.WriteOpcode(ENetworkCSOpcode::kNotifyCurrentChrPosition);
							out << 999;
							out << GetActorLocation();
						out << GetActorRotation();
						GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Location : %s"), *GetActorLocation().ToString()));
						GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Rotator : %s"), *GetActorRotation().ToString()));
						out.CompletePacketBuild();
						GetNetMgr().SendPacket(out);
					}
			}
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Rotator : %s"), *GetCharacterMovement()->Velocity.ToString()));
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAction("Attack", IE_Pressed,this, &APlayerCharacter::Attack);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Equipments.BodyComponent->SetAnimInstanceClass(AnimInstance);
	MyAnimInstance = Cast<UPlayerCharacterAnimInstance>(Equipments.BodyComponent->GetAnimInstance());
	MyAnimInstance->Delegate_CheckNextCombo.BindUFunction(this, FName("ContinueCombo"));
	MyAnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);

}

void APlayerCharacter::PossessedBy(AController* controller)
{
	Super::PossessedBy(controller);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void APlayerCharacter::BeginPlay()
{
	ABaseCharacter::BeginPlay();


	auto GameInstance = Cast < UXRGameInstance > (GetGameInstance());
	bool Ret = AccountManager::GetInstance().SetCurrentPlayerCharacter(this);
	check(Ret);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3020001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3120001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3220001, GetWorld(), this);
	ChangePartsById(EPartsType::HAIR, 110);
	ChangePartsById(EPartsType::FACE, 120);


}

void APlayerCharacter::MoveForward(float Value)
{
	if (bIsAttack == false)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
			bIsMove = true;
		}
		else
		{
			bIsMove = false;
		}
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (bIsAttack == false)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
			bIsMove = true;
		}
	}
}


void APlayerCharacter::ChangePartsById(EPartsType Type, int32 ID)
{
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FPartsResource* PartResourceTable = GameInstance->ItemManager->PartsDataTable->
		FindRow<FPartsResource>(*(FString::FromInt(ID)), TEXT("t"));

	if (Type == EPartsType::HAIR)
	{
		//�������
		FSoftObjectPath HairAssetPath = nullptr;
		HairAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate HairAssetLoadDelegate;
		HairAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			HairAssetPath, EPartsType::HAIR);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(HairAssetPath, HairAssetLoadDelegate);
	}
	else if (Type == EPartsType::FACE)
	{
		//���̽� ����
		FSoftObjectPath FaceAssetPath = nullptr;
		FaceAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate FaceAssetLoadDelegate;
		FaceAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			FaceAssetPath, EPartsType::FACE);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(FaceAssetPath, FaceAssetLoadDelegate);
	}

}

void APlayerCharacter::ChangeEquipment(UItem * Item, USkeletalMesh* SkMesh)
{
	UItemEquipment* EquipItem = Cast<UItemEquipment>(Item);

	if (EquipItem == nullptr)
		check(false);

	EEquipmentsType Types;

	switch (EquipItem->DefaultInfo.Type)
	{
		case 0: { Types = EEquipmentsType::BODY; break; }
		case 1: { Types = EEquipmentsType::HANDS; break; }
		case 2: { Types = EEquipmentsType::LEGS; break; }
		case 3: { Types = EEquipmentsType::WEAPON; break; }
	}


	//Ŭ���� �����ۺ������� �������� �̹� ����Ǿ� ���Դٰ� ����
	//���� ĳ���Ͱ� ��/�������� �Ƹ� GetPawn�����ɷ� �������� �� ��
	//�����ۺ������� �ּ·ε���� �Ǹ� �� �Լ��� �����
	//�����̸� ���� �Լ��� �ٸ��� �ұ�

	switch (Types)
	{
		case EEquipmentsType::BODY:
			Equipments.BodyItem = EquipItem;
			Equipments.BodyComponent->SetSkeletalMesh(SkMesh);
			MyAnimInstance->CurrentSkeleton = Equipments.BodyComponent->SkeletalMesh->Skeleton;
			break;
		case EEquipmentsType::HANDS:
			Equipments.HandsItem = EquipItem;
			Equipments.HandsComponent->SetSkeletalMesh(SkMesh);
			break;
		case EEquipmentsType::LEGS:
			Equipments.LegsItem = EquipItem;
			Equipments.LegsComponent->SetSkeletalMesh(SkMesh);
			break;
		case EEquipmentsType::WEAPON:
			Equipments.WeaponItem = EquipItem;
			Equipments.WeaponComponent->SetSkeletalMesh(SkMesh);
			break;
	}


}

void APlayerCharacter::ChangePartsComponentsMesh(EPartsType Type, USkeletalMesh * PartsMesh)
{
	if (Type == EPartsType::HAIR)
	{
		HairComponent->SetSkeletalMesh(PartsMesh);
	}
	else if (Type == EPartsType::FACE)
	{
		FaceComponent->SetSkeletalMesh(PartsMesh);
	}
}

void APlayerCharacter::Attack()
{
	//first
	if (bIsAttack == false)
	{
		bIsAttack = true;
		MyAnimInstance->PlayAttackMontage();
	}
	else
		bSavedCombo = true;
}

void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttack = false;
	bSavedCombo = false;
	ComboCount = 1;
}

void APlayerCharacter::ContinueCombo()
{
	if (bSavedCombo)
	{
		if (ComboCount >= kMaxComboCount)
		{
			ComboCount = 1;
			return;
		}

		ComboCount++;
		MyAnimInstance->JumpToComboMontageSection(ComboCount);
		bSavedCombo = false;
		XRLOG(Warning, TEXT("CurrentCombo : %d"), ComboCount);
	}
}


void APlayerCharacter::LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	AccountManager::GetInstance().GetCurrentPlayerCharacter()->ChangePartsComponentsMesh(Type, LoadedMesh.Get());

}
