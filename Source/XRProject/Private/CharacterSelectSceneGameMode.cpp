#include "CharacterSelectSceneGameMode.h"
#include "PlayerCharacter.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "InputStream.h"
#include <functional>

void ACharacterSelectSceneGameMode::SendConfirmRequest()
{
	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kLobbyConfirmRequest);
	out.WriteCString(ID.c_str());
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
}

void ACharacterSelectSceneGameMode::CreatePlayerCharacter(APlayerCharacter* Character, FCharacterSelectInfo& Info)
{
	UPlayerCharacterStatComponent* MyComponent = Character->PlayerStatComp;

	MyComponent->CharacterName = Info.Name.c_str();

	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//�������
	FSoftObjectPath HairAssetPath = nullptr;
	FPartsResource* HairResourceTable =  PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(Info.Hair)), TEXT("t"));
	HairAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(HairResourceTable->ResourceID);
	FStreamableDelegate HairAssetLoadDelegate;
	HairAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
		HairAssetPath, EPartsType::HAIR, Character);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(HairAssetPath, HairAssetLoadDelegate);


	//���̽� ����
	FSoftObjectPath FaceAssetPath = nullptr;
	FPartsResource* FaceResourceTable = PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(Info.Face)), TEXT("t"));
	FaceAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(FaceResourceTable->ResourceID);
	FStreamableDelegate FaceAssetLoadDelegate;
	FaceAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
		FaceAssetPath, EPartsType::FACE, Character);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(FaceAssetPath, FaceAssetLoadDelegate);

	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Info.armor_itemid, GetWorld(), Character);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Info.hand_itemid, GetWorld(), Character);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Info.shoes_itemid, GetWorld(), Character);
	MyComponent->Gender = Info.gender;
}

void ACharacterSelectSceneGameMode::LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type, APlayerCharacter* Character)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	Character->ChangePartsComponentsMesh(Type, LoadedMesh.Get());
	
}

ACharacterSelectSceneGameMode::ACharacterSelectSceneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	FString PartsDataPath = TEXT("DataTable'/Game/Resources/DataTable/PartsTable.PartsTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PARTS(*PartsDataPath);
	if (DT_PARTS.Succeeded())
		PartsDataTable = DT_PARTS.Object;

	BeforeSlotNumber = 0;


}

void ACharacterSelectSceneGameMode::BeginPlay()
{
	// �ν��Ͻ����� �κ񼭹��� IP, port�� �߰��ϴ� ������� �ٲٱ� & ���ڵ� ��ȿ�� ���ڵ�� ����
	CurrentWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), LoginWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kConfirmFailNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterCreateFail);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterListNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterList);

	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterSlotNotify)->BindUObject(
	//	this, &ACharacterSelectSceneGameMode::HandleCharacterSlot);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kMigrateZoneNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleMigrateZone);

	std::string Ip = AccountManager::GetInstance().GetLobbyIP();
	int16 Port = AccountManager::GetInstance().GetLobbyPort();
	GetNetMgr().Connect(Ip.c_str(), Port, std::bind(&ACharacterSelectSceneGameMode::SendConfirmRequest, this));


	/*ĳ���� ����â ī�޶� ��ġ*/
	MainCameraLocation = FVector(0.0f, 2740.0f, 290.0f);
	CharacterActorLocation = MainCameraLocation + FVector(100.0f, 0.0f, 0.0f);
	MainCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(),
		MainCameraLocation, FRotator::ZeroRotator);
	APlayerController* CurrentController = UGameplayStatics::GetPlayerController(this, 0);
	CurrentController->SetViewTarget(MainCamera);
	
}

void ACharacterSelectSceneGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetNetMgr().Close();
}

void ACharacterSelectSceneGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetNetMgr().Update();
}

void ACharacterSelectSceneGameMode::ChangeSelectedCharacter(int64 SlotNumber)
{
	CharacterList[BeforeSlotNumber]->SetActorHiddenInGame(true);
	CharacterList[SlotNumber]->SetActorHiddenInGame(false);
	BeforeSlotNumber = SlotNumber;

}

void ACharacterSelectSceneGameMode::HandleCharacterCreateFail(InputStream& input)
{
	CurrentWidget->CharacterLoginFail();
}



void ACharacterSelectSceneGameMode::HandleCharacterList(InputStream& input)
{

	int32_t SlotSize;
	input >> SlotSize;

	CharacterList.resize(5);
	
	for (int i = 0; i < SlotSize; i++)
	{
		int SlotNumber = input.ReadInt32();
		FCharacterSelectInfo Info;
		Info.Name = input.ReadCString();
		input >> Info.Level; input >> Info.Str; input >> Info.Dex;
		input >> Info.Int;	input >> Info.Job;  input >> Info.Face;
		input >> Info.Hair; input >> Info.Gold; input >> Info.Zone;
		input >> Info.x; input >> Info.y; input >> Info.z;
		input >> Info.armor_itemid; input >> Info.hand_itemid; input >> Info.shoes_itemid;
		input >> Info.weapon_itemid; input >> Info.sub_weapon_itemid; input >> Info.gender;

		APlayerCharacter* Character = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(),
			CharacterActorLocation, FRotator(0.0f, 180.0f, 0.0f));

		CreatePlayerCharacter(Character, Info);
		Character->SetActorHiddenInGame(true);
		CharacterList[SlotNumber] = Character;
		
	}

	
}

void ACharacterSelectSceneGameMode::HandleMigrateZone(InputStream& input)
{
	string InGameIP;
	int16 InGamePort;
	InGameIP = input.ReadCString();
	input >> InGamePort;
	AccountManager::GetInstance().SetInGameIP(InGameIP);
	AccountManager::GetInstance().SetInGamePort(InGamePort);

	GetNetMgr().Close();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LEVEL_Village"));
}

void ACharacterSelectSceneGameMode::HandleCharacterSlot(InputStream& input)
{
	int32_t SlotNum;
	string Name;
	int32_t Level;
	int32_t Str;
	int32_t Dex;
	int32_t Int;
	int32_t Job;
	int32_t Face;
	int32_t Hair;
	int32_t Gold;
	int32_t Zone;
	float x, y, z;
	int32_t armor_itemid;
	int32_t hand_itemid;
	int32_t shoes_itemid;;
	int32_t weapon_itemid;
	int32_t sub_weapon_itemid;
	int32_t gender;

	input >> SlotNum;
	if (SlotNum == -1)
	{
		CurrentWidget->CharacterCreateResult(false);
		return;
	}
	else
	{
		Name = input.ReadCString();
		input >> Level;
		input >> Str;
		input >> Dex;
		input >> Int;
		input >> Job;
		input >> Face;
		input >> Hair;
		input >> Gold;
		input >> Zone;
		input >> x;
		input >> y;
		input >> z;
		input >> armor_itemid;
		input >> hand_itemid;
		input >> shoes_itemid;;
		input >> weapon_itemid;
		input >> sub_weapon_itemid;
		input >> gender;
	}
}