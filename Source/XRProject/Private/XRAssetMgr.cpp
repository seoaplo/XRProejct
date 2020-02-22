// Fill out your copyright notice in the Description page of Project Settings.


#include "XRAssetMgr.h"
#include "Engine/ObjectLibrary.h"
#define OUT 



UXRAssetMgr::UXRAssetMgr()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Resources/DataTable/ResourceTable.ResourceTable'"));
	if(DataTable.Succeeded())
	{
		XRLOG(Warning, TEXT("FindedResourceTable"));
		ResourceDataTable = DataTable.Object;
	}
	else
	{
		XRLOG(Error, TEXT("Can't Find ResourceTable"));

	}

	static ConstructorHelpers::FObjectFinder<UDataTable> NPCDATATABLE(TEXT("DataTable'/Game/Resources/DataTable/MonsterTable.MonsterTable'"));
	if (NPCDATATABLE.Succeeded())
	{
		XRLOG(Warning, TEXT("Finded NPCTable"));
		NPCDataTable = NPCDATATABLE.Object;
	}
	else
	{
		XRLOG(Error, TEXT("Can't Find NPCTable"));
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> 
		NPCSKILLTABLE(TEXT("DataTable'/Game/Resources/DataTable/MonsterSkillTable.MonsterSkillTable'"));
	if (NPCDATATABLE.Succeeded())
	{
		XRLOG(Warning, TEXT("Finded NPCSkillTable"));
		NPCSkillDataTable = NPCSKILLTABLE.Object;
	}
	else
	{
		XRLOG(Error, TEXT("Can't Find NPCSkillTable"));
	}
}

bool UXRAssetMgr::ReadAssetDirectory(FString DirName, UClass* baseClass)
{
	/*XRLOG(Warning, TEXT("ReadAssetDir :%s"),*DirName);
	UObjectLibrary* ObjLib;
	ObjLib = UObjectLibrary::CreateLibrary(baseClass, false, GIsEditor);
	ObjLib->AddToRoot();
	ObjLib->LoadAssetDataFromPath(DirName);
	ObjLib->LoadAssetsFromAssetData();
	ObjLib->GetAssetDataList(AssetDatas[0]);
	XRLOG(Warning, TEXT("ReadAssetDirComplete :%s"), *DirName);
	return true;*/
	return true;
}



void UXRAssetMgr::ASyncLoadAssetFromPath(FSoftObjectPath  SoftObj, FStreamableDelegate CompleteDelegate)
{
	if (SoftObj != nullptr)
	{
		XRLOG(Warning, TEXT("ASyncLoadAssetStart : %s"), *SoftObj.ToString());
		UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObj,
			CompleteDelegate);
	}
	else
	{
		XRLOG(Warning, TEXT("Asset Not Exist!! "));
	}
}



void UXRAssetMgr::ASyncLoadAssetComplete(FString LoadAssetName)
{
	XRLOG(Warning, TEXT("ASyncLoadAssetCompleted"));
}

FSoftObjectPath UXRAssetMgr::FindResourceFromDataTable(int32 ResousrceID)
{
	if (ResourceDataTable != nullptr)
	{
		FResourceTableRow* ResourceTableRow =
			ResourceDataTable->FindRow<FResourceTableRow>
			(FName(*(FString::FromInt(ResousrceID))), FString(""));
		if (ResourceTableRow)
		{
			XRLOG(Warning, TEXT("Finded Resource ID : %d  Path : %s  Name : %s "), ResousrceID, *ResourceTableRow->ResourcePath, *ResourceTableRow->ResourceName);
			if (!AssetSoftPathList.Find(ResousrceID))
			{
				AssetSoftPathList.Add(ResousrceID, FSoftObjectPath(ResourceTableRow->ResourcePath));

			}
			return AssetSoftPathList[ResousrceID];
		}
	}
	XRLOG(Warning, TEXT("ResourceID %d Not Exist "), ResousrceID);
	return nullptr;
}


FResourceLocalSize UXRAssetMgr::FindResourceSizeFromTable(int32 ResousrceID)
{
	FResourceLocalSize OutTransForm;
	if (ResourceDataTable != nullptr)
	{
		FResourceTableRow* ResourceTableRow =
			ResourceDataTable->FindRow<FResourceTableRow>
			(FName(*(FString::FromInt(ResousrceID))), FString(""));
		if (ResourceTableRow)
		{
			XRLOG(Warning, TEXT("Finded Resource ID : %d  Path : %s  Name : %s "), ResousrceID, *ResourceTableRow->ResourcePath, *ResourceTableRow->ResourceName);
			
			OutTransForm.LocalTransform.InitFromString(ResourceTableRow->ResourceLocalTransForm);
			OutTransForm.CapsuleHeight = ResourceTableRow->CapsuleHeight;
			OutTransForm.CapsuleRad = ResourceTableRow->CapsuleRad;

			XRLOG(Warning, TEXT("TransForm : %s"), *OutTransForm.LocalTransform.ToString());
			
			return OutTransForm;
		}
	}
	XRLOG(Warning, TEXT("ResourceID %d Not Exist "), ResousrceID);
	return OutTransForm;
}


