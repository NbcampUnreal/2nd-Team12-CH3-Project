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

    if (HitCount >= TargetObjects.Num()) // 모든 오브젝트가 맞았으면
    {
        // 보스 소환
        if (BossClass)
        {
            GetWorld()->SpawnActor<AActor>(BossClass, SpawnLocation, FRotator::ZeroRotator);
        }

        // 특정 오브젝트 회전 (한 번만 실행)
        if (RotatingObject)
        {
            FRotator NewRotation = RotatingObject->GetActorRotation();
            NewRotation.Yaw += RotationAngle; // 지정된 각도만큼 회전
            RotatingObject->SetActorRotation(NewRotation);

            // 몇 초 뒤에 원래 회전 값으로 되돌리기 위해 타이머 설정
            GetWorld()->GetTimerManager().SetTimer(RotationResetTimerHandle, this, &ABossSpawnerTrigger::ResetRotation, RotationResetDelay, false);
        }
    }
}

void ABossSpawnerTrigger::ResetRotation()
{
    // 회전 값을 원래 상태로 되돌리기
    if (RotatingObject)
    {
        FRotator NewRotation = RotatingObject->GetActorRotation();
        NewRotation.Yaw -= RotationAngle; // 지정된 각도만큼 회전
        RotatingObject->SetActorRotation(NewRotation);
    }
}