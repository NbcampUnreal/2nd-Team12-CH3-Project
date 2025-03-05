#include "RecordBoard.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ARecordBoard::ARecordBoard()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
    RootComponent = MeshComp;
}

void ARecordBoard::BeginPlay()
{
    Super::BeginPlay();
}

void ARecordBoard::RegisterHit(FVector TargetHitLocation)
{
    FVector LocalPos = ConvertToRecordBoardSpace(TargetHitLocation);
    if (MarkerBlueprint)
    {
        AActor* NewMarker = GetWorld()->SpawnActor<AActor>(MarkerBlueprint, LocalPos, FRotator::ZeroRotator);

        if (NewMarker)
        {
            MarkerList.Add(NewMarker);  // ��� ����Ʈ�� �߰�
        }
    }
}

FVector ARecordBoard::ConvertToRecordBoardSpace(FVector TargetHitRatio)
{
    FVector Min, Max;
    MeshComp->GetLocalBounds(Min, Max);

    // ������Ʈ�� �������� �����Ͽ� ���� ũ�� ���
    FVector Scale = MeshComp->GetComponentScale();
    float ActualWidth = (Max.X - Min.X) * Scale.X;
    float ActualHeight = (Max.Y - Min.Y) * Scale.Y;
    float ActualDepth = (Max.Z - Min.Z) * Scale.Z;

    FVector temp = FVector(ActualWidth, ActualHeight, ActualDepth);

    FVector HitPos = FVector(
        TargetHitRatio.X * ActualWidth,  // X�� ũ�⿡ ���
        TargetHitRatio.Y * ActualHeight,  // Y�� ũ�⿡ ���
        TargetHitRatio.Z * ActualDepth   // Z�� ũ�⿡ ���
    );
    
    FVector RecordBoardPos = GetActorLocation() + HitPos;

    // ��ȯ�� ��ǥ�� �ٽ� ���� �������� ��ȯ�Ͽ� ��ȯ
    return RecordBoardPos;
}
