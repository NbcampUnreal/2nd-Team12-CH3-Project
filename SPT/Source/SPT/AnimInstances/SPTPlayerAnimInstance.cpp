// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerAnimInstance.h"
#include "SPTPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USPTPlayerAnimInstance::USPTPlayerAnimInstance()
{
	Character = nullptr;
	CharacterMovement = nullptr;

	Velocity = FVector::ZeroVector;
	GroundSpeed = 0.f;
	GroundDierction = 0.f;
	bShouldMove = false;
	bIsFalling = false;
	bIsCrouching = false;
}

void USPTPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Character = Cast<ACharacter>(GetOwningActor()))
	{
		CharacterMovement = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
	}
}

void USPTPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CharacterMovement && Character)
	{
		// 현재 캐릭터의 속도 벡터
		Velocity = CharacterMovement->GetLastUpdateVelocity();
		// 현재 캐릭터가 수평으로 이동중인 속도
		GroundSpeed = Velocity.Size2D();
		// 현재 캐릭터가 이동중인 방향
		GroundDierction = CalculateDirection(Velocity, Character->GetActorRotation());
		// 현재 캐릭터가 이동 중인지 여부
		// 현재 캐릭터가 플레이어의 조종을 받을 경우
		if (TryGetPawnOwner()->IsPlayerControlled())
		{
			bShouldMove = !FMath::IsNearlyZero(CharacterMovement->GetCurrentAcceleration().SizeSquared()) ? true : false;
		}
		// 현재 캐릭터가 AI의 조종을 받을 경우
		else
		{
			bShouldMove = !FMath::IsNearlyZero(GroundSpeed) ? true : false;
		}
		// 현재 캐릭터가 낙하 중인지 여부
		bIsFalling = CharacterMovement->IsFalling();
		// 현재 캐릭터가 앉아 있는지 여부
		bIsCrouching = CharacterMovement->IsCrouching();
	}
}
