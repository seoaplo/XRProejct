// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "XRAIController.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AXRAIController : public AAIController
{
	GENERATED_BODY()

public:
	AXRAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;


	void RunAI();
	void StopAI();

protected:
private:
	void OnRepeatTimer();




private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UAIPerceptionComponent* EnermyPerceptionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Sight* SightConfig;

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
