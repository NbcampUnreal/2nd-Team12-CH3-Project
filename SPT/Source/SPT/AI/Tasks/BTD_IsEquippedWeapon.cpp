#include "BTD_IsEquippedWeapon.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsEquippedWeapon::UBTD_IsEquippedWeapon()
{
    bCreateNodeInstance = true;
	NodeName = TEXT("Is Equipped Weapon");
}

bool UBTD_IsEquippedWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bIsEquippedWeapon = false;

	ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	if (!AIC || !BBC)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController or BlackboardComponent is NULL!"));
        return false;
    }

    APawn* ControlledPawn = AIC->GetPawn();
    if (ControlledPawn)
    {
        ASPTEnemyCharacter* EnemyCharacterRef = Cast<ASPTEnemyCharacter>(ControlledPawn);
        if (EnemyCharacterRef)
        {
            // 무기 장착 여부 체크
            bIsEquippedWeapon = EnemyCharacterRef->IsEquippedWeapon;

            UE_LOG(LogTemp, Warning, TEXT("[UBTD_IsEquippedWeapon] Is Equipping Weapon: %s"), bIsEquippedWeapon ? TEXT("True") : TEXT("False"));
        }
    }

    return bIsEquippedWeapon;
}
