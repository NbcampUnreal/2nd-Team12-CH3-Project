#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"

ASPTEnemyCharacter::ASPTEnemyCharacter()
{
	AIControllerClass = ASPTEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	IdleSpeed = 0.0f;
	JogSpeed = 450.0f;
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

void ASPTEnemyCharacter::Attack()
{
	// 기본 공격 로직
	UE_LOG(LogTemp, Warning, TEXT("Base Enemy Attack Triggered"));
}

//void ASPTEnemyCharacter::Try()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Try"));
//}