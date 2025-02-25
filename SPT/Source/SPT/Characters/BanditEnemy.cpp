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
            OnAttackEnd.Broadcast();  // �ִϸ��̼� ���� �� ȣ��
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
        }
    }
}
