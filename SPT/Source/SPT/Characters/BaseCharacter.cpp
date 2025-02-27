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

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
	

	if (Health <= 0.f)
	{
		OnDeath();
	}

	return 0.0f;
}

void ABaseCharacter::OnDeath()
{
	// 사망 후 로직
	// 바인드 된 함수 호출
	OnDethMulticastDelegate.Broadcast();

	// 바인드 된 함수에 대해 연결 해제
	OnDethMulticastDelegate.Clear();
}