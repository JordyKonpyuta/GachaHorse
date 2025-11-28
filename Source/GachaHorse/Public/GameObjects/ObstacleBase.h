// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleBase.generated.h"

class ABaseHorse;
class UBoxComponent;

UCLASS()
class GACHAHORSE_API AObstacleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UBoxComponent> RagdollArea;
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY()
	TObjectPtr<ABaseHorse> HorseRef;
	
	// ==========================
	// ==        Mercy         ==
	// ==========================
	
	UPROPERTY(EditAnywhere)
	float MercyTimerLimit = 0.2f;
	
protected:
	// ==========================
	// ==        Mercy         ==
	// ==========================

	UPROPERTY()
	bool bMercyTimerRuns = false;
	UPROPERTY()
	float MercyTimer = 0.0f;
	

public:	
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	AObstacleBase();
	
	virtual void Tick(float DeltaTime) override;
	
	// ==========================
	// ==      Collisions      ==
	// ==========================

	UFUNCTION()
	void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	virtual void BeginPlay() override;
};
