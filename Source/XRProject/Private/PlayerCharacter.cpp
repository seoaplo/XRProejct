// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "ItemManager.h"
#include "XRGameInstance.h"
#include "Components/InputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

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

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

#pragma region TESTCODE
	Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Equipments.BodyComponent->SetupAttachment(RootComponent);
	auto Mesh = 
		ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_human_body_newbie_male.SK_human_body_newbie_male'"));
	Equipments.BodyComponent->SetSkeletalMesh(Mesh.Object);
#pragma endregion
}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::Tick(float deltatime)
{
#pragma region TESTCODE
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
#pragma endregion
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::MoveRight);

}

void APlayerCharacter::BeginPlay()
{
	auto GameInstance = Cast<UXRGameInstance>(GetGameInstance());
	GameInstance->ItemManager->SetPlayerCharacter(this);
	GameInstance->ItemManager->BuildItem(3000001, GetWorld());

}

void APlayerCharacter::MoveForward(float Value)
{

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void APlayerCharacter::ChangeEquipment(int32 NumTypes, UItem * Item, USkeletalMesh* SkMesh)
{
	
	/*
	HAIR = 0,FACE,BODY,HANDS,LEGS,WEAPON,SUBWEAPON,
	*/
	EEquipmentsType Types;

	switch (NumTypes)
	{
		case 0: { Types = EEquipmentsType::HAIR; break; }
		case 1: { Types = EEquipmentsType::FACE; break; }
		case 2: { Types = EEquipmentsType::BODY; break; }
		case 3: { Types = EEquipmentsType::HANDS; break; }
		case 4: { Types = EEquipmentsType::LEGS; break; }
		case 5: { Types = EEquipmentsType::WEAPON; break; }
		case 6: { Types = EEquipmentsType::SUBWEAPON; break; }
	}

	//Ŭ���� �����ۺ������� �������� �̹� ����Ǿ� ���Դٰ� ����
	//���� ĳ���Ͱ� ��/�������� �Ƹ� GetPawn�����ɷ� �������� �� ��
	//�����ۺ������� �ּ·ε���� �Ǹ� �� �Լ��� �����
	//�����̸� ���� �Լ��� �ٸ��� �ұ�

	bool IsWeapon = false;

	UItemEquipment* EquipItem = Cast<UItemEquipment>(Item);
	UItemWeapon* WeaponItem = nullptr;

	if (EquipItem == nullptr)
	{
		IsWeapon = true;
		WeaponItem = Cast<UItemWeapon>(Item);
	}

	if (EquipItem == nullptr && WeaponItem == nullptr)
		check(false);

	switch (Types)
	{
		case EEquipmentsType::HAIR:
			Equipments.HairItem = EquipItem;
			Equipments.HairComponent->SkeletalMesh = SkMesh;
			break;
		case EEquipmentsType::FACE:
			Equipments.FaceItem = EquipItem;
			Equipments.FaceComponent->SkeletalMesh = SkMesh;
			break;
		case EEquipmentsType::BODY:
			Equipments.BodyItem = EquipItem;
			Equipments.BodyComponent->SkeletalMesh = SkMesh;
			break;
		case EEquipmentsType::HANDS:
			Equipments.HandsItem = EquipItem;
			Equipments.HandsComponent->SkeletalMesh = SkMesh;
			break;
		case EEquipmentsType::LEGS:
			Equipments.LegsItem = EquipItem;
			Equipments.LegsComponent->SkeletalMesh = SkMesh;
			break;
		case EEquipmentsType::WEAPON:
			Equipments.WeaponItem = WeaponItem;
			Equipments.WeaponComponent->SkeletalMesh = SkMesh;
			break;
		case EEquipmentsType::SUBWEAPON:
			Equipments.SubWeaponItem = WeaponItem;
			Equipments.SubWeaponComponent->SkeletalMesh = SkMesh;
			break;
	}


}