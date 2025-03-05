// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetBoard.h"
#include "Components/StaticMeshComponent.h"
#include "RecordBoard.h"
#include "SPT/Items/Weapons/FirearmWeapon.h"  // 총기 클래스 포함

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

//TakeDamage 오버라이드 (총기에서 FHitResult 가져오기)
float ATargetBoard::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (RecordBoard)
    {
        // DamageCauser가 총기라면 캐스팅하여 FHitResult 가져오기
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

            // 비율을 RecordBoard에 전달
            RecordBoard->RegisterHit(HitRatio);
        }
    }

    return DamageAmount;
}