// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGamemode.generated.h"

class ACheckpoints;
/**
 * 
 */
UCLASS()
class GACHAHORSE_API ABaseGamemode : public AGameModeBase
{
	GENERATED_BODY()

	// UPROPERTIES
public:
	// ==========================
	// ==      References      ==
	// ==========================
	
	UPROPERTY()
	TObjectPtr<ACheckpoints> StartCheckpointRef;
	
	// ===========================
	// ==         Timer         ==
	// ===========================

	UPROPERTY()
	int Timer = -5.0f;

	UPROPERTY()
	bool bHasStartedRun = false;

protected:

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void Tick(float DeltaTime) override;

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	UFUNCTION()
	void StartGame();
	
};