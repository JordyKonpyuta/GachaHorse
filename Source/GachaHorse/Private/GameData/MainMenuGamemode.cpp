// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/MainMenuGamemode.h"

#include "GameData/AllStructs.h"

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

	if (!AllHorsesPossessed->IsValidLowLevelFast())
	{
		FTimerHandle ErrorShutDown;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "YOU DID NOT APPLY A DATATABLE, SHUTTING DOWN GAME IN 5 SECONDS");
		GetWorldTimerManager().SetTimer(
			ErrorShutDown,
			this,
			&AMainMenuGamemode::CrashForDebug,
			5.0f,
			false);
		return;
	}

	if (!AllHorsesPossessed->RowStruct->IsValidLowLevelFast())
	{
		FTimerHandle ErrorShutDown;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "YOU DID NOT APPLY A DATATABLE, SHUTTING DOWN GAME IN 5 SECONDS");
		GetWorldTimerManager().SetTimer(
			ErrorShutDown,
			this,
			&AMainMenuGamemode::CrashForDebug,
			5.0f,
			false);
		return;
	}

	if (AllHorsesPossessed->RowStruct != FHorseDataStruct::StaticStruct())
	{
		FTimerHandle ErrorShutDown;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "YOU DID NOT APPLY AN ADEQUATE DATA TABLE, THIS IS NOT A HORSEDATASTRUCT, WHAT IS WRONG WITH YOU, SHUTTING DOWN GAME IN 5 SECONDS");
		GetWorldTimerManager().SetTimer(
			ErrorShutDown,
			this,
			&AMainMenuGamemode::CrashForDebug,
			5.0f,
			false);
		return;
	}
	
}

void AMainMenuGamemode::CrashForDebug()
{
	FGenericPlatformMisc::RequestExit(false);
}
