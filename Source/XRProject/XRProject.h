// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <vector>
#include "EngineMinimal.h"
using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(XRProject, Log, All);

#define XRLOG_CALLINFO	(FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__) + TEXT(")"))
#define XRLOG_S(Verbosity) UE_LOG(XRProject,Verbosity,TEXT("%s"),*XRLOG_CALLINFO)
#define XRLOG(Verbosity,Format,...) UE_LOG(XRProject,Verbosity,TEXT("%s %s"),*XRLOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
#define XRCHECK(Expr,...){if(!(Expr)) {ABLOG(Error,TEXT("ASSERTION:%s"),TEXT("'"#Expr"'")); return __VA_ARGS__; } }

std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc);
std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc);
FText int_to_comma_text(int Data);

struct Equipment
{
	int Type = -1;
	int ID = -1;
	int AddATK = -1;
	int AddDEF = -1;
	int AddSTR = -1;
	int AddDex = -1;
	int AddInt = -1;
	int Count = -1;
};

struct CharacterData
{
	const int32 kEquipmentArraySize = 4;

	// Actor Spawn Data
	int64_t ObjectID = -1;
	FVector Location = FVector(0, 0, 0);
	FRotator Rotator = FRotator(0, 0, 0);

	// Base Character Stat Data
	float Current_HP = -1;
	float Max_HP = -1;
	float Current_Stamina = -1;
	float Max_Stamina = -1;
	float Attack_Min = -1;
	float Attack_Max = -1;
	float Attack_Range = -1;
	float Attack_Speed = -1;
	float Speed = -1;
	float Defence = -1;

	std::wstring Name;

	int32 Level = -1;
	int32 Gender = -1;
	int32 FaceID = -1;
	int32 HairID = -1;
	int32 STR = -1;
	int32 DEX = -1;
	int32 INT = -1;
	int32 Current_Exp;
	int32 Max_Exp;

	// Equipment
	Equipment EquipArray[4];
};

struct MonsterData
{
	// Actor Spawn Data
	int32_t MonsterID = -1;
	int64_t ObjectID = -1;
	FVector Location = FVector(0, 0, 0);
	FRotator Rotator = FRotator(0, 0, 0);

	// Base Character Stat Data
	int Current_HP = -1;
	int Max_HP = -1;
	int Attack_Min = -1;
	int Attack_Max = -1;
	int Attack_Range = -1;
	int Attack_Speed = -1;
	int Speed = -1;
	int Defence = -1;
};
