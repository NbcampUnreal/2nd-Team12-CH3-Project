#include "BTT_UnEquipWeapon.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_UnEquipWeapon::UBTT_UnEquipWeapon()
{
	bCreateNodeInstance = true;
	NodeName = TEXT("BTT_UnEquip Weapon");
}

EBTNodeResult::Type UBTT_UnEquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

    if (!AIC || !BBC)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController or BlackboardComponent is NULL!"));
        return EBTNodeResult::Failed;
    }

    if (AIC && AIC->BBC)
    {
        EnemyCharacterRef = Cast<ASPTEnemyCharacter>(AIC->GetPawn());
        if (EnemyCharacterRef)
        {
            // 적 타입에 맞게 UnEquippedWeapon 호출
            EnemyCharacterRef->UnEquippedWeapon();

            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
