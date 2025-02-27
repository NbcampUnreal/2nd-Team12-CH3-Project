#include "BTT_EquipWeapon.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_EquipWeapon::UBTT_EquipWeapon()
{
	bCreateNodeInstance = true;
	NodeName = TEXT("BTT_Equip Weapon");
};

EBTNodeResult::Type UBTT_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
            // 적 타입에 맞게 EquippedWeapon 호출
            EnemyCharacterRef->EquippedWeapon();

            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}
