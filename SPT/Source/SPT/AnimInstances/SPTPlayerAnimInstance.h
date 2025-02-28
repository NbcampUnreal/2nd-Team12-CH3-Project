// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPTPlayerAnimInstance.generated.h"

class UCharacterMovementComponent;

UCLASS()
class SPT_API USPTPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USPTPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_Death();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<ACharacter> Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float GroundDierction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float AimYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float AimPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float TurnInPlaceYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float RotationScalar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bIsCrouching;
};
