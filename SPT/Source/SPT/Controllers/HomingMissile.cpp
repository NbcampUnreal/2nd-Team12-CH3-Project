#include "HomingMissile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h" 
#include "CollisionQueryParams.h" 



AHomingMissile::AHomingMissile()
{
    PrimaryActorTick.bCanEverTick = true;

    ExplosionRadius = 1000.0f;
    ExplosionDamage = 30.0f;

    // 충돌 감지용 컴포넌트
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(15.0f);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AHomingMissile::OnHit);

    // 미사일 메시
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    // 유도탄 이동 컴포넌트
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = MissileSpeed;
    ProjectileMovement->MaxSpeed = MissileSpeed;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->bRotationFollowsVelocity = true;

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(RootComponent);

    Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AHomingMissile::BeginPlay()
{
    Super::BeginPlay();

    // 플레이어를 타겟으로 설정
    Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Target)
    {
        // 플레이어의 위치를 계산하여 미사일을 그쪽으로 회전시킴
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();

        // 미사일이 수평으로 회전하도록 설정
        SetActorRotation(FRotator(0, NewRotation.Yaw, 0)); // Z축 회전만 적용하여 수평 회전


        ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingAccelerationMagnitude = 3000.0f;
    }
}

void AHomingMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 유도탄이 목표를 추적하도록 계속 업데이트
    if (Target)
    {
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();
        SetActorRotation(NewRotation);
    }
}



void AHomingMissile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{ 
    FString Message1 = "Exploded";

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    // 미사일이 충돌한 순간 폭발 처리
    Explode();

}

void AHomingMissile::Explode()
{
    TArray<AActor*> OverlappingActors;

    // ExplosionCollision 컴포넌트가 유효한지 확인
    if (ExplosionCollision)
    {
        ExplosionCollision->GetOverlappingActors(OverlappingActors);

        // 배열에 값이 들어왔는지 확인
        if (OverlappingActors.Num() > 0)
        {
            for (AActor* Actor : OverlappingActors)
            {
                // 액터가 유효하고 "Player" 태그가 있는지 확인
                if (Actor && Actor->ActorHasTag("Player"))
                {
                    // 데미지를 발생시켜 Actor->TakeDamage()가 실행되도록 함
                    UGameplayStatics::ApplyDamage(
                        Actor,                      // 데미지를 받을 액터
                        ExplosionDamage,            // 데미지 양
                        nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
                        this,                       // 데미지를 유발한 오브젝트(지뢰)
                        UDamageType::StaticClass()  // 기본 데미지 유형
                    );

                    UE_LOG(LogTemp, Warning, TEXT("Damaged!"));
                }
            }
        }
        else
        {
            // 배열이 비어 있는 경우 디버그 로그
            UE_LOG(LogTemp, Warning, TEXT("No actors overlapping in explosion area"));
        }
    }
    else
    {
        // ExplosionCollision이 유효하지 않으면 로그 찍기
        UE_LOG(LogTemp, Warning, TEXT("ExplosionCollision component is invalid"));
    }

    // 미사일 파괴
    Destroy();
}
