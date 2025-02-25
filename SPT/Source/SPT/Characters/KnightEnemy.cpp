#include "KnightEnemy.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

AKnightEnemy::AKnightEnemy()
{
}

void AKnightEnemy::BeginPlay()
{
    // AI 컨트롤러를 통해 블랙보드 가져오기
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(GetController());
    if (AIC && AIC->BBC)
    {
        // 블랙보드의 EnemyType 값을 Knight으로 설정
        AIC->BBC->SetValueAsEnum(FName("EnemyType"), 1); // 1 = Knight (Enum 값)
        UE_LOG(LogTemp, Warning, TEXT("Knight EnemyType Set in Blackboard!"));
    }
}

void AKnightEnemy::Attack()
{
	Super::Attack();

	UE_LOG(LogTemp, Warning, TEXT("Knight Enemy Attack!"));

}
