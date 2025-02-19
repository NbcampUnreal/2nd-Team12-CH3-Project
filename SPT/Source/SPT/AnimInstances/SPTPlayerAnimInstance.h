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
};
