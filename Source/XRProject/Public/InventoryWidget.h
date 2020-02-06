// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "Inventory.h"
#include "SlotWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"



UCLASS()
class XRPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
public:
	std::vector<USlotWidget*> list;
	bool bIsVisible;
	static UInventoryWidget* InventoryInstance;
public:
	static UInventoryWidget* GetInstance();
public:
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetVisible(bool IsVisible);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		bool GetVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SwitchVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		int GetWidthSize();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		int GetHeightSize();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetUp();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		FString GetGlod();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void AddList(USlotWidget* newslot);

};
