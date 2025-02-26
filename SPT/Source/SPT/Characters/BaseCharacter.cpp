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

	// �ɱ� ��� ����
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	// �ɱ� �� ���� �ɱ� �ִϸ��̼ǿ� �´� ���� ���� ����
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
	// ��� �� ����
	// ���ε� �� �Լ� ȣ��
	OnDethMulticastDelegate.Broadcast();

	// ���ε� �� �Լ��� ���� ���� ����
	OnDethMulticastDelegate.Clear();
}