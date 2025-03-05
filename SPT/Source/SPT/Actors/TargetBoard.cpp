// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetBoard.h"
#include "Components/StaticMeshComponent.h"
#include "RecordBoard.h"

ATargetBoard::ATargetBoard()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(false);  // ���� �ùķ��̼� ��Ȱ��ȭ (OnComponentHit() ��� ���)
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    // �浹 �̺�Ʈ ���ε�
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
        // TargetBoard�� ũ�� �������� �浹 ������ ���� ���
        FVector LocalHitPos = Hit.ImpactPoint - GetActorLocation();
        FVector Min, Max;
        MeshComp->GetLocalBounds(Min, Max);

        // ������Ʈ�� �������� �����Ͽ� ���� ũ�� ���
        FVector Scale = MeshComp->GetComponentScale();
        float ActualWidth = (Max.X - Min.X) * Scale.X;
        float ActualHeight = (Max.Y - Min.Y) * Scale.Y;
        float ActualDepth = (Max.Z - Min.Z) * Scale.Z;
        // �浹 ������ ���� ��� (X, Y, Z ���� -0.5 ~ 0.5 ���̷� ��ȯ)
        FVector HitRatio = FVector(
            LocalHitPos.X / ActualWidth,
            LocalHitPos.Y / ActualHeight,
            LocalHitPos.Z / ActualDepth
        );
        // ������ RecordBoard�� ����
        RecordBoard->RegisterHit(HitRatio);
    }
}