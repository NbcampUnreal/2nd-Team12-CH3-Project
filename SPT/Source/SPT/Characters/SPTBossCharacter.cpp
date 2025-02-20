#include "SPTBossCharacter.h"
#include "SPTBossCharacterAIController.h"

// Sets default values
ASPTBossCharacter::ASPTBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// �� ĳ���͸� ��Ʈ���� AIController Ŭ������ EnemyAIController�� ����
	AIControllerClass = ASPTBossCharacterAIController::StaticClass();
	// ������ ��ġ�ǰų� �����Ǹ� �ڵ����� AIController�� �Ҵ�ǵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASPTBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPTBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASPTBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

