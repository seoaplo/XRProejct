#include "CharacterSelectWidget.h"
#include "NetworkOpcode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "XRGameInstance.h"
#include "OutputStream.h"

void UCharacterSelectWidget::AddCharacter(APlayerCharacter* Character)
{
	CharacterList.push_back(Character);
	CharacterCount = CharacterList.size();
	UpdateList();
}

void UCharacterSelectWidget::DeleteCharacter(int Num)
{
	for (int i = Num; i < CharacterCount - 1; i++)
	{
		CharacterList[i] = CharacterList[i + 1];
	}
	CharacterList.pop_back();
	CharacterCount = CharacterList.size();
	UpdateList();
}

APlayerCharacter* UCharacterSelectWidget::GetCharacter(int Num)
{
	if (Num < CharacterList.size() || Num >= 0)
	{
		return CharacterList[Num];
	}
	return nullptr;
}

void UCharacterSelectWidget::CharacterSelectionRequest(int Numder)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterSelectionRequest);
	out << (int32_t)Numder;
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void UCharacterSelectWidget::CharacterCreateRequest(int SlotNum, FText Name, int FaceID, int HairID, int Gender)
{
	if (Name.IsEmpty())
	{
		return;
	}

	std::wstring w_Name(*Name.ToString());
	std::string c_Name(w_Name.begin(), w_Name.end());

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterCreateRequest);
	int32_t Zero = 0;
	out << (int32_t)SlotNum;
	out.WriteCString(c_Name.c_str());
	out << Zero; // ����
	out << Zero; // ��
	out << Zero; // ��
	out << Zero; // ��
	out << Zero; // ��
	out << (int32_t)FaceID; // ��
	out << (int32_t)HairID; // �Ӹ�
	out << Zero; // ��
	out << Zero; // ��
	out << Zero; // x
	out << Zero; // y
	out << Zero; // z
	out << Zero; // �Ƹ�id
	out << Zero; // �ڵ�id
	out << Zero; // ����id
	out << Zero; // ����id
	out << Zero; // �������id
	out << (int32_t)Gender; // ����
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
}

void UCharacterSelectWidget::CharacterDeleteRequest(int Numder)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterDeleteRequest);
	out << (int32_t)Numder;
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void UCharacterSelectWidget::ReturnLoginServer()
{
	OutputStream out;
	// �α��� ������ ȸ���Ѵٴ� ��Ŷ �ʿ�
	//out.WriteOpcode(ENetworkCSOpcode::kReturnLoginRequest);
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}