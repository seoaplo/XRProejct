// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotObject.h"
#include "Item.generated.h"

// ------------------------------------------------------------------
// ������
// ����, ���, �Һ�, 
// ------------------------------------------------------------------

UENUM()
enum class EItemType : uint8
{
	NONE,
	ETC,
	CONSUMPTION,
	EQUIPMENT,
};

UCLASS(BlueprintType)
class XRPROJECT_API UItem : public USlotObject
{
	GENERATED_BODY()

public:
	EItemType ItemType;
public:
	virtual int GetCount() override;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual bool Use();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetID();
public:
	EItemType GetItemType();
};
