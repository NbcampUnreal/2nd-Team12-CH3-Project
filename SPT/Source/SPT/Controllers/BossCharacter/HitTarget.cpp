// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/HitTarget.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AHitTarget::AHitTarget()
{
    PrimaryActorTick.bCanEverTick = false;

    // ǥ����(TargetMeshComp)�� RootComponent�� ����
    TargetMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
    RootComponent = TargetMeshComp;  // RootComponent�� ����

    // ���� �ùķ��̼� �� ��Ʈ �̺�Ʈ ����
    TargetMeshComp->SetSimulatePhysics(true);
    TargetMeshComp->OnComponentHit.AddDynamic(this, &AHitTarget::OnHit);

    // �����(BoardMeshComp) ���� �� RootComponent�� ÷��
    BoardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Board"));
    BoardMeshComp->SetupAttachment(RootComponent);  // RootComponent�� ÷��
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
    FVector HitLocation = Hit.ImpactPoint;  // ���� ��ġ
    RecordHit(HitLocation);
    DrawHitMarker(HitLocation);
    MoveToOtherTarget(HitLocation);  // ǥ������ �ٸ� ������ �ű�
}

void AHitTarget::RecordHit(FVector HitLocation)
{
    UE_LOG(LogTemp, Log, TEXT("Hit recorded at: %s"), *HitLocation.ToString());
}

void AHitTarget::DrawHitMarker(FVector HitLocation)
{
    // ���� ��ġ�� ��ü�� �׷��� ǥ��
    DrawDebugSphere(GetWorld(), HitLocation, 10.0f, 12, FColor::Green, false, 5.0f);
}

void AHitTarget::MoveToOtherTarget(FVector HitLocation)
{
    if (BoardMeshComp && TargetMeshComp)
    {
        // ǥ����(TargetMeshComp)�� ��� ��ġ (�߽� ��ǥ)
        FVector TargetRelativeLocation = TargetMeshComp->GetRelativeLocation();

        // ǥ������ ũ��(�ظ�� ����) ��������
        FBoxSphereBounds TargetBounds = TargetMeshComp->GetStaticMesh()->GetBounds();
        FVector TargetCubeExtent = TargetBounds.BoxExtent;
        FVector TargetScale = TargetMeshComp->GetComponentScale();

        // ǥ���� �ٴ��� �������� Z ���� ���
        float TargetBottomZ = TargetRelativeLocation.Z - TargetCubeExtent.Z * TargetScale.Z;
        float RelativeHitZ = HitLocation.Z - TargetBottomZ;
        float ZRatio = RelativeHitZ / (2 * TargetCubeExtent.Z * TargetScale.Z);

        // ǥ������ �߽����κ��� ���� ��ġ�� ����� �Ÿ� ��� (����, ����)
        FVector RelativeHitLocation = HitLocation - TargetRelativeLocation;

        // ����� �Ÿ��� ���� ��� (X, Y ����) - ������ ���
        float XRatio = RelativeHitLocation.X / (TargetCubeExtent.X * TargetScale.X);
        float YRatio = RelativeHitLocation.Y / (TargetCubeExtent.Y * TargetScale.Y);

        UE_LOG(LogTemp, Log, TEXT("X Ratio: %f, Y Ratio: %f, Z Ratio: %f"), XRatio, YRatio, ZRatio);

        // �����(BoardMeshComp)�� ��� ��ġ
        FVector BoardWorldLocation = BoardMeshComp->GetComponentLocation();

        // ������� ũ�� ��������
        FBoxSphereBounds BoardBounds = BoardMeshComp->GetStaticMesh()->GetBounds();
        FVector BoardCubeExtent = BoardBounds.BoxExtent;
        FVector BoardScale = BoardMeshComp->GetComponentScale();

        // ������� �ٴ��� �������� ��ȯ
        float BoardBottomZ = BoardWorldLocation.Z - BoardCubeExtent.Z * BoardScale.Z;
        float MappedZ = BoardBottomZ + ZRatio * (2 * BoardCubeExtent.Z * BoardScale.Z);

        // ������� ũ�⸦ �������� ��ġ ��ȯ
        FVector MappedLocation = BoardWorldLocation + FVector(XRatio * BoardCubeExtent.X * BoardScale.X,
            YRatio * BoardCubeExtent.Y * BoardScale.Y,
            MappedZ - BoardWorldLocation.Z);

        DrawDebugSphere(GetWorld(), MappedLocation, 10.0f, 12, FColor::Blue, false, 5.0f);
        UE_LOG(LogTemp, Log, TEXT("Final Location: %s"), *MappedLocation.ToString());
    }
}
