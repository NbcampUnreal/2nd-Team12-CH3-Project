// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SPTPlayerController.h"
#include "PlayerMainHUD.h"


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

	float NewHealth = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	SetHealth(NewHealth);

	// �÷��̾ �������� �ذŶ�� �÷��̾��� UI�� HitMarker�� ���
	if (EventInstigator)
	{
		UE_LOG(LogTemp, Log, TEXT("EventInstigator"));
		ASPTPlayerController* PlayerController = Cast<ASPTPlayerController>(EventInstigator);
		if (PlayerController)
		{
			UE_LOG(LogTemp, Log, TEXT("PlayerController"));
			UPlayerMainHUD* PlayerMainHUD = PlayerController->GetHUDWidgetInstance();
			PlayerMainHUD->HitTarget();
		}
	}

	if (Health <= 0.f)
	{
		OnDeath();
	}

	return 0.0f;
}

void ABaseCharacter::OnDeath()
{
	// �ݸ��� ���� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("DeathCharacter"));

	// ��� �ִϸ��̼� ���
	PlayDeathAnim();

	// ��� �� ����
	// ���ε� �� �Լ� ȣ��
	OnDeathDelegate.Broadcast();

	// ���ε� �� �Լ��� ���� ���� ����
	OnDeathDelegate.Clear();
}

void ABaseCharacter::SetHealth(float NewHP)
{
	Health = NewHP;

	OnHealthChangedDelegate.Broadcast(NewHP, MaxHealth);
}

void ABaseCharacter::PlayDeathAnim()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(DeathAnim);
	}
}
