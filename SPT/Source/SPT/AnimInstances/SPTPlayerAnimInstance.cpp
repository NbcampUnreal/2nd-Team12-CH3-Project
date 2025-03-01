// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerAnimInstance.h"
#include "SPTPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

USPTPlayerAnimInstance::USPTPlayerAnimInstance()
{
	Character = nullptr;
	CharacterMovement = nullptr;

	Velocity = FVector::ZeroVector;
	GroundSpeed = 0.f;
	GroundDierction = 0.f;
	AimYaw = 0.f;
	AimPitch = 0.f;
	TurnInPlaceYaw = 0.f;
	RotationScalar = 2.5f;
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
		// ���� ĳ������ �ӵ� ����
		Velocity = CharacterMovement->GetLastUpdateVelocity();
		// ���� ĳ���Ͱ� �������� �̵����� �ӵ�
		GroundSpeed = Velocity.Size2D();
		// ���� ĳ���Ͱ� �̵����� ����
		GroundDierction = CalculateDirection(Velocity, Character->GetActorRotation());
		// ���� ĳ���Ͱ� �̵� ������ ����
		// ���� ĳ���Ͱ� �÷��̾��� ������ ���� ���
		if (TryGetPawnOwner()->IsPlayerControlled())
		{
			bShouldMove = !FMath::IsNearlyZero(CharacterMovement->GetCurrentAcceleration().SizeSquared()) ? true : false;
		}
		// ���� ĳ���Ͱ� AI�� ������ ���� ���
		else
		{
			bShouldMove = !FMath::IsNearlyZero(GroundSpeed) ? true : false;
		}
		// ���� ĳ���Ͱ� ���� ������ ����
		bIsFalling = CharacterMovement->IsFalling();
		// ���� ĳ���Ͱ� �ɾ� �ִ��� ����
		bIsCrouching = CharacterMovement->IsCrouching();

		// 
		FRotator TargetRotate = Character->GetControlRotation() - Character->GetActorRotation();

		FRotator CurrentRotate(AimPitch, AimYaw, 0.f);

		FRotator DeltaRotate = UKismetMathLibrary::RInterpTo(CurrentRotate, TargetRotate, DeltaSeconds, 15.f);

		AimYaw = UKismetMathLibrary::ClampAngle(DeltaRotate.Yaw, -90.f, 90.f);
		AimPitch = UKismetMathLibrary::ClampAngle(DeltaRotate.Pitch, -90.f, 90.f);

		TurnInPlaceYaw = UKismetMathLibrary::ClampAngle(DeltaRotate.Yaw, -90.f, 90.f);

		float TurnYawWeight = (GetCurveValue(FName("TurnYawWeight")) > 0);
		if (TurnYawWeight > 0.f)
		{
			FRotator TurnYaw = FRotator::ZeroRotator;
			TurnYaw.Yaw = GetCurveValue(FName("RemainingTurnYaw")) * DeltaSeconds * RotationScalar;
			Character->AddActorLocalRotation(TurnYaw);
		}
		
	}
}

void USPTPlayerAnimInstance::AnimNotify_Death()
{
	// ���� ĳ���Ͱ� �÷��̾��� ������ ���� ���
	if (TryGetPawnOwner()->IsPlayerControlled())
	{
		Character->GetMesh()->SetVisibility(false);
	}
	// ���� ĳ���Ͱ� AI�� ������ ���� ���
	else
	{
		Character->Destroy();
	}
	
}
