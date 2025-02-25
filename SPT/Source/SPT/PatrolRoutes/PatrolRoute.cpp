#include "PatrolRoute.h"
#include "Components/SplineComponent.h"

APatrolRoute::APatrolRoute()
{
    Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
    Path->SetupAttachment(RootComponent);
}

void APatrolRoute::BeginPlay()
{
    Super::BeginPlay();
}

// C++ 변수 초기화는 생성자에서
// 기능 단위로 위에는 함수, 밑에는 변수

//// 이동 명령 실행
//MoveToLocation(
//    NextPoint,           // 목표 위치
//    5.0f,                // 목표 지점에 가까워졌다고 판단할 거리 (5cm)
//    true,                // 목표 지점과 겹쳤을 때 멈춤
//    true,                // 네비게이션 메쉬를 사용하여 경로 탐색
//    true,                // 목표 위치가 네비게이션 메쉬에 없을 경우, 가장 가까운 네비게이션 지점으로 투영
//    true,                // AI가 이동 중 회전 가능
//    nullptr,             // 네비게이션 경로 필터 없음
//    true                 // 부분 경로 허용 여부 (경로의 일부만 찾을 수 있어도 이동)
//);

