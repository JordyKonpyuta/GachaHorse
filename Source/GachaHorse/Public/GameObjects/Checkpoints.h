// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoints.generated.h"

class UArrowComponent;
class UBoxComponent;

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

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
};
