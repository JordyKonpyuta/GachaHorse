// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersData/BaseHorse.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "GameData/BaseGameInstance.h"
#include "GameData/BaseGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


	// ==========================
	// ==    Base Functions    ==
	// ==========================

ABaseHorse::ABaseHorse()
{
	PrimaryActorTick.bCanEverTick = true;

	// PREPARE COMPONENTS
	// SPRING ARM

	HorseSpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	HorseSpringArm->SetupAttachment(GetCapsuleComponent());
	HorseSpringArm->TargetArmLength = 600.0f;
	HorseSpringArm->SetRelativeRotation(FRotator(-20,0,0));

	// CAMERA
	
	HorseCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	HorseCamera->SetupAttachment(HorseSpringArm);

	// SKELETONS

	RiderSkel = CreateDefaultSubobject<USkeletalMeshComponent>("Rider Skeleton");
	RiderSkel->SetupAttachment(GetMesh());
	RiderSkel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HorseSkel = CreateDefaultSubobject<USkeletalMeshComponent>("Horse Skeleton");
	HorseSkel->SetupAttachment(GetMesh());
	HorseSkel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SaddleSkel = CreateDefaultSubobject<USkeletalMeshComponent>("Saddle Skeleton");
	SaddleSkel->SetupAttachment(HorseSkel);
	SaddleSkel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HairSkel = CreateDefaultSubobject<USkeletalMeshComponent>("Hair Skeleton");
	HairSkel->SetupAttachment(HorseSkel);
	HairSkel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BeltsSkel = CreateDefaultSubobject<USkeletalMeshComponent>("Belts Skeleton");
	BeltsSkel->SetupAttachment(HorseSkel);
	BeltsSkel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ReinsSkel = CreateDefaultSubobject<USkeletalMeshComponent>("Reins Skeleton");
	ReinsSkel->SetupAttachment(HorseSkel);
	ReinsSkel->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);


	
	// FUNCTIONS

	// ????? POURQUOI TU MARCHES PLUS?
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseHorse::OnHit);
}

void ABaseHorse::BeginPlay()
{
	Super::BeginPlay();

	// ????? CA FIXE LE RAGDOLL JE SUPPOSE
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseHorse::OnHit);

	// ZEROTH : GET STATS

	GameInstanceRef = Cast<UBaseGameInstance>(GetGameInstance());
	PrepareSetStats();

	// FIRST : PREPARE RESPAWN

	SetPlayerRespawn();

	// SECOND : SET ALL SPEED VALUES

	InitAcceleration();
	InitSpeed();
	InitHandling();
	GetCharacterMovement()->JumpZVelocity = 0.0f;

	// THIRD : SET SPEED

	SetTargetSpeed(1);
	
	// FOURTH : CREATE WIDGET
	
	GetCharacterMovement()->MaxWalkSpeed = 50000.0f;

	// FIFTH : SET-UP REF IN GAMEMODE

	GameModeRef = Cast<ABaseGamemode>(GetWorld()->GetAuthGameMode());
	if (GameModeRef)
		GameModeRef->HorseRef = this;
}

void ABaseHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ALWAYS CHECK SLOPE
	HazardCheck();

	// CHARGE JUMP
	ChargeJump(DeltaTime);
	
	// CHECK IF SPEED STAGE NEEDS TO CHANGE
	if (bCheckSpeedShift)
		ActualChangeSpeed();

	// MOVE HORSEY FORWARD
	CalculateCurrentSpeed();

	// MOVE CAMERA
	if (!bBurstingCamera)
		CalculateCameraValues();
	if (!bIsRagdoll)
		MoveCameraValuesDT(DeltaTime);
	
	if (!GetCharacterMovement()->IsMovingOnGround() && bIsChargingJump)
	{
		bIsChargingJump = false;
		JumpCharge = 0.0f;
	}

	if (SlopeType == 0)
		MaxAvailableSpeed = TargetSpeed * (1 + HazardModifier);
	else if (SlopeType == 1)
		MaxAvailableSpeed = TargetSpeed * (1 + HazardModifier) * 1.15;
	else if (SlopeType == -1)
		MaxAvailableSpeed = TargetSpeed * (1 + HazardModifier) * (1 - FMath::Max(0, 0.15 - 0.005 * Stats[0]));
	else
		MaxAvailableSpeed = TargetSpeed * (1 + HazardModifier);
	
	
	if (CurrentSpeed > MaxAvailableSpeed + 1)
	{
		if (!bIsRagdoll && GetCharacterMovement()->IsMovingOnGround())
		{
			AddMovementInput(GetActorForwardVector(),
				-(150 - SlopeType * 75) * DeltaTime, false);
		}
	}
	else if (CurrentSpeed < MaxAvailableSpeed - 1)
	{
		if (!bIsRagdoll && GetCharacterMovement()->IsMovingOnGround())
		{
			AddMovementInput(GetActorForwardVector(),
				(120 + SlopeType * 60) * DeltaTime, false);
		}
	}

	GEngine->AddOnScreenDebugMessage(-2, 0.f, FColor::Blue, FString::SanitizeFloat(CurrentSpeed));
	GEngine->AddOnScreenDebugMessage(-1,0.f, FColor::Yellow, FString::SanitizeFloat(HorseCamera->GetRelativeLocation().Z));

	// MOVE HORSEY LEFTY RIGHTY
	if (!(-0.1 < SideSpeed && SideSpeed < 0.1))
		AddMovementInput(GetActorRightVector(), (SideSpeed < 0 ? 2 : -2) * DeltaTime * TickCorrecter);

	// CAMERA SHAKE
	if (!bCameraJumpMove)
		CameraShakeMovement(DeltaTime);
	else
		CameraShakeLanded(DeltaTime);

	// LANDING BEHAVIOUR
	if (bHasJustJumped)
	{
		float NewHalfHeight = FMath::FInterpTo(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - 34, 46, DeltaTime, 92);

		if (NewHalfHeight >= 46)
		{
			bHasJustJumped = false;
			NewHalfHeight = 46;
		}
		GetCapsuleComponent()->SetCapsuleSize(34, NewHalfHeight + 34, true);
	}

	// WOOOOOSH SPEED EFFECTS PARTICLES YEAAAAAH
	if (CurrentSpeed > 1700)
	{
		FVector WindAngle = FVector(UKismetMathLibrary::DegCos(GetActorRotation().Yaw) * -25000, UKismetMathLibrary::DegSin(GetActorRotation().Yaw) * -25000, 0);
		TweakSpeedFX((CurrentSpeed - 1700) * 0.1 + 100, WindAngle);
	}
	else
	{
		TweakSpeedFX(0, FVector(0,0,0));
	}
	
	// GAME END :

	if (bGameEnded)
	{
		HorseSpringArm->TargetArmLength = FMath::FInterpTo(HorseSpringArm->TargetArmLength, 200, DeltaTime, 20);
		HorseSpringArm->SetRelativeRotation(FRotator(0, -20, FMath::FInterpTo(HorseSpringArm->GetRelativeRotation().Roll, -270, DeltaTime, 135)));
	}
}

void ABaseHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move
		EnhancedInputComponent->BindAction(Move_Action, ETriggerEvent::Triggered, this, &ABaseHorse::Turn);
		EnhancedInputComponent->BindAction(Move_Action, ETriggerEvent::Completed, this, &ABaseHorse::StopTurn);

		// Jumping
		EnhancedInputComponent->BindAction(Jump_Action, ETriggerEvent::Started, this, &ABaseHorse::PrepareJump);
		EnhancedInputComponent->BindAction(Jump_Action, ETriggerEvent::Completed, this, &ABaseHorse::ReleaseJump);

		// Speed
		EnhancedInputComponent->BindAction(Speed_Action, ETriggerEvent::Started, this, &ABaseHorse::StartChangeSpeed);
		EnhancedInputComponent->BindAction(Speed_Action, ETriggerEvent::Completed, this, &ABaseHorse::StopChangeSpeed);
	}
}

	// =========================
	// ==      Movements      ==
	// =========================

void ABaseHorse::Turn(const FInputActionValue& Value)
{
	if (bIsRagdoll)
		return;

	float TurnAngle = (Value.Get<float>() * TurnRateFactor) / (bIsChargingJump ? 2 : 1) / (GetCharacterMovement()->IsMovingOnGround() ? 1 : 100);
	TurnAngle *= TickCorrecter;
	
	AddControllerYawInput(TurnAngle * GetWorld()->GetDeltaSeconds());
	GetCharacterMovement()->Velocity = UKismetMathLibrary::RotateAngleAxis(GetCharacterMovement()->Velocity, TurnAngle * GetWorld()->GetDeltaSeconds(), FVector(0, 0, 1));

	CameraRollMultiplier = -2 * Value.Get<float>();
	bCameraRoll = true;
}

