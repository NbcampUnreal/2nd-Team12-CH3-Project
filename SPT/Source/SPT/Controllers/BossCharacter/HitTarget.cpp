// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/HitTarget.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AHitTarget::AHitTarget()
{
    PrimaryActorTick.bCanEverTick = false;

    // 표적지(TargetMeshComp)를 RootComponent로 설정
    TargetMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
    RootComponent = TargetMeshComp;  // RootComponent로 설정

    // 물리 시뮬레이션 및 히트 이벤트 설정
    TargetMeshComp->SetSimulatePhysics(true);
    TargetMeshComp->OnComponentHit.AddDynamic(this, &AHitTarget::OnHit);

    // 기록지(BoardMeshComp) 생성 및 RootComponent에 첨부
    BoardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));
    BoardMeshComp->SetupAttachment(RootComponent);  // RootComponent에 첨부
}

void AHitTarget::BeginPlay()
{
    Super::BeginPlay();

    if (CubeMesh)
    {
        TargetMeshComp->SetStaticMesh(CubeMesh);
        BoardMeshComp->SetStaticMesh(CubeMesh);
    }
}

void AHitTarget::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    FVector HitLocation = Hit.ImpactPoint;  // 맞은 위치
    RecordHit(HitLocation);
    DrawHitMarker(HitLocation);
    MoveToOtherTarget(HitLocation);  // 표적지를 다른 곳으로 옮김
}

void AHitTarget::RecordHit(FVector HitLocation)
{
    UE_LOG(LogTemp, Log, TEXT("Hit recorded at: %s"), *HitLocation.ToString());
}

void AHitTarget::DrawHitMarker(FVector HitLocation)
{
    // 맞은 위치에 구체를 그려서 표시
    DrawDebugSphere(GetWorld(), HitLocation, 10.0f, 12, FColor::Green, false, 5.0f);
}

void AHitTarget::MoveToOtherTarget(FVector HitLocation)
{
    if (BoardMeshComp && TargetMeshComp)
    {
        // 표적지(TargetMeshComp)의 상대 위치 (중심 좌표)
        FVector TargetRelativeLocation = TargetMeshComp->GetRelativeLocation();

        // 표적지의 크기(밑면과 높이) 가져오기
        FBoxSphereBounds TargetBounds = TargetMeshComp->GetStaticMesh()->GetBounds();
        FVector TargetCubeExtent = TargetBounds.BoxExtent;
        FVector TargetScale = TargetMeshComp->GetComponentScale();

        // 표적지 바닥을 기준으로 Z 비율 계산
        float TargetBottomZ = TargetRelativeLocation.Z - TargetCubeExtent.Z * TargetScale.Z;
        float RelativeHitZ = HitLocation.Z - TargetBottomZ;
        float ZRatio = RelativeHitZ / (2 * TargetCubeExtent.Z * TargetScale.Z);

        // 표적지의 중심으로부터 맞은 위치의 상대적 거리 계산 (가로, 세로)
        FVector RelativeHitLocation = HitLocation - TargetRelativeLocation;

        // 상대적 거리의 비율 계산 (X, Y 비율) - 스케일 고려
        float XRatio = RelativeHitLocation.X / (TargetCubeExtent.X * TargetScale.X);
        float YRatio = RelativeHitLocation.Y / (TargetCubeExtent.Y * TargetScale.Y);

        UE_LOG(LogTemp, Log, TEXT("X Ratio: %f, Y Ratio: %f, Z Ratio: %f"), XRatio, YRatio, ZRatio);

        // 기록지(BoardMeshComp)의 상대 위치
        FVector BoardWorldLocation = BoardMeshComp->GetComponentLocation();

        // 기록지의 크기 가져오기
        FBoxSphereBounds BoardBounds = BoardMeshComp->GetStaticMesh()->GetBounds();
        FVector BoardCubeExtent = BoardBounds.BoxExtent;
        FVector BoardScale = BoardMeshComp->GetComponentScale();

        // 기록지의 바닥을 기준으로 변환
        float BoardBottomZ = BoardWorldLocation.Z - BoardCubeExtent.Z * BoardScale.Z;
        float MappedZ = BoardBottomZ + ZRatio * (2 * BoardCubeExtent.Z * BoardScale.Z);

        // 기록지의 크기를 기준으로 위치 변환
        FVector MappedLocation = BoardWorldLocation + FVector(XRatio * BoardCubeExtent.X * BoardScale.X,
            YRatio * BoardCubeExtent.Y * BoardScale.Y,
            MappedZ - BoardWorldLocation.Z);

        DrawDebugSphere(GetWorld(), MappedLocation, 10.0f, 12, FColor::Blue, false, 5.0f);
        UE_LOG(LogTemp, Log, TEXT("Final Location: %s"), *MappedLocation.ToString());
    }
}
