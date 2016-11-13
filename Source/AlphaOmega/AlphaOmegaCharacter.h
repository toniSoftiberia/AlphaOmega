// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Pickup.h"
#include "AlphaOmegaCharacter.generated.h"

UCLASS(config=Game)
class AAlphaOmegaCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;


	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;
public:
	AAlphaOmegaCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** Called when we press a keyto collect any pickups inside the COllectionSphere*/
	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void CollectPickups();

	/** The starting power level of our character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float InitialPower;

	/** Multiplier for our character speed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float SpeedFactor;

	/** Speed when the character level = 0*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float BaseSpeed;

	/** Handles if the playar is collecting*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickups", Meta = (BlueprintProtected = "true"))
		bool Collecting;

	UFUNCTION(BlueprintImplementableEvent, Category = "Power")
		void PowerChangeEffect();

private:
	/** Current power level of our character*/
	UPROPERTY(VisibleAnywhere, Category = "Power")
		float CharacterPower;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns CollectionSphere subobject **/
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }


	/** Accessor funtion for initial power*/
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetInitialPower();

	/** Accessor funtion for current power*/
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetCurrentPower();

	/** Accessor funtion for collecting state*/
	UFUNCTION(BlueprintPure, Category = "Pickups")
		bool IsCollecting();

	/** Function to update the character's power
	* @param PowerChange This is the amount to change the power by, and it can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdatePower(float PowerChange);

	UFUNCTION(BlueprintCallable, Category = "Pickups")
		void UpdateCollecting(bool NewState);
};

