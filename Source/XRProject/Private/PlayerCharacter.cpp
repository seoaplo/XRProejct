// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InputComponent.h"
#include "PlayerCharacter.h"

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


void APlayerCharacter::ChangeEquipment(EEquipmentsType Types, UItem * Item)
{



}