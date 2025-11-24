// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuCameraManager.generated.h"

class AMainMenuController;

UENUM(BlueprintType)
enum class ECameraPosition : uint8
{
	TitleScreen		UMETA(DisplayName = "Title Screen"),
	Menu			UMETA(DisplayName = "Menu"),
	HorseSummoning	UMETA(DisplayName = "Horse Summoning"),
	EquipSummoning	UMETA(DisplayName = "Equipment Summoning"),
	Management		UMETA(DisplayName = "Management")
};

UCLASS()
class GACHAHORSE_API AMainMenuCameraManager : public AActor
{
	GENERATED_BODY()

	// UPROPERTIES
public:	
	// ==========================
	// ==      References      ==
	// ==========================

	UPROPERTY(Blueprintable, BlueprintReadOnly)
	TObjectPtr<AMainMenuController> ControllerRef;

	UPROPERTY(EditAnywhere)
	TMap<ECameraPosition, TObjectPtr<ACameraActor>> Cameras =
		{
		{ECameraPosition::TitleScreen, nullptr},
		{ECameraPosition::Menu, nullptr},
		{ECameraPosition::HorseSummoning, nullptr},
		{ECameraPosition::EquipSummoning, nullptr},
		{ECameraPosition::Management, nullptr}
		};

	UPROPERTY()
	FTimerHandle WidgetCreationTimerHandle;
	
	// ==========================
	// ==       Movement       ==
	// ==========================

	UPROPERTY()
	ECameraPosition CurrentCameraPosition = ECameraPosition::TitleScreen;
	
	// ===========================
	// ==         Timer         ==
	// ===========================

	UPROPERTY()
	bool bCurrentlyMoving;
	
	// ==========================
	// ==        Widget        ==
	// ==========================
	
protected:
	// =========================
	// ==        Sound        ==
	// =========================

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite) 
	TObjectPtr<USoundBase> CameraChangeSound;

	// UFUNCTIONS
public:	
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	AMainMenuCameraManager();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	UFUNCTION()
	void CheckForPregameErrors();
	
	virtual void BeginPlay() override;
	
	// ===========================
	// ==      Camera Move      ==
	// ===========================

	UFUNCTION(BlueprintCallable, Blueprintable)
	void ChangeCameraView(ECameraPosition MenuType);
	void ChangeCameraView();

	UFUNCTION(BlueprintNativeEvent)
	void CreateGameWidget(ECameraPosition MenuType);

	UFUNCTION(Blueprintable, BlueprintCallable)
	void ChangeMenu();
};
