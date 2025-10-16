// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class GACHAHORSE_API ABaseController : public APlayerController
{
	GENERATED_BODY()

	// UPROPERTIES
public:
	
protected:
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<TObjectPtr<UInputMappingContext>> MappingContext;

	// UFUNCTIONS
public:

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
};
