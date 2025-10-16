// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersData/BaseController.h"
#include "EnhancedInputSubsystems.h"

void ABaseController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
			for (UInputMappingContext* CurrentContext : MappingContext)
				Subsystem->AddMappingContext(CurrentContext, 0);
}
