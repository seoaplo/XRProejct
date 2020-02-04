// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemETC.generated.h"

USTRUCT(BlueprintType)
struct FETCDefaultInfo
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
		FString ToolTip;
};

struct ETCInfo // �������� �����ִ� ������
{
	int32_t ID;
	int32_t Count;
};

UCLASS()
class XRPROJECT_API UItemETC : public UItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FETCDefaultInfo DefaultInfo; // ������ �⺻ �Ӽ�
private:
	ETCInfo AddInfo; // ������ ���� �� ���� ���ۿ�
public:
	virtual int GetCount() override;
	virtual int GetID() override;
	virtual void SetCount(int Count);
	UTexture2D* GetIcon() override;
public:
};
