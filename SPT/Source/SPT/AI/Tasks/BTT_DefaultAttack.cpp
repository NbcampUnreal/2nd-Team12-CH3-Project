#include "BTT_DefaultAttack.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_DefaultAttack::UBTT_DefaultAttack()
{
    // �� Behavior Tree ���ึ�� �������� Task Node �ν��Ͻ��� ����
    bCreateNodeInstance = true;
    NodeName = TEXT("BTT_Default Attack");
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerCompRef = &OwnerComp;

    // AI ��Ʈ�ѷ��� ������ ��������
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
    
    if (!AIC || !BBC)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController or BlackboardComponent is NULL!"));
        return EBTNodeResult::Failed;
    }

    // Pawn�� SPTEnemyCharacter Ÿ������ Ȯ�� �� ĳ����
    EnemyCharacterRef = Cast<ASPTEnemyCharacter>(AIC->GetPawn());
    if (EnemyCharacterRef)
    {
        // ���� ���� �̺�Ʈ ���ε�
        EnemyCharacterRef->OnAttackEnd.AddDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);

        // ���� ���� �α�
        UE_LOG(LogTemp, Warning, TEXT("[DefaultAttack] %s Starts Attack"), *EnemyCharacterRef->GetName());

        // �������� Ȱ���Ͽ� �ش� �� Ŭ������ Attack() ȣ��
        EnemyCharacterRef->Attack();

        return EBTNodeResult::InProgress;  // ���� �� ����
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyCharacter is NULL or of unknown type!"));
        return EBTNodeResult::Failed;
    }
}

void UBTT_DefaultAttack::OnAttackFinished()
{
    if (EnemyCharacterRef)
    {
        // ����� �� �ߺ� ���ε� ����
        EnemyCharacterRef->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
    }

    if (OwnerCompRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("DefaultAttack Task is done!"));
        FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
    }
}
