// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/BaseGamemode.h"

#include "GameObjects/Checkpoints.h"

void ABaseGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (!bHasStartedRun)
		if (Timer >= 0.0f)
			StartGame();
			
}

void ABaseGamemode::StartGame()
{
	// INITIALIZE VALUES
	Timer = 0.0f;
	bHasStartedRun = true;

	// UNSOLIDIFY START CHECKPOINT
	if (!StartCheckpointRef->IsValidLowLevelFast())
		return;

	//StartCheckpointRef->
}
