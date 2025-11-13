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
	TSoftObjectPtr<UMaterialInstance> HorseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> HairMaterial;

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
		TSoftObjectPtr<UMaterialInstance> NewHairMat)
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


USTRUCT(BlueprintType)
struct GACHAHORSE_API FWorldMapDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	// WORLD DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> WorldToLoad;

	// DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BestPersonalTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> Rankings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> WorldPicture;

	FWorldMapDataStruct()
	{
		WorldToLoad = nullptr;
		BestPersonalTime = 0;
		LevelName = "LevelName";
		Rankings = {
			{"Name1", 100.000},
			{"Name2", 100.823},
			{"Name3", 101.643},
			{"Name4", 102.289},
			{"Name5", 102.979},
			{"Name6", 103.679},
			{"Name7", 104.542},
			{"Name8", 105.075},
			{"Name9", 105.523},
			{"Name10", 106.091},
			{"Name11", 106.833},
			{"Name12", 107.591},
			{"Name13", 108.210},
			{"Name14", 108.524},
			{"Name15", 109.114},
			{"Name16", 109.757},
			{"Name17", 110.252},
			{"Name18", 111.068},
			{"Name19", 111.784},
			{"Name20", 112.761},
			{"Name21", 113.128},
			{"Name22", 113.621},
			{"Name23", 114.225},
			{"Name24", 114.518},
			{"Name25", 115.070},
			{"Name26", 115.423},
			{"Name27", 115.666},
			{"Name28", 116.566},
			{"Name29", 116.953},
			{"Name30", 117.599},
			{"Name31", 118.418},
			{"Name32", 118.664},
			{"Name33", 119.214},
			{"Name34", 119.940},
			{"Name35", 120.228},
			{"Name36", 120.904},
			{"Name37", 121.825},
			{"Name38", 122.513},
			{"Name39", 122.784},
			{"Name40", 123.265},
			{"Name41", 124.126},
			{"Name42", 124.583},
			{"Name43", 125.074},
			{"Name44", 125.640},
			{"Name45", 126.242},
			{"Name46", 126.465},
			{"Name47", 126.992},
			{"Name48", 127.627},
			{"Name49", 128.514},
			{"Name50", 128.875},
			{"Name51", 129.308},
			{"Name52", 130.117},
			{"Name53", 130.897},
			{"Name54", 131.182},
			{"Name55", 131.722},
			{"Name56", 132.415},
			{"Name57", 133.373},
			{"Name58", 133.985},
			{"Name59", 134.204},
			{"Name60", 134.964},
			{"Name61", 135.453},
			{"Name62", 136.238},
			{"Name63", 136.961},
			{"Name64", 137.643},
			{"Name65", 137.754},
			{"Name66", 138.376},
			{"Name67", 139.256},
			{"Name68", 139.906},
			{"Name69", 140.207},
			{"Name70", 140.662},
			{"Name71", 141.377},
			{"Name72", 141.627},
			{"Name73", 142.254},
			{"Name74", 143.208},
			{"Name75", 143.615},
			{"Name76", 144.282},
			{"Name77", 145.042},
			{"Name78", 145.541},
			{"Name79", 146.185},
			{"Name80", 146.711},
			{"Name81", 147.027},
			{"Name82", 147.679},
			{"Name83", 148.436},
			{"Name84", 149.252},
			{"Name85", 149.987},
			{"Name86", 150.682},
			{"Name87", 151.501},
			{"Name88", 151.995},
			{"Name89", 152.882},
			{"Name90", 153.401},
			{"Name91", 154.160},
			{"Name92", 154.707},
			{"Name93", 155.414},
			{"Name94", 155.970},
			{"Name95", 156.635},
			{"Name96", 157.579},
			{"Name97", 158.232},
			{"Name98", 158.512},
			{"Name99", 159.251},
			{"Name100", 159.857}};
		WorldPicture = nullptr;
	}

	FWorldMapDataStruct(TSoftObjectPtr<UWorld> WorldChosen, float CurTime, FString Name,
		TMap<FName, float> NewRankings, TSoftObjectPtr<UTexture2D> TheWorldPicture)
	{
		WorldToLoad = WorldChosen;
		BestPersonalTime = CurTime;
		LevelName = Name;
		Rankings = NewRankings;
		WorldPicture = TheWorldPicture;
	}
};


USTRUCT(BlueprintType)
struct GACHAHORSE_API FEquipDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	// CARD DATA
	UPROPERTY(BlueprintReadOnly, Blueprintable)
	bool bEquipmentPossessed;
	UPROPERTY(BlueprintReadOnly, Blueprintable)
	int Level;

	// DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EquipmentID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipmentName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture> EquipmentPicture;

	// Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> AccelerationPerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> SpeedPerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> HandlingPerLevel;

	// MESH
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> EquipMaterial;

	FEquipDataStruct()
	{
		bEquipmentPossessed = false;
		Level = 1;
		EquipmentID = 0;
		Rarity = 0;
		EquipmentName = "ThisIsNotASaddle";
		EquipmentPicture = nullptr;
		AccelerationPerLevel = {0,0,0,0,0,0};
		SpeedPerLevel= {0,0,0,0,0,0};
		HandlingPerLevel= {0,0,0,0,0,0};
		EquipMaterial = nullptr;
	}

	FEquipDataStruct(bool PossessEquip, int NewLevel, int32 NewID, int32 NewRarity,
		FString NewName, TSoftObjectPtr<UTexture> NewEquipPic,
		TArray<int32> NewAccelerationLevelArray, TArray<int32> NewSpeedLevelArray,
		TArray<int32> NewHandlingLevelArray, TSoftObjectPtr<UMaterialInstance> NewEquipMat)
	{
		bEquipmentPossessed = PossessEquip;
		Level = NewLevel;
		EquipmentID = NewID;
		Rarity = NewRarity;
		EquipmentName = NewName;
		EquipmentPicture = NewEquipPic;
		AccelerationPerLevel = NewAccelerationLevelArray;
		SpeedPerLevel = NewSpeedLevelArray;
		HandlingPerLevel = NewHandlingLevelArray;
		EquipMaterial = NewEquipMat;
	}
};

class GACHAHORSE_API AllStructs
{
public:
	AllStructs();
	~AllStructs();
};
