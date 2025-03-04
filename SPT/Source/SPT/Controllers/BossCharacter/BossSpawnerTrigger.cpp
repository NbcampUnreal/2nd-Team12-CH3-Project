// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/BossSpawnerTrigger.h"
#include "TriggerObject.h"

ABossSpawnerTrigger::ABossSpawnerTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABossSpawnerTrigger::BeginPlay()
{
    Super::BeginPlay();

    for (AActor* Obj : TargetObjects)
    {
        ATriggerObject* TriggerObj = Cast<ATriggerObject>(Obj);
        if (TriggerObj)
        {
            TriggerObj->OnHitTrigger.AddDynamic(this, &ABossSpawnerTrigger::OnObjectHit);
        }
    }
}

void ABossSpawnerTrigger::OnObjectHit()
{
    HitCount++;

    if (HitCount >= TargetObjects.Num()) // ��� ������Ʈ�� �¾�����
    {
        // ���� ��ȯ
        if (BossClass)
        {
            GetWorld()->SpawnActor<AActor>(BossClass, SpawnLocation, FRotator::ZeroRotator);
        }

        // Ư�� ������Ʈ ȸ�� (�� ���� ����)
        if (RotatingObject)
        {
            FRotator NewRotation = RotatingObject->GetActorRotation();
            NewRotation.Yaw += RotationAngle; // ������ ������ŭ ȸ��
            RotatingObject->SetActorRotation(NewRotation);

            // �� �� �ڿ� ���� ȸ�� ������ �ǵ����� ���� Ÿ�̸� ����
            GetWorld()->GetTimerManager().SetTimer(RotationResetTimerHandle, this, &ABossSpawnerTrigger::ResetRotation, RotationResetDelay, false);
        }
    }
}

void ABossSpawnerTrigger::ResetRotation()
{
    // ȸ�� ���� ���� ���·� �ǵ�����
    if (RotatingObject)
    {
        FRotator NewRotation = RotatingObject->GetActorRotation();
        NewRotation.Yaw -= RotationAngle; // ������ ������ŭ ȸ��
        RotatingObject->SetActorRotation(NewRotation);
    }
}