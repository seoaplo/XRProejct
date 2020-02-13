// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"


UQuickSlot::UQuickSlot(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsEmpty = true;
	bIsSkill = false;
	ID = 0;
}

void UQuickSlot::PushButton()
{
	// �������� ������ ����
}

UTexture2D * UQuickSlot::GetIcon()
{
	if (!bIsEmpty)
	{

	}
	return nullptr;
}

int UQuickSlot::GetCount()
{
	if (bIsEmpty || bIsSkill)
	{
		return 0;
	}
	else
	{
		return Inventory::GetInstance().GetItemCount(ID);
	}
}

void UQuickSlot::SetItem(UItem* NewItem)
{
	if (NewItem->GetItemType() == EItemType::CONSUMPTION)
	{
		bIsEmpty = false;
		bIsSkill = false;
		ID = NewItem->GetID();
	}
}

void UQuickSlot::SetSkill(UPlayerSkill* NewSkill)
{
	if (false) // ��ų �����ϰ� ��Ƽ���� ��쿡�� ���ü� �ֵ��� �Ѵ�.
	{
		bIsEmpty = false;
		bIsSkill = true;
		//ID = NewSkill->GetID();
	}
}

void UQuickSlot::ClearSlot()
{
	bIsEmpty = true;
	bIsSkill = false;
	ID = 0;
	Update();
}
