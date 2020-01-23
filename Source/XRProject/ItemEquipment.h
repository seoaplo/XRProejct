// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemEquipment.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentDefaultInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		UTexture2D* Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 MaleMeshResourceID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 FemaleMeshResourceID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 SubType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqLEV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 DEF;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 STR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 DEX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 INT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqSTR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqDEX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqINT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString ToolTip;
};

struct EquipmentInfo // �������� �����ִ� ������
{
	int32_t ID;
	int32_t AddDEF;
	int32_t AddSTR;
	int32_t AddDEX;
	int32_t AddINT;
};

UCLASS()
class XRPROJECT_API UItemEquipment : public UItem
{
	GENERATED_BODY()
public:
	UItemEquipment();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FEquipmentDefaultInfo DefaultInfo; // ������ �⺻ �Ӽ�
private:
	EquipmentInfo AddInfo; // ������ �߰� �ɷ�ġ �� ���� ���ۿ�
public:
	virtual bool Use() override { return false; };
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddDEF() { return AddInfo.AddDEF; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddSTR() { return AddInfo.AddSTR; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddDEX() { return AddInfo.AddDEX; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddINT() { return AddInfo.AddINT; };
};