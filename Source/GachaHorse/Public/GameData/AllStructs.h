// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AllStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GACHAHORSE_API FHorseDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	// CARD DATA
	UPROPERTY(BlueprintReadOnly, Blueprintable)
	bool bHorsePossessed;
	UPROPERTY(BlueprintReadOnly, Blueprintable)
	int ShardNumber;
	UPROPERTY(BlueprintReadOnly, Blueprintable)
	int Level;

	// DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HorseID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HorseName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture> HorsePicture;

	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> AccelerationPerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpeedPerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> HandlingPerLevel;

	// MESH
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterial> HorseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterial> HairMaterial;

	FHorseDataStruct()
	{
		bHorsePossessed = false;
		ShardNumber = 0;
		Level = 1;
		HorseID = 0;
		Rarity = 0;
		HorseName = "ThisIsNotAHorse";
		HorsePicture = nullptr;
		AccelerationPerLevel = {0,0,0,0,0,0};
		SpeedPerLevel= {0,0,0,0,0,0};
		HandlingPerLevel= {0,0,0,0,0,0};
		HorseMaterial = nullptr;
		HairMaterial = nullptr;
	}

	FHorseDataStruct(bool PossessHorse, int NewShards, int NewLevel, int32 NewID,
		int32 NewRarity, FString NewName, TSoftObjectPtr<UTexture> NewHorsePic,
		TArray<int32> NewAccelerationLevelArray, TArray<int32> NewSpeedLevelArray,
		TArray<int32> NewHandlingLevelArray, TSoftObjectPtr<UMaterial> NewHorseMat,
		TSoftObjectPtr<UMaterial> NewHairMat)
	{
		bHorsePossessed = PossessHorse;
		ShardNumber = NewShards;
		Level = NewLevel;
		HorseID = NewID;
		Rarity = NewRarity;
		HorseName = NewName;
		HorsePicture = NewHorsePic;
		AccelerationPerLevel = NewAccelerationLevelArray;
		SpeedPerLevel = NewSpeedLevelArray;
		HandlingPerLevel = NewHandlingLevelArray;
		HorseMaterial = NewHorseMat;
		HairMaterial = NewHairMat;
	}
};

class GACHAHORSE_API AllStructs
{
public:
	AllStructs();
	~AllStructs();
};
