#include "CharacterSelectSceneGameMode.h"
#include "PlayerCharacter.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "InputStream.h"



ACharacterSelectSceneGameMode::ACharacterSelectSceneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterSelectSceneGameMode::BeginPlay()
{
	CurrentWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), LoginWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	// �ν��Ͻ����� �κ񼭹��� IP, port�� �߰��ϴ� ������� �ٲٱ� & ���ڵ� ��ȿ�� ���ڵ�� ����

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kConfirmFailNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterCreateFail);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterListNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterList);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterSlotNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterSlot);

	std::string Ip = AccountManager::GetInstance().GetLobbyIP();
	int16 Port = AccountManager::GetInstance().GetLobbyPort();
	GetNetMgr().Connect(Ip.c_str(), Port,
		[this]() {
		std::string ID = AccountManager::GetInstance().GetAccountID();
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kLobbyConfirmRequest);
		out.WriteCString(ID.c_str());
		GetNetMgr().SendPacket(out);
	});

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

void ACharacterSelectSceneGameMode::HandleCharacterCreateFail(InputStream& input)
{
}

void ACharacterSelectSceneGameMode::HandleCharacterList(InputStream& input)
{
}

void ACharacterSelectSceneGameMode::HandleCharacterSlot(InputStream& input)
{
}