void ABaseHorse::StopTurn()
{
	CameraRollMultiplier = 0;
	bCameraRoll = false;
}

void ABaseHorse::PrepareJump()
{
	// DON'T BEGIN THE CHARGING IF IN THE AIR OR ALREADY CHARGING
	if (bIsChargingJump || !GetCharacterMovement()->IsMovingOnGround())
		return;

	// LET TICK START CHARGING JUMP
	bIsChargingJump = true;
	JumpCharge = 0.0f;
	GetCharacterMovement()->JumpZVelocity = 0.0f;

	// DISPLAY CHARGE WIDGET
	Widget_ShowCharge();
}

void ABaseHorse::ReleaseJump()
{
	// DON'T KEEP THE CHARGING IF IN THE AIR
	if (!GetCharacterMovement()->IsMovingOnGround())
	{
		bIsChargingJump = false;
		JumpCharge = 0.0f;
		return;
	}
	
	if (!bIsChargingJump)
		return;

	if (!bIsRagdoll && CurrentSpeed > 750 && GetCharacterMovement()->JumpZVelocity > 325.0f)
	{
		Jump();
		GetCapsuleComponent()->SetCapsuleSize(34,34,true);
	}
	bIsChargingJump = false;
	JumpCharge = 0.0f;
	Widget_HideCharge();
}

void ABaseHorse::StartChangeSpeed(const FInputActionValue& Value)
{
	NewSpeedValue = Value.Get<float>();
	bCheckSpeedShift = true;
}

void ABaseHorse::StopChangeSpeed()
{
	bCheckSpeedShift = false;
}

void ABaseHorse::ActualChangeSpeed()
{
	if (!bCanShiftSpeed)
		return;
	
	if (NewSpeedValue < 0)
	{
		if (CurrentSpeedIndex <= 0)
			return;

		CurrentSpeedIndex -= 1;
		SetTargetSpeed(CurrentSpeedIndex);
		CameraBurst(-150);
	}
	else
	{
		if (CurrentSpeedIndex >= 5)
			return;

		CurrentSpeedIndex += 1;
		SetTargetSpeed(CurrentSpeedIndex);
		CameraBurst(150);
	}

	PauseShiftSpeedPossibility(1.0f);
}

	// =========================
	// ==       Respawn       ==
	// =========================

void ABaseHorse::SetPlayerRespawn()
{
	RespawnPoint = GetActorTransform();
}

void ABaseHorse::SetPlayerRespawn(const FTransform& RespawnLoc)
{
	RespawnPoint = RespawnLoc;
}

	// =========================
	// ==        Stats        ==
	// =========================

void ABaseHorse::PrepareSetStats()
{
	TArray<int> NewStats = {0,0,0};
	int CurHorseLevel = GameInstanceRef->ChosenHorseData.Level - 1;
	int CurEquipLevel = GameInstanceRef->ChosenEquipData.Level - 1;
	NewStats[0] = GameInstanceRef->ChosenHorseData.AccelerationPerLevel[CurHorseLevel] + GameInstanceRef->ChosenEquipData.AccelerationPerLevel[CurEquipLevel];
	NewStats[1] = GameInstanceRef->ChosenHorseData.SpeedPerLevel[CurHorseLevel] + GameInstanceRef->ChosenEquipData.SpeedPerLevel[CurEquipLevel];
	NewStats[2] = GameInstanceRef->ChosenHorseData.HandlingPerLevel[CurHorseLevel] + GameInstanceRef->ChosenEquipData.HandlingPerLevel[CurEquipLevel];
	SetStats(NewStats);
}

void ABaseHorse::SetStats(TArray<int> StatsToSet)
{
	if (StatsToSet.Num() != 3)
		return;
	
	Stats = StatsToSet;
}

void ABaseHorse::SetStats(int StatAccel, int StatSpeed, int StatHandling)
{
	Stats = {StatAccel, StatSpeed, StatHandling};
}
	
	// ========================
	// ==        Jump        ==
	// ========================

float ABaseHorse::GetJumpPower()
{
	return JumpCharge;
}

	// =========================
	// ==        Speed        ==
	// =========================

void ABaseHorse::InitAcceleration()
{
	GetCharacterMovement()->MaxAcceleration = BaseAcceleration + (BaseAccelerationMultiplier * Stats[0]);
}

