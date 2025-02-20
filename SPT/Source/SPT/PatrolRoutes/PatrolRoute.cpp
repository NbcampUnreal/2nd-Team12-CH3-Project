#include "PatrolRoute.h"

APatrolRoute::APatrolRoute()
{
    PatrolSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));
    RootComponent = PatrolSpline;

    PatrolIndex = 0;
    Direction = 1;
}

void APatrolRoute::IncrementPatrolRoute()
{
    if (!PatrolSpline) return;

    int32 NumPoints = PatrolSpline->GetNumberOfSplinePoints();

    if (NumPoints <= 1) return;  // 유효한 스플라인 포인트가 없으면 반환 (1개는 시작점과 끝점이 동일)

    PatrolIndex += Direction;

    // 끝에 도달하면 방향 반전
    if (PatrolIndex >= NumPoints - 1)
    {
        PatrolIndex = NumPoints - 1;
        Direction = -1;
    }
    else if (PatrolIndex <= 0)
    {
        PatrolIndex = 0;
        Direction = 1;
    }
}

FVector APatrolRoute::GetNextPatrolPoint() const
{
    if (!PatrolSpline) return FVector::ZeroVector;

    return PatrolSpline->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}

// C++ 변수 초기화는 생성자에서
// 기능 단위로 위에는 함수, 밑에는 변수