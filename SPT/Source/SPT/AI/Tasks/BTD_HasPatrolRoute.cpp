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
	// AIController ��������
	ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
	if (!AIC)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTD_HasPatrolRoute: AIController is NULL!"));
		return false;
	}

	// ��Ʈ�ѵ� Pawn�� SPTEnemyCharacter�� ĳ����
	ASPTEnemyCharacter* EnemyCharacterRef = Cast<ASPTEnemyCharacter>(AIC->GetPawn());
	if (!EnemyCharacterRef)
	{
		UE_LOG(LogTemp, Error, TEXT("UBTD_HasPatrolRoute: Controlled Pawn is not a SPTEnemyCharacter!"));
		return false;
	}

	// PatrolRoute ��ȿ�� �˻�
	if (!IsValid(EnemyCharacterRef->PatrolRoute))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTD_HasPatrolRoute: PatrolRoute is NULL!"));
		return false;
	}

	return true;
}
