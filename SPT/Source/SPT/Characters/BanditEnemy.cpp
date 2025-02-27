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
    // AI ��Ʈ�ѷ��� ���� ������ ��������
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(GetController());
    if (AIC && AIC->BBC)
    {
        // �������� EnemyType ���� Bandit���� ����
       AIC->BBC->SetValueAsEnum(FName("EnemyType"), 2); // 2 = Bandit (Enum ��)
        UE_LOG(LogTemp, Warning, TEXT("%s EnemyType Set in Blackboard!"), *GetName());
    }
}

void ABanditEnemy::Attack()
{
    Super::Attack();  // �θ� �⺻ ���� ���� ȣ��
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[BanditEnemy] Playing Attack Montage for: %s"), *GetName());
            //AnimInstance->Montage_Play(AttackMontage);

            //// ��������Ʈ ���� ��������� ����
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
            //            OnAttackEnd.Broadcast();  // �ִϸ��̼� ���� �� ȣ��
            //        }
            //    });

            //// ��������Ʈ ���
            //AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
            OnAttackEnd.Broadcast();  // �ִϸ��̼� ���� �� ȣ��
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
            //if (RifleClass) // Rifle�� UClass* ����
            //{
            //    FActorSpawnParameters SpawnParams;
            //    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Default;

            //    // ���� ���� ��ġ���� ����
            //    FTransform SpawnTransform = GetActorTransform();
            //    ABP_Rifle* SpawnedRifle = GetWorld()->SpawnActor<ABP_Rifle>(RifleClass, SpawnTransform, SpawnParams);

            //    if (SpawnedRifle)
            //    {
            //        // ������ ���⸦ �տ� ����
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
