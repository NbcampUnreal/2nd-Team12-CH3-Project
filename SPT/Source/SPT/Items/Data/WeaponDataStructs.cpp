// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataStructs.h"
#include "GameFramework/SpringArmComponent.h"
#include "SPTPlayerCharacter.h"
#include "Camera/CameraComponent.h"

void FWeaponAimData::SetData(ASPTPlayerCharacter* InOwner)
{
	if (!InOwner)
	{
		return;
	}

	USpringArmComponent* SpringArm = InOwner->FindComponentByClass<USpringArmComponent>();
	if (SpringArm)
	{
		SpringArm->TargetArmLength = TargetArmLength;
		SpringArm->SocketOffset = SocketOffset;
		SpringArm->bEnableCameraLag = bEnableCameraLag;
	}
}

void FWeaponAimData::SetDataByNoneCurve(ASPTPlayerCharacter* InOwner)
{
	if (!InOwner)
	{
		return;
	}

	USpringArmComponent* SpringArm = InOwner->FindComponentByClass<USpringArmComponent>();
	if (SpringArm)
	{
		SpringArm->TargetArmLength = TargetArmLength;
		SpringArm->SocketOffset = SocketOffset;
		SpringArm->bEnableCameraLag = bEnableCameraLag;
	}

	UCameraComponent* Camera = InOwner->FindComponentByClass<UCameraComponent>();
	if (Camera)
	{
		Camera->FieldOfView = FieldOfView;
	}
}
