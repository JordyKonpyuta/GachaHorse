// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/MainMenuCameraManager.h"
#include "Camera/CameraActor.h"
#include "CharactersData/MainMenuController.h"
#include "GameData/BaseGameInstance.h"
#include "GameData/BaseGamemode.h"
#include "GameData/MainMenuGamemode.h"
#include "Kismet/GameplayStatics.h"

	// ==========================
	// ==    Base Functions    ==
	// ==========================

AMainMenuCameraManager::AMainMenuCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMainMenuCameraManager::CheckForPregameErrors()
{
	bool ErrorFound = false;

	if (!Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController())->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("CONTROLLER NOT FOUND, SHUTTING THE GAME DOWN"));
		ErrorFound = true;
	}

	for (TPair<ECameraPosition, TObjectPtr<ACameraActor>> CurCam : Cameras)
	{
		if (!CurCam.Value->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Warning, TEXT("+1 CAMERA NOT SET IN CAMERAS MAP"));
			ErrorFound = true;
		}
	}

	if (ErrorFound)
		FGenericPlatformMisc::RequestExit(false);
}

// Called when the game starts or when spawned
void AMainMenuCameraManager::BeginPlay()
{
	Super::BeginPlay();

	CheckForPregameErrors();

	ControllerRef = Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController());

	if (!ControllerRef->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("CONTROLLER NOT FOUND, SHUTTING THE GAME DOWN"));
		FGenericPlatformMisc::RequestExit(false);
	}
	
	ControllerRef->CameraHandler = this;

	CurrentCameraPosition = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())->bTitleScreenAlreadyAppeared ?
		ECameraPosition::Menu : ECameraPosition::TitleScreen;

	if (CurrentCameraPosition == ECameraPosition::Menu)
	{
		Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->PrepareLoadingScreen();
	}
	else
	{
		// CHANGE CAMERA TARGET
		ChangeCameraView();
	}
}

// Called every frame
void AMainMenuCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
	
	// ===========================
	// ==      Camera Move      ==
	// ===========================

void AMainMenuCameraManager::ChangeCameraView()
{
	ACameraActor* NewCamera = Cameras[CurrentCameraPosition];
	ControllerRef->SetViewTargetWithBlend(NewCamera, 0.0f);

	//UE::AudioEditor::PlaySound(CameraChangeSound);
	UGameplayStatics::PlaySound2D(GetWorld(), CameraChangeSound, 1.0f, 1.0f, 0.0f);
	
	ChangeMenu();
}

void AMainMenuCameraManager::ChangeCameraView(ECameraPosition MenuType)
{
	if (!((MenuType == ECameraPosition::HorseSummoning || MenuType == ECameraPosition::EquipSummoning)
		&& (CurrentCameraPosition == ECameraPosition::EquipSummoning || CurrentCameraPosition == ECameraPosition::HorseSummoning)))
	{
		GetWorldTimerManager().SetTimer(
        		WidgetCreationTimerHandle,
        		this,
        		&AMainMenuCameraManager::ChangeMenu,
        		1.25f,
        		false);
	}
	
	CurrentCameraPosition = MenuType;
	ACameraActor* NewCamera = Cameras[CurrentCameraPosition];
	ControllerRef->SetViewTargetWithBlend(NewCamera, 1.0f);
}

void AMainMenuCameraManager::ChangeMenu()
{
	CreateGameWidget(CurrentCameraPosition);
}

void AMainMenuCameraManager::CreateGameWidget_Implementation(ECameraPosition MenuType)
{
}