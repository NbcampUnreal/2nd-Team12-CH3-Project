#include "BossCharacter/BossAttackWarningDecal.h"
#include "Components/DecalComponent.h"

// ������
ABossAttackWarningDecal::ABossAttackWarningDecal()
{
    PrimaryActorTick.bCanEverTick = true; // Tick Ȱ��ȭ
    // ���ο� ��Ʈ ������Ʈ ����
    USceneComponent* NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = NewRootComponent;  // ���ο� ��Ʈ ������Ʈ�� ��Ʈ�� ����

    // ù ��° ��Į ������Ʈ ����
    DecalComponent1 = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent1"));
    DecalComponent1->SetupAttachment(RootComponent);  // ��Ʈ ������Ʈ�� ����
    DecalComponent1->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
    DecalComponent1->DecalSize = FVector(10.f, 100.f, 100.0f);

    // �� ��° ��Į ������Ʈ ����
    DecalComponent2 = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent2"));
    DecalComponent2->SetupAttachment(RootComponent);
    DecalComponent2->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
    DecalComponent2->DecalSize = FVector(10.f, 100.f, 100.0f);

    CurrentTime = 0.0f;
    MaxDuration = 1.0f; // �⺻��
    DecalRadius = 0.0f;
    DecalComponent2->SortOrder = 1;  // ���� ���ϼ��� �켱������ ����
}

// ��Į �ʱ�ȭ �Լ�
void ABossAttackWarningDecal::InitializeDecal(UMaterialInterface* InDecalMaterial1, UMaterialInterface* InDecalMaterial2, FVector Location, float W,float H, float Duration)
{
    SetActorLocation(Location);

    if (InDecalMaterial1)
    {
        DecalComponent1->SetDecalMaterial(InDecalMaterial1);
        DecalComponent1->DecalSize = FVector(40.0f, W, H);
    }

    if (InDecalMaterial2)
    {
        DecalComponent2->SetDecalMaterial(InDecalMaterial2);
        DecalComponent2->DecalSize = FVector(40.0f, W, H);
    }
    CurrentTime = 0.0f;
    MaxDuration = Duration;
    GetWorld()->GetTimerManager().SetTimer(DecalTimerHandle, this, &ABossAttackWarningDecal::DestroyDecal, Duration, false);
}

    

// �� �����Ӹ��� ȣ��Ǵ� �Լ� (Opacity ����)
void ABossAttackWarningDecal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // �ð��� ���� DecalSize�� ��ȭ��Ű��
    float NewSize = FMath::Clamp(CurrentTime / MaxDuration, 0.0f, 1.0f);  // 0���� 1�� ���ϴ� ��

    // DecalComponent2 ũ�⸦ ���������� ��ȭ��Ű��
    FVector NewScale = FVector(1.0f, NewSize, NewSize);  // X, Y, Z�� ������ ����
    DecalComponent2->SetRelativeScale3D(NewScale);  // ��Į ������Ʈ�� ������� �������� ����
    CurrentTime += DeltaTime;
}


// ��Į ���� �Լ�
void ABossAttackWarningDecal::DestroyDecal()
{
    if (DecalComponent1)
    {
        DecalComponent1->DestroyComponent();
    }
    if (DecalComponent2)
    {
        DecalComponent2->DestroyComponent();
    }
    Destroy();
}