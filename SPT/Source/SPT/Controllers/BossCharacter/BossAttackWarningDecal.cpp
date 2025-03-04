#include "BossCharacter/BossAttackWarningDecal.h"
#include "Components/DecalComponent.h"

// 생성자
ABossAttackWarningDecal::ABossAttackWarningDecal()
{
    PrimaryActorTick.bCanEverTick = true; // Tick 활성화
    // 새로운 루트 컴포넌트 생성
    USceneComponent* NewRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = NewRootComponent;  // 새로운 루트 컴포넌트를 루트로 설정

    // 첫 번째 데칼 컴포넌트 생성
    DecalComponent1 = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent1"));
    DecalComponent1->SetupAttachment(RootComponent);  // 루트 컴포넌트에 연결
    DecalComponent1->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
    DecalComponent1->DecalSize = FVector(10.f, 100.f, 100.0f);

    // 두 번째 데칼 컴포넌트 생성
    DecalComponent2 = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent2"));
    DecalComponent2->SetupAttachment(RootComponent);
    DecalComponent2->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
    DecalComponent2->DecalSize = FVector(10.f, 100.f, 100.0f);

    CurrentTime = 0.0f;
    MaxDuration = 1.0f; // 기본값
    DecalRadius = 0.0f;
    DecalComponent2->SortOrder = 1;  // 높은 값일수록 우선순위가 높음
}

// 데칼 초기화 함수
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

    

// 매 프레임마다 호출되는 함수 (Opacity 조절)
void ABossAttackWarningDecal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 시간에 따라 DecalSize를 변화시키기
    float NewSize = FMath::Clamp(CurrentTime / MaxDuration, 0.0f, 1.0f);  // 0에서 1로 변하는 값

    // DecalComponent2 크기를 물리적으로 변화시키기
    FVector NewScale = FVector(1.0f, NewSize, NewSize);  // X, Y, Z에 스케일 적용
    DecalComponent2->SetRelativeScale3D(NewScale);  // 데칼 컴포넌트의 상대적인 스케일을 변경
    CurrentTime += DeltaTime;
}


// 데칼 삭제 함수
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