// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemConsumption.generated.h"

USTRUCT(BlueprintType)
struct FConsumptionDefaultInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 IconResourceID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 RecoveryHP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 RecoveryStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString ToolTip;
};

struct ConsumptionInfo // �������� �����ִ� ������
{
	int32_t ID;
	int32_t Count;
};

UCLASS()
class XRPROJECT_API UItemConsumption : public UItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FConsumptionDefaultInfo DefaultInfo; // ������ �⺻ �Ӽ�
private:
	ConsumptionInfo AddInfo; // ������ �߰� �ɷ�ġ �� ���� ���ۿ�
public:
	virtual bool Use() override;
	virtual int GetCount() override;
	virtual int GetID() override;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		UTexture2D* GetIcon() { return nullptr; };
};
