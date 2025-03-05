// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetBoard.h"
#include "Components/StaticMeshComponent.h"
#include "RecordBoard.h"
#include "SPT/Items/Weapons/FirearmWeapon.h"  // �ѱ� Ŭ���� ����

ATargetBoard::ATargetBoard()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATargetBoard::BeginPlay()
{
    Super::BeginPlay();
}

//TakeDamage �������̵� (�ѱ⿡�� FHitResult ��������)
float ATargetBoard::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (RecordBoard)
    {
        // DamageCauser�� �ѱ��� ĳ�����Ͽ� FHitResult ��������
        AFirearmWeapon* Gun = Cast<AFirearmWeapon>(DamageCauser);
        if (Gun)
        {
            FHitResult HitResult = Gun->HitResult;

            FVector LocalHitPos = HitResult.ImpactPoint - GetActorLocation();
            FVector Min, Max;
            MeshComp->GetLocalBounds(Min, Max);

            FVector Scale = MeshComp->GetComponentScale();
            float ActualWidth = (Max.X - Min.X) * Scale.X;
            float ActualHeight = (Max.Y - Min.Y) * Scale.Y;
            float ActualDepth = (Max.Z - Min.Z) * Scale.Z;

            FVector HitRatio = FVector(
                LocalHitPos.X / ActualWidth,
                LocalHitPos.Y / ActualHeight,
                LocalHitPos.Z / ActualDepth
            );

            // ������ RecordBoard�� ����
            RecordBoard->RegisterHit(HitRatio);
        }
    }

    return DamageAmount;
}