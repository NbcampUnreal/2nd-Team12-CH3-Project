// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	// 앉기 기능 설정
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	// 앉기 시 현재 앉기 애니메이션에 맞는 절반 높이 설정
	GetCharacterMovement()->SetCrouchedHalfHeight(65.f);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
