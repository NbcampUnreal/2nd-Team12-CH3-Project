#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"


ASPTEnemyCharacter::ASPTEnemyCharacter()
{
	AIControllerClass = ASPTEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	IdleSpeed = 0.0f;
    WalkSpeed = 30.0f;
	JogSpeed = 200.0f;
}

void ASPTEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	//TryAttack.AddUObject(this, &ASPTEnemyCharacter::Try);
	//TryAttack.Broadcast();
}

void ASPTEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPTEnemyCharacter::SetMovementSpeed(EMovementSpeed NewSpeed)
{
    switch (NewSpeed)
    {
    case EMovementSpeed::Idle:
        GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;
        break;
    case EMovementSpeed::Walking:
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
        break;
    case EMovementSpeed::Jogging:
        GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
        break;
    case EMovementSpeed::Sprinting:
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
        break;
    default:
        break;
    }
}

void ASPTEnemyCharacter::Attack()
{
	// 기본 공격 로직
	UE_LOG(LogTemp, Warning, TEXT("Base Enemy Attack Triggered"));
}

void ASPTEnemyCharacter::EquippedWeapon()
{
	IsEquippedWeapon = true;
	UE_LOG(LogTemp, Warning, TEXT("Enemy has Weapon"));
}

void ASPTEnemyCharacter::UnEquippedWeapon()
{
	IsEquippedWeapon = false;
	UE_LOG(LogTemp, Warning, TEXT("Enemy has not Weapon"));
}

//void ASPTEnemyCharacter::Try()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Try"));
//}