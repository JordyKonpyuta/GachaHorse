// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGamemode.generated.h"


class UBaseGameInstance;
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
	// ==========================
	// ==      References      ==
	// ==========================

	UPROPERTY()
	TObjectPtr<UBaseGameInstance> InstanceRef;

protected:
	// =========================
	// ==        Money        ==
	// =========================

	UPROPERTY()
	int SummonMoney = 1000000;

	UPROPERTY()
	int ScrapMoney = 0;
	
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
	
	// =========================================================
	// ==         █     █ █████ ███  ██    ████ █████         ==
	// ==          █ █ █    █   █  █ █ ███ ██     █           ==
	// ==           █ █   █████ ███  ████  ████   █           ==
	// =========================================================
	
	// ==========================
	// ==   GACHA! GAMBLING!   ==
	// ==========================

	UFUNCTION(BlueprintNativeEvent, Blueprintable, BlueprintCallable)
	void DisplaySummonResults(bool bIsHorses, int ThingID);

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
	
	// ==========================
	// ==   GACHA! GAMBLING!   ==
	// ==========================

	UFUNCTION()
	TArray<int> CalculatePossibleGains(bool bTenSummons);

	UFUNCTION(Blueprintable, BlueprintCallable)
	void GachaPullHorses(bool bTenSummons);

	UFUNCTION(Blueprintable, BlueprintCallable)
	void GachaPullEquips(bool bTenSummons);

	// ==========================
	// ==        DEBOOG        ==
	// ==========================

	UFUNCTION()
	void CheckPossibleCrash();
	
};
