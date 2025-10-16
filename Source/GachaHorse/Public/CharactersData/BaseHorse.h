// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BaseHorse.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;

UCLASS()
class GACHAHORSE_API ABaseHorse : public ACharacter
{
	GENERATED_BODY()

	// UPROPERTY
public:
	// =========================
	// ==      Movements      ==
	// =========================

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> Move_Action;
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> Jump_Action;
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> Speed_Action;
	
protected:
	// ==========================
	// ==      Components      ==
	// ==========================

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> HorseSpringArm;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> HorseCamera;
	
	// =========================
	// ==        Stats        ==
	// =========================

	/**
	 * Array with three ints. \n
	 * First Stat : Acceleration \n
	 * Second Stat : Speed \n
	 * Third Stat : Handling
	 */
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Stats")
	TArray<int> Stats = {150,150,2};
	
	// =========================
	// ==        Speed        ==
	// =========================

	// TARGETS
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Targets")
	float TargetSpeed = 0.0f;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Targets")
	int CurrentSpeedIndex = 1;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Targets")
	TArray<float> SpeedTable = {-200, 0, 350, 800, 1200, 1750};

	// ACTUAL
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	bool bCanShiftSpeed = true;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	FTimerHandle ShiftSpeedTimerHandle;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	float CurrentSpeed = 0.0f;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	float SideSpeed = 0.0f;

	// SLOPE
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Slope")
	int SlopeType;
	
	// ==========================
	// ==         Jump         ==
	// ==========================
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Jump")
	bool bIsChargingJump = false;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Jump")
	float JumpCharge = 0.0f;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Jump")
	bool bHasJustJumped = false;
	
	// ==========================
	// ==   Lateral movement   ==
	// ==========================

	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Lateral Movement")
	float TurnRateFactor = 0.0f;
	
	// =========================
	// ==       Respawn       ==
	// =========================

	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Respawn")
	FTransform RespawnPoint;
	
	// =========================
	// ==       Ragdoll       ==
	// =========================

	/**
	 * As you can see, the above property is a boolean that allows one to know whether
	 * the character is currently in the process of being a ragdoll.
	 * In truth, this is an incredibly necessary and important, core function of the
	 * game ; without the ragdoll, it would be like Mario without Luigi, spaghetti
	 * without Ketchup, or indeed, Aerith without a sword sticking through her dumb
	 * fucking chest.
	 */
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Ragdoll")
	bool bIsRagdoll;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Ragdoll")
	FTimerHandle RagdollTimerHandle;
	
	// ==========================
	// ==       End Game       ==
	// ==========================
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="EndGame")
	bool bGameEnded;
	

	// UFUNCTIONS
public:	
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	ABaseHorse();
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// =========================
	// ==        Speed        ==
	// =========================

	UFUNCTION()
	void SetTargetSpeed(int IndexSpeed);
	
	// ==========================
	// ==       End Game       ==
	// ==========================

	UFUNCTION()
	void FinishRace();
	
protected:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	virtual void BeginPlay() override;
	
	// =========================
	// ==      Movements      ==
	// =========================

	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	UFUNCTION()
	void PrepareJump(const FInputActionValue& Value);
	UFUNCTION()
	void ReleaseJump(const FInputActionValue& Value);
	
	UFUNCTION()
	void ChangeSpeed(const FInputActionValue& Value);
	
	// =========================
	// ==       Respawn       ==
	// =========================

	UFUNCTION()
	void SetPlayerRespawn();
	void SetPlayerRespawn(const FTransform& RespawnLoc);
	
	// =========================
	// ==        Speed        ==
	// =========================

	UFUNCTION()
	void InitAcceleration();

	UFUNCTION()
	void InitSpeed();

	UFUNCTION()
	void InitHandling();

	UFUNCTION()
	void ResetShiftSpeed();

	UFUNCTION()
	void SlopeCheck();

	UFUNCTION()
	void CalculateCurrentSpeed();
	
	// ==========================
	// ==         Jump         ==
	// ==========================

	UFUNCTION()
	void ChargeJump(float DeltaTime);

	virtual void Landed(const FHitResult& Hit) override;
	
	// =========================
	// ==       Respawn       ==
	// =========================

	UFUNCTION()
	void Respawn();
	
	// =========================
	// ==       Ragdoll       ==
	// =========================

	UFUNCTION()
	void BeginRagdoll();
	UFUNCTION()
	void CeaseRagdoll();
	
	// ========================
	// ==       Damage       ==
	// ========================
	
	UFUNCTION()    
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// =========================
	// ==       Widgets       ==
	// =========================

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateWidgetRace();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeleteWidgetRace();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateWidgetFinish();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeleteWidgetFinish();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Widget_ShowCharge();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Widget_HideCharge();
	
	// =========================
	// ==        TESTS        ==
	// =========================
	
	void A(const FInputActionValue& Value);
	void B(const FInputActionValue& Value);
	void C(const FInputActionValue& Value);
};