void ABaseHorse::InitSpeed()
{
	SpeedTable[0] = -1 * (BaseSpeed + (BaseSpeedMultiplier * Stats[1]));
	SpeedTable[1] = 0;
	SpeedTable[2] = 2 * (BaseSpeed + (BaseSpeedMultiplier * Stats[1]));
	SpeedTable[3] = 5 * (BaseSpeed + (BaseSpeedMultiplier * Stats[1]));
	SpeedTable[4] = 8 * (BaseSpeed + (BaseSpeedMultiplier * Stats[1]));
	SpeedTable[5] = 10 * (BaseSpeed + (BaseSpeedMultiplier * Stats[1]));
}

void ABaseHorse::InitHandling()
{
	TurnRateFactor = BaseRotation + (Stats[2] * BaseRotationMultiplier);
}

void ABaseHorse::PauseShiftSpeedPossibility(float TimeBeforeNewShift)
{
	bCanShiftSpeed = false;
	GetWorldTimerManager().SetTimer(
				ShiftSpeedTimerHandle,
				this,
				&ABaseHorse::ResetShiftSpeed,
				TimeBeforeNewShift,
				false);
}

void ABaseHorse::ResetShiftSpeed()
{
	bCanShiftSpeed = true;
}

void ABaseHorse::SetTargetSpeed(int IndexSpeed)
{
	TargetSpeed = SpeedTable[IndexSpeed];
	CurrentSpeedIndex = IndexSpeed;
}

void ABaseHorse::HazardCheck()
{
	// ONLY DOES THIS IF YOU'RE ON THE GROUND
	if (!GetCharacterMovement()->IsMovingOnGround())
	{
		SlopeType = 0;
		return;
	}

	// LINE TRACE TO THE GROUND
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = true;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
	
	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		GetActorLocation(),
		GetActorLocation() + FVector(0,0,-200),
		ECC_Pawn,
		RV_TraceParams);

	if (!RV_Hit.GetActor()->IsValidLowLevelFast())
		return;
	
	// ASSIGN SLOPE TYPE

	float SlopeTypeCalculation = FVector::DotProduct(RV_Hit.ImpactNormal, GetActorForwardVector());

	// NEARLY EQUAL TO 0?
	if (-0.0001 < SlopeTypeCalculation && SlopeTypeCalculation < 0.0001)
		SlopeType = 0;
	else
		SlopeType = SlopeTypeCalculation > 0 ? 1 : -1;

	// ASSIGN GROUND TYPE
	if (!RV_Hit.PhysMaterial->IsValidLowLevelFast())
		HazardModifier = 0.0f;
	else
	{
		// DO ALL MATERIALS IN TIME
		if (RV_Hit.PhysMaterial.Get()->SurfaceType == SurfaceType1)
			HazardModifier = 0.0f;
		else if (RV_Hit.PhysMaterial->SurfaceType == SurfaceType2)
			HazardModifier = -0.1f;
		else
			HazardModifier = 0.0f;
	}
}

void ABaseHorse::CalculateCurrentSpeed()
{
	CurrentSpeed = FVector2D::DotProduct(FVector2D(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y), FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y));
	SideSpeed = FVector2D::DotProduct(FVector2D(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y), FVector2D(GetActorRightVector().X, GetActorRightVector().Y));
}
	
	// ==========================
	// ==        Camera        ==
	// ==========================

void ABaseHorse::SetInitialCameraValues()
{
	HorseSpringArm->TargetArmLength = 600 + CurrentSpeed * 0.05;
	HorseSpringArm->SetRelativeRotation(FRotator(-20 + CurrentSpeed * 0.001,0,0));
	HorseCamera->FieldOfView = 90 + CurrentSpeed * 0.005;
}

void ABaseHorse::MoveCameraValuesDT(float DeltaTime)
{
	HorseSpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(HorseSpringArm->TargetArmLength, TargetCameraArmLength, DeltaTime, 5);
	HorseCamera->FieldOfView = UKismetMathLibrary::FInterpTo(HorseCamera->FieldOfView, TargetCameraFOV, DeltaTime, 0.5);
	GEngine->AddOnScreenDebugMessage(-1,0.f, FColor::Red, FString::SanitizeFloat(HorseSpringArm->TargetArmLength));
	GEngine->AddOnScreenDebugMessage(-1,0.f, FColor::Red, FString::SanitizeFloat(TargetCameraArmLength));
	
	float CurRotPitch = UKismetMathLibrary::FInterpTo(HorseSpringArm->GetComponentRotation().Pitch, TargetCameraArmRotation, DeltaTime, 0.00001);
	HorseSpringArm->SetRelativeRotation(FRotator(CurRotPitch, 0, 0));
}

