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
            MarkerList.Add(NewMarker);  // 기록 리스트에 추가
        }
    }
}

FVector ARecordBoard::ConvertToRecordBoardSpace(FVector TargetHitRatio)
{
    FVector Min, Max;
    MeshComp->GetLocalBounds(Min, Max);

    // 컴포넌트의 스케일을 적용하여 실제 크기 계산
    FVector Scale = MeshComp->GetComponentScale();
    float ActualWidth = (Max.X - Min.X) * Scale.X;
    float ActualHeight = (Max.Y - Min.Y) * Scale.Y;
    float ActualDepth = (Max.Z - Min.Z) * Scale.Z;

    FVector temp = FVector(ActualWidth, ActualHeight, ActualDepth);

    FVector HitPos = FVector(
        TargetHitRatio.X * ActualWidth,  // X축 크기에 비례
        TargetHitRatio.Y * ActualHeight,  // Y축 크기에 비례
        TargetHitRatio.Z * ActualDepth   // Z축 크기에 비례
    );
    
    FVector RecordBoardPos = GetActorLocation() + HitPos;

    // 변환된 좌표를 다시 월드 공간으로 변환하여 반환
    return RecordBoardPos;
}
