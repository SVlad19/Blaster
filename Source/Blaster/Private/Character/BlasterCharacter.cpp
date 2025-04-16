// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Config/InputDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"
#include "BlasterComponent/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/BlasterAnimInstance.h"


ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (IsLocallyControlled()) {
		if (OverlappingWeapon) {
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

AWeapon* ABlasterCharacter::GetEquippedWeapon()
{
	if (!Combat) {
		return nullptr;
	}

	return Combat->EquippedWeapon.Get();
}

FVector ABlasterCharacter::GetHitTarget() const
{
	if (!Combat) {
		return FVector();
	}

	return Combat->HitTarget;
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(InputDataAsset);
	check(Combat);

	if (APlayerController* PlayerController = GetController<APlayerController>()) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(InputDataAsset->InputMapping.LoadSynchronous(), 0);
		}
	}
}

void ABlasterCharacter::AimOffset(float DeltaTime)
{
	if (Combat && !Combat->EquippedWeapon.IsValid()) {
		return;
	}

	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) {
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;

		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning) {
			InterpAO_Yaw = AO_Yaw;
		}

		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}

	if (Speed > 0.f || bIsInAir) {
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled()) {
		// Map pitch from [270;360) to [-90;0]
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);

		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);

	}
}

void ABlasterCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f) {
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if(AO_Yaw < -90.f){
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning) {
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;

		if (FMath::Abs(AO_Yaw) < 15.f) {
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}

}

void ABlasterCharacter::PlayFireMontage(bool bAiming)
{
	if (!Combat || !Combat->EquippedWeapon.IsValid()) {
		return;
	}

	TWeakObjectPtr<UAnimInstance> AnimInstance = MakeWeakObjectPtr(GetMesh()->GetAnimInstance());
	if (AnimInstance.IsValid() && FireWeaponMontage) {
		AnimInstance->Montage_Play(FireWeaponMontage);

		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Combat->Character = MakeWeakObjectPtr(this);
}

void ABlasterCharacter::Jump()
{
	if (bIsCrouched) {
		UnCrouch();
	}
	else {
		Super::Jump();
	}

}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon.IsValid());
}

bool ABlasterCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_AutonomousOnly);
}


void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(InputDataAsset->MoveInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Move);
		Input->BindAction(InputDataAsset->LookInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Look);
		Input->BindAction(InputDataAsset->JumpInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Jumping);
		Input->BindAction(InputDataAsset->EquipInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Equip);
		Input->BindAction(InputDataAsset->CrouchInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Crouching);
		Input->BindAction(InputDataAsset->AimInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Aiming);
		Input->BindAction(InputDataAsset->FireInput, ETriggerEvent::Triggered, this, &ABlasterCharacter::Fire);
	}
}


void ABlasterCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ABlasterCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(-LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ABlasterCharacter::Jumping(const FInputActionValue& Value)
{
	Jump();
}

void ABlasterCharacter::Equip(const FInputActionValue& Value)
{
	if (Combat) {
		if (HasAuthority()) {
			Combat->EquipWeapon(OverlappingWeapon.Get());
		}
		else {
			ServerEquipButtonPressed();
		}
	}
}

void ABlasterCharacter::Crouching(const FInputActionValue& Value)
{
	const bool Hold = Value.Get<bool>();

	if (Hold) {
		Crouch();
	}
	else {
		UnCrouch();
	}
}

void ABlasterCharacter::Aiming(const FInputActionValue& Value)
{
	const bool Hold = Value.Get<bool>();

	if (Hold) {
		Combat->SetAiming(true);
	}
	else {
		Combat->SetAiming(false);
	}
}

void ABlasterCharacter::Fire(const FInputActionValue& Value)
{
	if (Combat) {
		const bool Pressed = Value.Get<bool>();

		Combat->Fire(Pressed);
	}
}

void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat) {
		Combat->EquipWeapon(OverlappingWeapon.Get());
	}
}

void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if (LastWeapon) {
		LastWeapon->ShowPickupWidget(false);
	}
}

