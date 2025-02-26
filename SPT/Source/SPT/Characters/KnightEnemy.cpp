#include "KnightEnemy.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"

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
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("[KnightEnemy] Playing Attack Montage for: %s"), *GetName());
            //AnimInstance->Montage_Play(AttackMontage);

            //// ��������Ʈ ���� ��������� ����
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
            //            OnAttackEnd.Broadcast();  // �ִϸ��̼� ���� �� ȣ��
            //        }
            //    });

            //// ��������Ʈ ���
            //AnimInstance->Montage_SetEndDelegate(MontageEndDelegate);
            OnAttackEnd.Broadcast();  // �ִϸ��̼� ���� �� ȣ��
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

            //if (SwordClass) // Sword�� UClass* ����
            //{
            //    FActorSpawnParameters SpawnParams;
            //    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Default;            

            //    // ���� ���� ��ġ���� ����
            //    FTransform SpawnTransform = GetActorTransform();
            //    ABP_Sword* SpawnedSword = GetWorld()->SpawnActor<ABP_Sword>(SwordClass, SpawnTransform, SpawnParams);

            //    if (SpawnedSword)
            //    {
            //        // ������ ���⸦ �տ� ����
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
