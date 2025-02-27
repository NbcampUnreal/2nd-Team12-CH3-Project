#include "BanditEnemy.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"

ABanditEnemy::ABanditEnemy()
{

}

void ABanditEnemy::BeginPlay()
{
    Super::BeginPlay();
    // AI 컨트롤러를 통해 블랙보드 가져오기
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(GetController());
    if (AIC && AIC->BBC)
    {
        // 블랙보드의 EnemyType 값을 Bandit으로 설정
       AIC->BBC->SetValueAsEnum(FName("EnemyType"), 2); // 2 = Bandit (Enum 값)
        UE_LOG(LogTemp, Warning, TEXT("%s EnemyType Set in Blackboard!"), *GetName());
    }
}

void ABanditEnemy::Attack()
{
    Super::Attack();  // 부모 기본 공격 로직 호출
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] Playing Attack Montage for: %s"), *GetName());
            //AnimInstance->Montage_Play(AttackMontage);

            //// 델리게이트 변수 명시적으로 생성
            //FOnMontageEnded MontageEndDelegate;
            //MontageEndDelegate.BindLambda([this](UAnimMontage*, bool bInterrupted)
            //    {
            //        if (bInterrupted)
            //        {
            //            UE_LOG(LogTemp, Error, TEXT("[BanditEnemy] Attack Montage Interrupted for: %s"), *GetName());
            //        }
            //        else
            //        {
            //            UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] Attack Montage Finished for: %s"), *GetName());
            //            OnAttackEnd.Broadcast();  // 애니메이션 종료 시 호출
            //        }
            //    });

            //// 델리게이트 등록
            //AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
            OnAttackEnd.Broadcast();  // 애니메이션 종료 시 호출
        }
    }
}

void ABanditEnemy::EquippedWeapon()
{
    Super::EquippedWeapon();
    if (EquipMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] Equip Weapon Montage"));
            bool IsEquippingWeapon = true;
            //if (RifleClass) // Rifle의 UClass* 참조
            //{
            //    FActorSpawnParameters SpawnParams;
            //    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Default;

            //    // 현재 액터 위치에서 스폰
            //    FTransform SpawnTransform = GetActorTransform();
            //    ABP_Rifle* SpawnedRifle = GetWorld()->SpawnActor<ABP_Rifle>(RifleClass, SpawnTransform, SpawnParams);

            //    if (SpawnedRifle)
            //    {
            //        // 스폰된 무기를 손에 부착
            //        FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
            //        SpawnedRifle->AttachToComponent(GetMesh(), AttachRules, FName("hand_r_rifle_socket"));

            //        UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] Rifle Attached Successfully"));
            //    }
            //}
            return;
        }
    }
}

void ABanditEnemy::UnEquippedWeapon()
{
    Super::UnEquippedWeapon();
    if (UnEquipMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] UnEquip Weapon Montage"));
            //if (EquippedRifle)
            //{
            //    EquippedRifle->Destroy();
            //    EquippedRifle = nullptr;
            //}
            return;
        }
    }
}
