// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGamemode.generated.h"

class UBaseGameInstance;
class ABaseHorse;
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
	TObjectPtr<UBaseGameInstance> BaseGameInstanceRef;
	
	UPROPERTY()
	TObjectPtr<ACheckpoints> StartCheckpointRef;

	UPROPERTY()
	TObjectPtr<ABaseHorse> HorseRef;
	
	// ===========================
	// ==         Timer         ==
	// ===========================

	UPROPERTY()
	float Timer = -7.0f;

	UPROPERTY()
	bool bThreeBeforeGo = false;
	bool bTwoBeforeGo = false;
	bool bOneBeforeGo = false;
	bool bHasStartedRun = false;
	bool bHasEndedRun = false;

protected:

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	ABaseGamemode();
	
	virtual void Tick(float DeltaTime) override;
	
	// =========================
	// ==       Widgets       ==
	// =========================

	UFUNCTION(BlueprintCallable, Blueprintable, BlueprintNativeEvent)
	void HideAwayTimer();
	
	// =========================
	// ==       EndGame       ==
	// =========================

	UFUNCTION()
	void Victory();

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
	
	// ==========================
	// ==      Beginnings      ==
	// ==========================

	UFUNCTION()
	void StartGame();
	
	// =========================
	// ==       EndGame       ==
	// =========================

	UFUNCTION()
	int CalculateRank(float TimeOfRank) const;
	
	// =========================
	// ==       Widgets       ==
	// =========================

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupWidgets();

	/**
	 * Widget called to do that "3,2,1, go!" thing
	 * @param TimeBeforeGo How many times this function will be called still before the "go" appears
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Widget_ReadyToGo(int TimeBeforeGo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void WidgetTimerUpdate(float CurrentTimer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void WidgetVictory(float CurrentTimer, int OldRank, int NewRank, int RewardAmount);
};