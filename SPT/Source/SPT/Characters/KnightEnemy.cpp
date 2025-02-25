#include "KnightEnemy.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

AKnightEnemy::AKnightEnemy()
{
}

void AKnightEnemy::BeginPlay()
{
    // AI ��Ʈ�ѷ��� ���� ������ ��������
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(GetController());
    if (AIC && AIC->BBC)
    {
        // �������� EnemyType ���� Knight���� ����
        AIC->BBC->SetValueAsEnum(FName("EnemyType"), 1); // 1 = Knight (Enum ��)
        UE_LOG(LogTemp, Warning, TEXT("Knight EnemyType Set in Blackboard!"));
    }
}

void AKnightEnemy::Attack()
{
	Super::Attack();

	UE_LOG(LogTemp, Warning, TEXT("Knight Enemy Attack!"));

}
