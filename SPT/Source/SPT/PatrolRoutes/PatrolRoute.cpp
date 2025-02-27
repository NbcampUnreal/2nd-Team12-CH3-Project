#include "PatrolRoute.h"
#include "Components/SplineComponent.h"

APatrolRoute::APatrolRoute()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultScenRoot"));
    Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
    Path->SetupAttachment(RootComponent);
}

FVector APatrolRoute::GetNextPatrolPoint(AActor* Requester)
{
    if (!Path || Path->GetNumberOfSplinePoints() == 0)
    {
        return FVector::ZeroVector;
    }

    // ���� AI���� ���� �ε����� �����ؾ� �Ѵٸ� ���� Ȱ��
    int32& CurrentIndex = PatrolIndexes.FindOrAdd(Requester, 0);
    FVector PatrolPoint = Path->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);

    // �ε��� ��ȯ
    CurrentIndex = (CurrentIndex + 1) % Path->GetNumberOfSplinePoints();

    return PatrolPoint;
}

// C++ ���� �ʱ�ȭ�� �����ڿ���
// ��� ������ ������ �Լ�, �ؿ��� ����

//// �̵� ��� ����
//MoveToLocation(
//    NextPoint,           // ��ǥ ��ġ
//    5.0f,                // ��ǥ ������ ��������ٰ� �Ǵ��� �Ÿ� (5cm)
//    true,                // ��ǥ ������ ������ �� ����
//    true,                // �׺���̼� �޽��� ����Ͽ� ��� Ž��
//    true,                // ��ǥ ��ġ�� �׺���̼� �޽��� ���� ���, ���� ����� �׺���̼� �������� ����
//    true,                // AI�� �̵� �� ȸ�� ����
//    nullptr,             // �׺���̼� ��� ���� ����
//    true                 // �κ� ��� ��� ���� (����� �Ϻθ� ã�� �� �־ �̵�)
//);

