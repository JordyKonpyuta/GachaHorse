// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGamemode.generated.h"


struct FEquipDataStruct;
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

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBaseGameInstance> InstanceRef;
	
	// ==========================
	// ==      Management      ==
	// ==========================

	UPROPERTY(BlueprintReadOnly)
	bool bIsCheckingHorse = true;

protected:
	// ==========================
	// ==       Missions       ==
	// ==========================

	FTimerHandle MissionTimerHandle;

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	AMainMenuGamemode();
	
	virtual void Tick(float DeltaTime) override;
	
	// =========================
	// ==   Horse Selection   ==
	// =========================

	UFUNCTION(BlueprintCallable)
	void SwitchHorse(bool bClickedLeft);
	
	UFUNCTION(BlueprintNativeEvent, Blueprintable, BlueprintCallable)
	void SelectHorseWidget(FHorseDataStruct HorseDisplayed);

	UFUNCTION(BlueprintCallable)
	void LevelUpHorse();

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void LevelUpHorseWidgetUpdate(FHorseDataStruct HorseDisplayed);
	
	// =========================
	// == Equipment Selection ==
	// =========================
	
	UFUNCTION(BlueprintCallable)
	void SwitchEquip(bool bClickedLeft);
	
	UFUNCTION(BlueprintNativeEvent, Blueprintable, BlueprintCallable)
	void SelectEquipWidget(FEquipDataStruct EquipDisplayed);

	UFUNCTION(BlueprintCallable)
	void LevelUpEquip();

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void LevelUpEquipWidgetUpdate(FEquipDataStruct EquipDisplayed);
	
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
	
	// ==========================
	// ==    Fission Mailed    ==
	// ==========================

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void DisplayMissionsWidget(FString& TrackMissionName, int RankMissionNumber, FString& HorseMissionName);
	
	// ==========================
	// ==   Rankings Results   ==
	// ==========================

	UFUNCTION(Blueprintable, BlueprintCallable)
	void PrepareRankingResults(int CurrentWorldSelected);

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
	
	// ==========================
	// ==       Missions       ==
	// ==========================

	UFUNCTION()
	void DoWeNeedToRefreshNow();

	UFUNCTION()
	void RefreshMissions();

	UFUNCTION()
	float CalculateTimeBeforeNextRefresh(FDateTime NextMissionTime);

	UFUNCTION()
	void PrepareDisplayMissions();
	
	// ==========================
	// ==   GACHA! GAMBLING!   ==
	// ==========================

	UFUNCTION()
	TArray<int> CalculatePossibleHorseGains(bool bTenSummons);

	UFUNCTION()
	TArray<int> CalculatePossibleEquipGains(bool bTenSummons);

	UFUNCTION(Blueprintable, BlueprintCallable)
	void GachaPullHorses(bool bTenSummons);

	UFUNCTION(Blueprintable, BlueprintCallable)
	void GachaPullEquips(bool bTenSummons);

	UFUNCTION()
	void PrepareSummonResults(bool bIsHorses, int ThingID, float Delay);
	
	// ==========================
	// ==   Rankings Results   ==
	// ==========================

	UFUNCTION(BlueprintNativeEvent, Blueprintable, BlueprintCallable)
	void DisplayRankingsResults(bool bIsPlayer, FName CurRankName, float CurRankTime);

	// ==========================
	// ==        DEBOOG        ==
	// ==========================

	UFUNCTION()
	void CheckPossibleCrash();
	
};
