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

