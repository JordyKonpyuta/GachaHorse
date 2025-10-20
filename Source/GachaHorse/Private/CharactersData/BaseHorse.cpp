// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactersData/BaseHorse.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
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

	// FUNCTIONS

	// ????? POURQUOI TU MARCHES PLUS?
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseHorse::OnHit);
}

void ABaseHorse::BeginPlay()
{
	Super::BeginPlay();

	// ????? CA FIXE LE RAGDOLL JE SUPPOSE
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseHorse::OnHit);

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

	CreateWidgetRace();
	
	GetCharacterMovement()->MaxWalkSpeed = 50000.0f;
}

void ABaseHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(1 / DeltaTime));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Emerald, FString::SanitizeFloat(CurrentSpeed));

	// ALWAYS CHECK SLOPE
	SlopeCheck();

	// CHARGE JUMP
	ChargeJump(DeltaTime);

	// MOVE HORSEY FORWARD
	CalculateCurrentSpeed();

	if (!GetCharacterMovement()->IsMovingOnGround() && bIsChargingJump)
	{
		bIsChargingJump = false;
		JumpCharge = 0.0f;
	}

	if (!((1 + SlopeType * 0.15) * TargetSpeed - 1  < CurrentSpeed && CurrentSpeed < (1 + SlopeType * 0.15) * TargetSpeed + 1))
		if (!bIsRagdoll && GetCharacterMovement()->IsMovingOnGround())
			AddMovementInput(GetActorForwardVector(), SlopeType * 0.5 + (CurrentSpeed <= TargetSpeed * (1 + SlopeType * 0.15) ? 1 : -1), false);

	// MOVE HORSEY LEFTY RIGHTY
	if (!(-0.1 < SideSpeed && SideSpeed < 0.1))
		AddMovementInput(GetActorRightVector(), (SideSpeed < 0 ? 2 : -2) * DeltaTime * TickCorrecter);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::SanitizeFloat((SideSpeed < 0 ? 40 : -40) * DeltaTime));
	
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

	
	// GAME END :

	if (bGameEnded)
	{
		HorseSpringArm->TargetArmLength = FMath::FInterpTo(HorseSpringArm->TargetArmLength, 200, DeltaTime, 200);
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

		// Jumping
		EnhancedInputComponent->BindAction(Jump_Action, ETriggerEvent::Started, this, &ABaseHorse::PrepareJump);
		EnhancedInputComponent->BindAction(Jump_Action, ETriggerEvent::Completed, this, &ABaseHorse::ReleaseJump);

		// Speed
		EnhancedInputComponent->BindAction(Speed_Action, ETriggerEvent::Started, this, &ABaseHorse::ChangeSpeed);
	}
}
	
	// =========================
	// ==      Movements      ==
	// =========================

void ABaseHorse::Turn(const FInputActionValue& Value)
{
	if (bIsRagdoll)
		return;

	float TurnAngle = (Value.Get<float>() * TurnRateFactor) / (bIsChargingJump ? 3 : 1) / (GetCharacterMovement()->IsMovingOnGround() ? 1 : 100);
	TurnAngle *= TickCorrecter;
	
	AddControllerYawInput(TurnAngle * GetWorld()->GetDeltaSeconds());
	GetCharacterMovement()->Velocity = UKismetMathLibrary::RotateAngleAxis(GetCharacterMovement()->Velocity, TurnAngle * GetWorld()->GetDeltaSeconds(), FVector(0, 0, 1));
}

void ABaseHorse::PrepareJump(const FInputActionValue& Value)
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

void ABaseHorse::ReleaseJump(const FInputActionValue& Value)
{
	// DON'T KEEP THE CHARGING IF IN THE AIR
	if (!GetCharacterMovement()->IsMovingOnGround())
	{
		bIsChargingJump = false;
		JumpCharge = 0.0f;
		return;
	}

	if (!bIsRagdoll && CurrentSpeed > 750 && GetCharacterMovement()->JumpZVelocity > 325.0f)
	{
		Jump();
		GetCapsuleComponent()->SetCapsuleSize(34,34,true);
	}
	bIsChargingJump = false;
	JumpCharge = 0.0f;
	Widget_HideCharge();
}

