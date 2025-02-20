#include "SPTBossCharacter.h"
#include "SPTBossCharacterAIController.h"

// Sets default values
ASPTBossCharacter::ASPTBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// 이 캐릭터를 컨트롤할 AIController 클래스를 EnemyAIController로 지정
	AIControllerClass = ASPTBossCharacterAIController::StaticClass();
	// 레벨에 배치되거나 스폰되면 자동으로 AIController가 할당되도록 설정
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