void ABaseHorse::CameraBurst(float AddedLengthToSpring)
{
	bBurstingCamera = true;
	
	TargetCameraArmLength += AddedLengthToSpring;
	GetWorldTimerManager().SetTimer(
		BurstHandle,
		this,
		&ABaseHorse::CameraUnburst,
		0.3f,
		false);
}

void ABaseHorse::CameraUnburst()
{
	bBurstingCamera = false;
	TargetCameraArmLength = 600 + CurrentSpeed * 0.05;
}

void ABaseHorse::CalculateCameraValues()
{
	TargetCameraArmLength = 600 + CurrentSpeed * 0.05;
	TargetCameraArmRotation = -20 + CurrentSpeed * 0.001;
	TargetCameraFOV = 90 + CurrentSpeed * 0.005;
}

void ABaseHorse::CameraShakeMovement(float DeltaTime)
{
	if (CurrentSpeed < SpeedTable[4] - 5 || bCameraJumpMove)
		CameraRollMultiplier = 0;
	
	float NewCameraRoll = UKismetMathLibrary::FInterpTo(HorseCamera->GetRelativeRotation().Roll, CameraRollMultiplier, DeltaTime, bCameraRoll ? 1.25 : 2.5);
	HorseCamera->SetRelativeRotation(FRotator(HorseCamera->GetRelativeRotation().Pitch, HorseCamera->GetRelativeRotation().Yaw, NewCameraRoll));
}

void ABaseHorse::StartCameraShakeLanded()
{
	bCameraGoingDown = true;
	bCameraJumpMove = true;
	HorseCamera->SetRelativeRotation(FRotator(HorseCamera->GetRelativeRotation().Pitch, HorseCamera->GetRelativeRotation().Yaw, 0));
	GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "LANDED");
}

void ABaseHorse::CameraShakeLanded(float DeltaTime)
{
	float NewZLoc = UKismetMathLibrary::FInterpTo(HorseCamera->GetRelativeLocation().Z, bCameraGoingDown ? -60 : 0, DeltaTime, 10);
	HorseCamera->SetRelativeLocation(FVector(0, 0, NewZLoc));
	
	if (HorseCamera->GetRelativeLocation().Z <= -50.0f)
		bCameraGoingDown = false;
	
	if (HorseCamera->GetRelativeLocation().Z >= -0.5 && !bCameraGoingDown)
	{
		bCameraJumpMove = false;
		HorseCamera->SetRelativeLocation(FVector(0, 0, 0));
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Yellow, "HAHA");

	}
}

	// ========================
	// ==        Jump        ==
	// ========================

void ABaseHorse::ChargeJump(float DeltaTime)
{
	// DON'T WASTE TIME IF YOU'RE READY TO JUMP
	if (!bIsChargingJump || JumpCharge == 1.0f || !GetCharacterMovement()->IsMovingOnGround())
		return;

	// MAKE SURE YOU CAN'T GO HIGHER THAN 100%
	if (JumpCharge > 1.0f)
	{
		JumpCharge = 1.0f;
		return;
	}

	// APPLY PERCENTAGE CALC
	JumpCharge = FMath::Clamp(JumpCharge + (DeltaTime / 1.5f), 0.0f, 1.0f);

	// MAKE SURE TO CHANGE JUMP VELOCITY ONLY IF YOU CAN JUMP
	if (JumpCharge < 0.7)
		return;

	// CHANGE JUMP Z VELOCITY
	GetCharacterMovement()->JumpZVelocity = 450 * JumpCharge;
}

void ABaseHorse::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	Landed_Blueprint();
	StartCameraShakeLanded();

	bHasJustJumped = true;
}

void ABaseHorse::Landed_Blueprint_Implementation()
{
}

	// ==========================
	// ==       End Game       ==
	// ==========================

void ABaseHorse::FinishRace()
{
	SetTargetSpeed(1);
	CreateWidgetFinish();
}
	
	// =========================
	// ==       Respawn       ==
	// =========================

