// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HorseGameSave.generated.h"

struct FHorseDataStruct;
/**
 * 
 */
UCLASS()
class GACHAHORSE_API UHorseGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHorseDataStruct> HorseData;

	// MONEYS
	UPROPERTY()
	int SummonMoney = 0;
	UPROPERTY()
	int Shards = 0;
};
