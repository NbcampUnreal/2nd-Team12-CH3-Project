#include "BTT_DefaultAttack.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_DefaultAttack::UBTT_DefaultAttack()
{
    // 각 Behavior Tree 실행마다 독립적인 Task Node 인스턴스를 생성
    bCreateNodeInstance = true;
    NodeName = TEXT("BTT_Default Attack");
}

EBTNodeResult::Type UBTT_DefaultAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerCompRef = &OwnerComp;

    // AI 컨트롤러와 블랙보드 가져오기
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
    
    if (!AIC || !BBC)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController or BlackboardComponent is NULL!"));
        return EBTNodeResult::Failed;
    }

    // Pawn이 SPTEnemyCharacter 타입인지 확인 후 캐스팅
    EnemyCharacterRef = Cast<ASPTEnemyCharacter>(AIC->GetPawn());
    if (EnemyCharacterRef)
    {
        // 공격 종료 이벤트 바인딩
        EnemyCharacterRef->OnAttackEnd.AddDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);

        // 공격 시작 로그
        UE_LOG(LogTemp, Warning, TEXT("[DefaultAttack] %s Starts Attack"), *EnemyCharacterRef->GetName());

        // 다형성을 활용하여 해당 적 클래스의 Attack() 호출
        EnemyCharacterRef->Attack();

        return EBTNodeResult::InProgress;  // 공격 중 상태
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
        // 재실행 시 중복 바인딩 방지
        EnemyCharacterRef->OnAttackEnd.RemoveDynamic(this, &UBTT_DefaultAttack::OnAttackFinished);
    }

    if (OwnerCompRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("DefaultAttack Task is done!"));
        FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
    }
}
