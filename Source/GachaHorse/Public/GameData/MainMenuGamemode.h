// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGamemode.generated.h"


struct FHorseDataStruct;
/**
 * 
 */
UCLASS()
class GACHAHORSE_API AMainMenuGamemode : public AGameModeBase
{
	GENERATED_BODY()

	// UPROPERTIES
public:

protected:
	// =========================
	// ==        Money        ==
	// =========================

	UPROPERTY()
	int HorseMoney = 0;
	
	// =========================
	// ==      Summoning      ==
	// =========================

	UPROPERTY(EditAnywhere, Category="GameData")
	UDataTable* AllHorsesPossessed;

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	AMainMenuGamemode();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;

	// ==========================
	// ==        DEBOOG        ==
	// ==========================

	UFUNCTION()
	void CrashForDebug();
	
};
