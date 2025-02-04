// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	bool IsDead() const;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	

private:
	class APawn* PlayerPawn;

	//UPROPERTY(EditAnywhere)
	//float AcceptanceRadius = 200;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehaviorTree;
};
