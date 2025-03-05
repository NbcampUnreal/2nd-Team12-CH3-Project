// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetBoard.h"
#include "Components/StaticMeshComponent.h"
#include "RecordBoard.h"

ATargetBoard::ATargetBoard()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(false);  // 물리 시뮬레이션 비활성화 (OnComponentHit() 방식 사용)
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // 충돌 이벤트 바인딩
    MeshComp->OnComponentHit.AddDynamic(this, &ATargetBoard::OnHit);

}

void ATargetBoard::BeginPlay()
{
    Super::BeginPlay();
}

void ATargetBoard::OnHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (RecordBoard)
    {
        // TargetBoard의 크기 기준으로 충돌 지점의 비율 계산
        FVector LocalHitPos = Hit.ImpactPoint - GetActorLocation();
        FVector Min, Max;
        MeshComp->GetLocalBounds(Min, Max);

        // 컴포넌트의 스케일을 적용하여 실제 크기 계산
        FVector Scale = MeshComp->GetComponentScale();
        float ActualWidth = (Max.X - Min.X) * Scale.X;
        float ActualHeight = (Max.Y - Min.Y) * Scale.Y;
        float ActualDepth = (Max.Z - Min.Z) * Scale.Z;
        // 충돌 지점의 비율 계산 (X, Y, Z 각각 -0.5 ~ 0.5 사이로 변환)
        FVector HitRatio = FVector(
            LocalHitPos.X / ActualWidth,
            LocalHitPos.Y / ActualHeight,
            LocalHitPos.Z / ActualDepth
        );
        // 비율을 RecordBoard에 전달
        RecordBoard->RegisterHit(HitRatio);
    }
}