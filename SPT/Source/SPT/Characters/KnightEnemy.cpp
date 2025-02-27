#include "KnightEnemy.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"

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
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[KnightEnemy] Playing Attack Montage for: %s"), *GetName());
            //AnimInstance->Montage_Play(AttackMontage);

            //// 델리게이트 변수 명시적으로 생성
            //FOnMontageEnded MontageEndDelegate;
            //MontageEndDelegate.BindLambda([this](UAnimMontage*, bool bInterrupted)
            //    {
            //        if (bInterrupted)
            //        {
            //            UE_LOG(LogTemp, Error, TEXT("[KnightEnemy] Attack Montage Interrupted for: %s"), *GetName());
            //        }
            //        else
            //        {
            //            UE_LOG(LogTemp, Warning, TEXT("[KnightEnemy] Attack Montage Finished for: %s"), *GetName());
            //            OnAttackEnd.Broadcast();  // 애니메이션 종료 시 호출
            //        }
            //    });

            //// 델리게이트 등록
            //AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
            OnAttackEnd.Broadcast();  // 애니메이션 종료 시 호출
        }
    }
}

void AKnightEnemy::EquippedWeapon()
{
    Super::EquippedWeapon();
    if (EquipMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[KnightEnemy] Equip Weapon Montage"));

            //if (SwordClass) // Sword의 UClass* 참조
            //{
            //    FActorSpawnParameters SpawnParams;
            //    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Default;            

            //    // 현재 액터 위치에서 스폰
            //    FTransform SpawnTransform = GetActorTransform();
            //    ABP_Sword* SpawnedSword = GetWorld()->SpawnActor<ABP_Sword>(SwordClass, SpawnTransform, SpawnParams);

            //    if (SpawnedSword)
            //    {
            //        // 스폰된 무기를 손에 부착
            //        FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
            //        SpawnedSword->AttachToComponent(GetMesh(), AttachRules, FName("hand_r_Sword_socket"));

            //        UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] Sword Attached Successfully"));
            //    }
            //}
        }
    }
}

void AKnightEnemy::UnEquippedWeapon()
{
    Super::UnEquippedWeapon();
    if (UnEquipMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[KnightEnemy] UnEquip Weapon Montage"));
            //if (EquippedSword)
            //{
            //    EquippedSword->Destroy();
            //    EquippedSword = nullptr;
            //}
        }
    }
}