void ABaseHorse::ChangeSpeed(const FInputActionValue& Value)
{
	if (Value.Get<float>() < 0)
	{
		if (CurrentSpeedIndex <= 0)
			return;

		CurrentSpeedIndex -= 1;
		SetTargetSpeed(CurrentSpeedIndex);
	}
	else
	{
		if (CurrentSpeedIndex >= 5)
			return;

		CurrentSpeedIndex += 1;
		SetTargetSpeed(CurrentSpeedIndex);
	}
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

	// =========================
	// ==        Speed        ==
	// =========================

void ABaseHorse::InitAcceleration()
{
	GetCharacterMovement()->MaxAcceleration = 300 + (20 * Stats[0]);
}

void ABaseHorse::InitSpeed()
{
	SpeedTable[5] = 1750 + (Stats[1] * 25);
}

void ABaseHorse::InitHandling()
{
	TurnRateFactor = 0.05 + (Stats[2] * 0.005);
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

void ABaseHorse::SlopeCheck()
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
	RV_TraceParams.bReturnPhysicalMaterial = false;

	//Re-initialize hit info
	FHitResult RV_Hit(ForceInit);
	
	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		GetActorLocation(),
		GetActorLocation() + FVector(0,0,-200),
		ECC_Pawn,
		RV_TraceParams);

	float SlopeTypeCalculation = FVector::DotProduct(RV_Hit.ImpactNormal, GetActorForwardVector());

	// NEARLY EQUAL TO 0?
	if (-0.0001 < SlopeTypeCalculation && SlopeTypeCalculation < 0.0001)
	{
		SlopeType = 0;
	}
	else
	{
		SlopeType = SlopeTypeCalculation > 0 ? 1 : -1;
	}
}

void ABaseHorse::CalculateCurrentSpeed()
{
	CurrentSpeed = FVector2D::DotProduct(FVector2D(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y), FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y));
	SideSpeed = FVector2D::DotProduct(FVector2D(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y), FVector2D(GetActorRightVector().X, GetActorRightVector().Y));
}

	// ========================
	// ==        Jump        ==
	// ========================

void ABaseHorse::ChargeJump(float DeltaTime)
{
	// DON'T WASTE TIME IF YOU'RE READY TO JUMP
	if (!bIsChargingJump || JumpCharge == 1.0f)
		return;

	// MAKE SURE YOU CAN'T GO HIGHER THAN 100%
	if (JumpCharge > 1.0f)
	{
		JumpCharge = 1.0f;
		return;
	}

	// APPLY PERCENTAGE CALC
	JumpCharge = FMath::Clamp(JumpCharge + (DeltaTime / 3), 0.0f, 1.0f);

	// MAKE SURE TO CHANGE JUMP VELOCITY ONLY IF YOU CAN JUMP
	if (JumpCharge < 0.7)
		return;

	// CHANGE JUMP Z VELOCITY
	GetCharacterMovement()->JumpZVelocity = 450 * JumpCharge;
}

void ABaseHorse::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bHasJustJumped = true;
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
}


	// =========================
	// ==       Ragdoll       ==
	// =========================

void ABaseHorse::BeginRagdoll()
{
	bIsRagdoll = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->StopMovementImmediately();

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
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
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

	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH");
	
	if (FVector2D::DotProduct(NormalHitSave2D, ForwardVector2D) < -0.5)
	{
		// HEADSHOT!
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			if (CurrentSpeed > 800)
			{
				BeginRagdoll();
				SetTargetSpeed(1);
			}
			else
			{
				GetCharacterMovement()->StopMovementImmediately();
				SetTargetSpeed(FMath::Clamp(TargetSpeed, 0, 1));
			}
		}
		else
		{
			BeginRagdoll();
			SetTargetSpeed(1);
		}
	}
	else if (FVector2D::DotProduct(NormalHitSave2D, ForwardVector2D) <= 0.0)
	{
		// SIDE HIT
		AddControllerYawInput(FVector2D::DotProduct(NormalHitSave2D, RightVector2D) > 0 ? 0.5 : -0.5);
		
		GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity.RotateAngleAxis(FVector2D::DotProduct(NormalHitSave2D, RightVector2D) > 0 ? 0.5 : -0.5, FVector(0,0,1));

		if (CurrentSpeed > 1000)
		{
			SetTargetSpeed(FMath::Clamp(CurrentSpeedIndex, 0, 4));
			bCanShiftSpeed = false;
			GetWorldTimerManager().SetTimer(
				ShiftSpeedTimerHandle,
				this,
				&ABaseHorse::ResetShiftSpeed,
				1.0,
				false);
		}
	}
	else
	{
		// BACK HIT
	}
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