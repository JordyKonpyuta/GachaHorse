// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuCameraManager.generated.h"

UENUM(BlueprintType)
enum ECameraPosition : uint8
{
	Menu		UMETA(DisplayName = "Menu"),
	Summoning   UMETA(DisplayName = "Summoning"),
	Management	UMETA(DisplayName = "Management")
};

UCLASS()
class GACHAHORSE_API AMainMenuCameraManager : public AActor
{
	GENERATED_BODY()

	// UPROPERTIES
public:	
	// ==========================
	// ==      References      ==
	// ==========================

	UPROPERTY()
	TObjectPtr<ACameraActor> MenuCamera;
	UPROPERTY()
	TObjectPtr<ACameraActor> SummoningCamera;
	UPROPERTY()
	TObjectPtr<ACameraActor> ManagementCamera;
	
	
	// ==========================
	// ==       Movement       ==
	// ==========================

	UPROPERTY()
	ECameraPosition CurrentCameraPosition = ECameraPosition::Menu;
	
	// ===========================
	// ==         Timer         ==
	// ===========================

	UPROPERTY()
	bool bCurrentlyMoving;
	
	// ==========================
	// ==        Widget        ==
	// ==========================
	
protected:

	// UFUNCTIONS
public:	
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	AMainMenuCameraManager();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
};
