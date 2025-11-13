// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/MainMenuGamemode.h"

#include "GameData/AllStructs.h"
#include "GameData/BaseGameInstance.h"
#include "GameData/BaseGamemode.h"

AMainMenuGamemode::AMainMenuGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainMenuGamemode::BeginPlay()
{
	Super::BeginPlay();

	CheckPossibleCrash();
}
	
	// ==========================
	// ==   GACHA! GAMBLING!   ==
	// ==========================

TArray<int> AMainMenuGamemode::CalculatePossibleGains(bool bTenSummons)
{
	TArray<int> AllHorses;
	int CurOdd;

	if (!bTenSummons)
	{
		CurOdd = FMath::RandRange(0.000f, 100.000f);

		if (CurOdd >= 93.75)
		{
			// RARE HORSE :DDD
			AllHorses.Add(FMath::RandRange(9, 12));
		}
		else if (CurOdd >= 62.5)
		{
			// UNCOMMON HORSE :)
			AllHorses.Add(FMath::RandRange(5, 8));
		}
		else
		{
			// COMMON HORSE :(
			AllHorses.Add(FMath::RandRange(0, 4));
		}

		return AllHorses;
	}

	for (int i = 0; i < 9; i++)
	{
		CurOdd = FMath::RandRange(0.000f, 100.000f);

		if (CurOdd >= 93.75)
		{
			// RARE HORSE :DDD
			AllHorses.Add(FMath::RandRange(9, 12));
		}
		else if (CurOdd >= 62.5)
		{
			// UNCOMMON HORSE :)
			AllHorses.Add(FMath::RandRange(5, 8));
		}
		else
		{
			// COMMON HORSE :(
			AllHorses.Add(FMath::RandRange(0, 4));
		}
	}

	CurOdd = FMath::RandRange(0, 5);

	if (CurOdd < 5)
	{
		// UNCOMMON HORSE DDD:
		AllHorses.Add(FMath::RandRange(5, 8));
	}
	else
	{
		// RARE HORSE :DDD
		AllHorses.Add(FMath::RandRange(9, 12));
	}
	
	return AllHorses;
}

void AMainMenuGamemode::GachaGotHorses()
{
	UBaseGameInstance* InstanceRef = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	
}

void AMainMenuGamemode::GachaGotEquips()
{
	
}

	// ==========================
	// ==        DEBOOG        ==
	// ==========================

void AMainMenuGamemode::CheckPossibleCrash()
{
	bool ErrorFound = false;

	if (!AllHorsesPossessed->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("DATA TABLE FOR HORSES NOT FOUND, SHUTTING THE GAME DOWN"));
		ErrorFound = true;
	}
	else
	{
		if (!AllHorsesPossessed->RowStruct->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Warning, TEXT("DATA TABLE APPEARS TO BE CORRUPTED, SHUTTING THE GAME DOWN"));
			ErrorFound = true;
		}
		else
		{
			if (AllHorsesPossessed->RowStruct != FHorseDataStruct::StaticStruct())
			{
				UE_LOG(LogTemp, Warning, TEXT("DATA TABLE IS NOT OF THE CORRECT TYPE, SHUTTING THE GAME DOWN"));
				ErrorFound = true;
			}
		}
	}
	
	if (ErrorFound)
		FGenericPlatformMisc::RequestExit(false);
}
