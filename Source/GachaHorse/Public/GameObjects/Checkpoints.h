// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoints.generated.h"

class ABaseHorse;
class UArrowComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class ECheckpointType : uint8
{
	Generic		UMETA (DisplayName = "Generic"),
	LastGen		UMETA (DisplayName = "Last Generic"),
	Start		UMETA (DisplayName = "Start")
};


UCLASS()
class GACHAHORSE_API ACheckpoints : public AActor
{
	GENERATED_BODY()

	// UPROPERTIES
public:	
	// ==========================
	// ==      Components      ==
	// ==========================
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UBoxComponent> CheckpointArea;
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UArrowComponent> Arrow;
	
	// ==========================
	// ==        Values        ==
	// ==========================

	UPROPERTY(EditAnywhere, Category="Checkpoints")
	ECheckpointType CurrentCheckpointType = ECheckpointType::Generic;

	UPROPERTY(EditAnywhere, Category="Checkpoints", meta = (EditCondition = "CurrentCheckpointType == ECheckpointType::Generic"))
	int CurrentIndex = 0;

protected:

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	ACheckpoints();
	
	virtual void Tick(float DeltaTime) override;
	
	// ==========================
	// ==      Collisions      ==
	// ==========================

	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void UnblockStart();

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
	
	// ==========================
	// ==    Race Functions    ==
	// ==========================
	
	// ===========================
	// ==   Overlap Functions   ==
	// ===========================

	UFUNCTION()
	void RegularCheckpointCrossed(ABaseHorse* HorseActor);

	UFUNCTION()
	void StartCheckPointCrossed(ABaseHorse* HorseActor);
};
