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

	float NewHealth = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	SetHealth(NewHealth);

	// 플레이어가 데미지를 준거라면 플레이어의 UI에 HitMarker를 띄움
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
	// 콜리전 설정 변경
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeathCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("DeathCharacter"));

	// 사망 애니메이션 재생
	PlayDeathAnim();

	// 사망 후 로직
	// 바인드 된 함수 호출
	OnDeathDelegate.Broadcast();

	// 바인드 된 함수에 대해 연결 해제
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