void ABaseHorse::Respawn()
{
	if (bIsRagdoll)
		CeaseRagdoll();
	
	SetActorLocation(RespawnPoint.GetLocation());
	GetController()->SetControlRotation(FRotator(RespawnPoint.GetRotation()));
	SetInitialCameraValues();
}

	// =========================
	// ==       Ragdoll       ==
	// =========================

void ABaseHorse::VFXOnHit_Implementation(bool bIsHorseKill)
{
}

void ABaseHorse::BeginRagdoll()
{
	bIsRagdoll = true;
	HorseSkel->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	HorseSkel->SetSimulatePhysics(true);
	RiderSkel->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	RiderSkel->SetSimulatePhysics(true);
	GetCharacterMovement()->StopMovementImmediately();
	TimeBeginsSlow();
	
	SetTargetSpeed(1);
	VFXOnHit(true);

	GetWorldTimerManager().SetTimer(
		RagdollTimerHandle,
		this,
		&ABaseHorse::Respawn,
		1.0f,
		false);
}

void ABaseHorse::CeaseRagdoll()
{
	bIsRagdoll = false;
	HorseSkel->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	HorseSkel->SetSimulatePhysics(false);
	HorseSkel->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	HorseSkel->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	
	RiderSkel->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	RiderSkel->SetSimulatePhysics(false);
	RiderSkel->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	RiderSkel->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
}

void ABaseHorse::TimeBeginsSlow_Implementation()
{
}

void ABaseHorse::TimeManipulation(float GameSpeed)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GameSpeed);

	HorseSpringArm->TargetArmLength = 300.0f + GameSpeed * 300.0f;
	TargetCameraArmLength = 300.0f + GameSpeed * 300.0f;

	if (GameSpeed == 1.0f)
	{
		HorseSpringArm->TargetArmLength = 600.0f;
		TargetCameraArmLength = 600.0f;
	}
}

	// ========================
	// ==       Damage       ==
	// ========================

void ABaseHorse::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (FVector2D(Hit.ImpactNormal.X, Hit.ImpactNormal.Y).Length() < 0.35 || bIsRagdoll)
		return;
		
	FVector2D NormalHitSave2D = FVector2D(Hit.ImpactNormal.X, Hit.ImpactNormal.Y);
	FVector2D ForwardVector2D = FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y);
	FVector2D RightVector2D = FVector2D(GetActorRightVector().X, GetActorRightVector().Y);
	
	if (FVector2D::DotProduct(NormalHitSave2D, ForwardVector2D) < -0.5)
	{
		// HEADSHOT!
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			if (CurrentSpeed > SpeedTable[3])
			{
				BeginRagdoll();
			}
			else
			{
				GetCharacterMovement()->StopMovementImmediately();
				SetTargetSpeed(FMath::Clamp(TargetSpeed, 0, 1));
				VFXOnHit(false);
			}
		}
		else
		{
			BeginRagdoll();
		}
	}
	else if (FVector2D::DotProduct(NormalHitSave2D, ForwardVector2D) <= 0.0)
	{
		// SIDE HIT
		AddControllerYawInput(FVector2D::DotProduct(NormalHitSave2D, RightVector2D) > 0 ? 0.5 : -0.5);
		
		GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity.RotateAngleAxis(FVector2D::DotProduct(NormalHitSave2D, RightVector2D) > 0 ? 0.5 : -0.5, FVector(0,0,1));

		if (CurrentSpeed > SpeedTable[3] && CurrentSpeedIndex > 4)
		{
			CameraBurst(-150);
			SetTargetSpeed(FMath::Clamp(CurrentSpeedIndex, 0, 4));
			PauseShiftSpeedPossibility(1.0f);
		}
		VFXOnHit(false);
	}
	else
	{
		// BACK HIT
	}
}
	
	// =========================
	// ==         VFX         ==
	// =========================

void ABaseHorse::TweakSpeedFX_Implementation(float SpawnRate, FVector Windspeed)
{
}

	// =========================
	// ==       Widgets       ==
	// =========================

void ABaseHorse::CreateWidgetRace_Implementation(){}

void ABaseHorse::DeleteWidgetRace_Implementation(){}

void ABaseHorse::CreateWidgetFinish_Implementation(){}
 
void ABaseHorse::DeleteWidgetFinish_Implementation(){}

void ABaseHorse::Widget_ShowCharge_Implementation()
{
}

void ABaseHorse::Widget_HideCharge_Implementation()
{
}