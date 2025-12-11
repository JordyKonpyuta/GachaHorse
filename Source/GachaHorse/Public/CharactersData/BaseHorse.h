// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BaseHorse.generated.h"

class UBaseGameInstance;
class ABaseGamemode;
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
	// ==========================
	// ==      Correction      ==
	// ==========================

	UPROPERTY(EditAnywhere, Category="Tick")
	float TickCorrecter = 200.0f;
	
	// =========================
	// ==      Movements      ==
	// =========================

	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> Move_Action;
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> Jump_Action;
	UPROPERTY(EditAnywhere, Category="Inputs")
	TObjectPtr<UInputAction> Speed_Action;
	
	// =========================
	// ==     Race Values     ==
	// =========================

	UPROPERTY()
	int CurrentCheckpointIndex = -1;
	
	// =========================
	// ==        Stats        ==
	// =========================

	/**
	 * Array with three ints. \n
	 * First Stat : Acceleration \n
	 * Second Stat : Speed \n
	 * Third Stat : Handling
	 */
	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category="Stats")
	TArray<int> Stats = {5,5,5};
	
	// ==========================
	// ==    Editable Stats    ==
	// ==========================

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseSpeedMultiplier = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRotation = 0.04;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRotationMultiplier = 0.0015;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAcceleration = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAccelerationMultiplier = 10;
	
protected:
	// ==========================
	// ==      References      ==
	// ==========================

	UPROPERTY()
	TObjectPtr<ABaseGamemode> GameModeRef;

	UPROPERTY()
	TObjectPtr<UBaseGameInstance> GameInstanceRef;
	
	// ==========================
	// ==      Components      ==
	// ==========================

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> HorseSpringArm;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> HorseCamera;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletonComponent")
	TObjectPtr<USkeletalMeshComponent> RiderSkel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletonComponent")
	TObjectPtr<USkeletalMeshComponent> HorseSkel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletonComponent")
	TObjectPtr<USkeletalMeshComponent> SaddleSkel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletonComponent")
	TObjectPtr<USkeletalMeshComponent> HairSkel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletonComponent")
	TObjectPtr<USkeletalMeshComponent> BeltsSkel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SkeletonComponent")
	TObjectPtr<USkeletalMeshComponent> ReinsSkel;
	
	// =========================
	// ==        Speed        ==
	// =========================

	// TARGETS
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Targets")
	float TargetSpeed = 0.0f;
	UPROPERTY()
	float MaxAvailableSpeed = 0.0f;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Targets")
	int CurrentSpeedIndex = 1;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Targets")
	TArray<float> SpeedTable = {-200, 0, 350, 800, 1200, 1750};

	// ACTUAL
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	float CurrentSpeed = 0.0f;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	float SideSpeed = 0.0f;
	UPROPERTY()
	float TargetTurnAngle = 0.0f;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	float TurnAngle = 0.0f;

	// SLOPE
	/**
	 * - 1 = Going Down (because it's positive)
	 * - 0 = flat
	 * - -1 = Going Up (because it's negative) 
	 */
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Slope")
	int SlopeType;

	// Hazard
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Slope")
	float HazardModifier = 0.0f;

	// SPEED CHANGES
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	bool bCanShiftSpeed = true;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	bool bCheckSpeedShift = false;
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	float NewSpeedValue = true;
	
	UPROPERTY(Blueprintable, BlueprintReadWrite, Category="Speed Actual")
	FTimerHandle ShiftSpeedTimerHandle;
	
	// ==========================
	// ==        Camera        ==
	// ==========================
	
	FTimerHandle BurstHandle;
	
	UPROPERTY()
	bool bBurstingCamera = false;

	UPROPERTY()
	float TargetCameraArmLength = 600.0f;
	
	UPROPERTY()
	float TargetCameraArmRotation = -20.0f;
	
	UPROPERTY()
	float TargetCameraFOV = 90;

	UPROPERTY()
	float CameraRollMultiplier = 0.0f;
	UPROPERTY()
	bool bCameraRoll = false;
	UPROPERTY()
	bool bCameraJumpMove = false;
	UPROPERTY()
	bool bCameraGoingDown = false;

	UPROPERTY()
	float TargetCameraAngleEndgame = 0.0f;
	
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
	// ==        Stats        ==
	// =========================

	UFUNCTION()
	void PrepareSetStats();
	
	UFUNCTION()
	void SetStats(TArray<int> StatsToSet);
	void SetStats(int StatAccel, int StatSpeed, int StatHandling);
	
	// ========================
	// ==        Jump        ==
	// ========================

	UFUNCTION()
	float GetJumpPower();
	
	// =========================
	// ==        Speed        ==
	// =========================

	UFUNCTION()
	void SetTargetSpeed(int IndexSpeed);
	
	// =========================
	// ==       Ragdoll       ==
	// =========================

	UFUNCTION()
	void BeginRagdoll();
	UFUNCTION()
	bool GetRagdollState();
	
	// =========================
	// ==       Respawn       ==
	// =========================

	UFUNCTION()
	void SetPlayerRespawn();
	void SetPlayerRespawn(const FTransform& RespawnLoc);
	
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
	void StopTurn();

	UFUNCTION(BlueprintCallable)
	void PrepareJump();
	UFUNCTION(BlueprintCallable)
	void ReleaseJump();
	
	UFUNCTION(BlueprintCallable)
	void StartChangeSpeed(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable)
	void StopChangeSpeed();
	UFUNCTION()
	void ActualChangeSpeed();

	UFUNCTION()
	void AnimTurn(float DeltaTime);
	
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
	void PauseShiftSpeedPossibility(float TimeBeforeNewShift);

	UFUNCTION()
	void ResetShiftSpeed();

	UFUNCTION()
	void HazardCheck();

	UFUNCTION()
	void CalculateCurrentSpeed();
	
	// ==========================
	// ==        Camera        ==
	// ==========================

	UFUNCTION()
	void SetInitialCameraValues();
	UFUNCTION()
	void MoveCameraValuesDT(float DeltaTime);
	
	UFUNCTION()
	void CameraBurst(float AddedLengthToSpring);
	UFUNCTION()
	void CameraUnburst();

	UFUNCTION()
	void CalculateCameraValues();


	UFUNCTION()
	void CameraShakeMovement(float DeltaTime);
	UFUNCTION()
	void StartCameraShakeLanded();
	UFUNCTION()
	void CameraShakeLanded(float DeltaTime);
	
	// ==========================
	// ==         Jump         ==
	// ==========================

	UFUNCTION()
	void ChargeJump(float DeltaTime);

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void Landed_Blueprint();
	
	// =========================
	// ==       Respawn       ==
	// =========================

	UFUNCTION()
	void Respawn();
	
	// =========================
	// ==       Ragdoll       ==
	// =========================

	UFUNCTION(BlueprintNativeEvent)
	void VFXOnHit(bool bIsHorseKill);
	UFUNCTION()
	void CeaseRagdoll();

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void TimeBeginsSlow();
	UFUNCTION(Blueprintable, BlueprintCallable)
	void TimeManipulation(float GameSpeed);
	
	// ========================
	// ==       Damage       ==
	// ========================
	
	UFUNCTION()    
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// =========================
	// ==         VFX         ==
	// =========================

	UFUNCTION(BlueprintNativeEvent)
	void TweakSpeedFX(float SpawnRate, FVector Windspeed);
	
	// =========================
	// ==       Widgets       ==
	// =========================

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateWidgetRace();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeleteWidgetRace();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Widget_ShowCharge();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Widget_HideCharge();
};
