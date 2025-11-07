// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/MainMenuCameraManager.h"
#include "Camera/CameraActor.h"
#include "CharactersData/MainMenuController.h"

// Sets default values
AMainMenuCameraManager::AMainMenuCameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuCameraManager::BeginPlay()
{
	Super::BeginPlay();

	Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController())->CameraHandler = this;

	// CHANGE CAMERA TARGET
	Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController())->SetViewTargetWithBlend(MenuCamera, 0.0f);
	     
}

// Called every frame
void AMainMenuCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

