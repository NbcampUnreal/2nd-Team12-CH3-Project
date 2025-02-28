#include "BTD_HasPatrolRoute.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_HasPatrolRoute::UBTD_HasPatrolRoute()
{
	bCreateNodeInstance = true;
	NodeName = TEXT("Has Patrol Route");
}

bool UBTD_HasPatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// AIController 가져오기
	ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
	if (!AIC)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTD_HasPatrolRoute: AIController is NULL!"));
		return false;
	}

	// 컨트롤된 Pawn을 SPTEnemyCharacter로 캐스팅
	ASPTEnemyCharacter* EnemyCharacterRef = Cast<ASPTEnemyCharacter>(AIC->GetPawn());
	if (!EnemyCharacterRef)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTD_HasPatrolRoute: Controlled Pawn is not a SPTEnemyCharacter!"));
		return false;
	}

	// PatrolRoute 유효성 검사
	if (!IsValid(EnemyCharacterRef->PatrolRoute))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTD_HasPatrolRoute: PatrolRoute is NULL!"));
		return false;
	}

	return true;
}
