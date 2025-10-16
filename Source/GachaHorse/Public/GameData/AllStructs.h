// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AllStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GACHAHORSE_API FHorseDataStruct
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HorseID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HorseName;

	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> AccelerationPerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpeedPerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> HandlingPerLevel;

	// MESH
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> HorseMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimInstance> HorseAnimation;

	FHorseDataStruct()
	{
		HorseID = 0;
		HorseName = "ThisIsntAHorse";
		AccelerationPerLevel.Empty();
		SpeedPerLevel.Empty();
		HandlingPerLevel.Empty();
		HorseMesh = nullptr;
		HorseAnimation = nullptr;
	}

	FHorseDataStruct(int32 NewID, FString NewName, TArray<int32> NewAccelerationLevelArray,
		TArray<int32> NewSpeedLevelArray, TArray<int32> NewHandlingLevelArray, TSoftObjectPtr<USkeletalMesh> NewMesh,
		TSoftObjectPtr<UAnimInstance> NewAnimation)
	{
		HorseID = NewID;
		HorseName = NewName;
		AccelerationPerLevel = NewAccelerationLevelArray;
		SpeedPerLevel = NewSpeedLevelArray;
		HandlingPerLevel = NewHandlingLevelArray;
		HorseMesh = NewMesh;
		HorseAnimation = NewAnimation;
	}
};

class GACHAHORSE_API AllStructs
{
public:
	AllStructs();
	~AllStructs();
};
