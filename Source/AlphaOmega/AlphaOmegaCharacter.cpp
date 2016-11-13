// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AlphaOmega.h"
#include "AlphaOmegaCharacter.h"
#include "Pickup.h" 
#include "BatteryPickup.h"

//////////////////////////////////////////////////////////////////////////
// AAlphaOmegaCharacter

AAlphaOmegaCharacter::AAlphaOmegaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

												   // Create the collection Sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	//CollectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform, EAttachLocation::KeepWorldPosition);
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Set a base power level for the character
	InitialPower = 2000.f;
	CharacterPower = InitialPower;

	// Set the dependence of the speed on the power level
	SpeedFactor = 0.75f;
	BaseSpeed = 10.f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAlphaOmegaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &AAlphaOmegaCharacter::CollectPickups);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAlphaOmegaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAlphaOmegaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAlphaOmegaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAlphaOmegaCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AAlphaOmegaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AAlphaOmegaCharacter::TouchStopped);
}

void AAlphaOmegaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AAlphaOmegaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AAlphaOmegaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAlphaOmegaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAlphaOmegaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAlphaOmegaCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AAlphaOmegaCharacter::CollectPickups() {


	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// keep track of the collected battery power
	float CollectedPower = 0;

	// For each actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected) {

		// Cast the actor to APickup
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
		// If the cast is succesfull and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive()) {

			// Call the pickup's WasCollected function
			TestPickup->WasCollected();

			// Check to see if the pickup is also a battery
			ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(TestPickup);

			if (TestBattery) {

				// Increase the collected power
				CollectedPower += TestBattery->GetPower();
			}
			// Deactivate the pickup
			TestPickup->SetActive(false);
		}
	}
	if (CollectedPower > 0) {

		UpdatePower(CollectedPower);
		Collecting = true;
	}
}

// Reports starting power
float AAlphaOmegaCharacter::GetInitialPower() {
	return InitialPower;
}

// Reports current power
float AAlphaOmegaCharacter::GetCurrentPower() {
	return CharacterPower;
}

// Sets the new power level
void AAlphaOmegaCharacter::UpdatePower(float PowerChange) {

	// Change power
	CharacterPower += PowerChange;

	// Change speed based on power
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + SpeedFactor * CharacterPower;

	// Call visual effect
	PowerChangeEffect();
}

// Reports if the player is collecting
bool AAlphaOmegaCharacter::IsCollecting() {
	return Collecting;
}

// Sets the new state to is collecting
void AAlphaOmegaCharacter::UpdateCollecting(bool NewState) {

	// Set new state
	Collecting = NewState;
}