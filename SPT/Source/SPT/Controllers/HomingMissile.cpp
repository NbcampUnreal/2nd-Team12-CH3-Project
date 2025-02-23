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

    // �浹 ������ ������Ʈ
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(15.0f);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AHomingMissile::OnHit);

    // �̻��� �޽�
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    // ����ź �̵� ������Ʈ
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

    // �÷��̾ Ÿ������ ����
    Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Target)
    {
        // �÷��̾��� ��ġ�� ����Ͽ� �̻����� �������� ȸ����Ŵ
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();

        // �̻����� �������� ȸ���ϵ��� ����
        SetActorRotation(FRotator(0, NewRotation.Yaw, 0)); // Z�� ȸ���� �����Ͽ� ���� ȸ��


        ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingAccelerationMagnitude = 3000.0f;
    }
}

void AHomingMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ����ź�� ��ǥ�� �����ϵ��� ��� ������Ʈ
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
    // �̻����� �浹�� ���� ���� ó��
    Explode();

}

void AHomingMissile::Explode()
{
    TArray<AActor*> OverlappingActors;

    // ExplosionCollision ������Ʈ�� ��ȿ���� Ȯ��
    if (ExplosionCollision)
    {
        ExplosionCollision->GetOverlappingActors(OverlappingActors);

        // �迭�� ���� ���Դ��� Ȯ��
        if (OverlappingActors.Num() > 0)
        {
            for (AActor* Actor : OverlappingActors)
            {
                // ���Ͱ� ��ȿ�ϰ� "Player" �±װ� �ִ��� Ȯ��
                if (Actor && Actor->ActorHasTag("Player"))
                {
                    // �������� �߻����� Actor->TakeDamage()�� ����ǵ��� ��
                    UGameplayStatics::ApplyDamage(
                        Actor,                      // �������� ���� ����
                        ExplosionDamage,            // ������ ��
                        nullptr,                    // �������� ������ ��ü (���ڸ� ��ġ�� ĳ���Ͱ� �����Ƿ� nullptr)
                        this,                       // �������� ������ ������Ʈ(����)
                        UDamageType::StaticClass()  // �⺻ ������ ����
                    );

                    UE_LOG(LogTemp, Warning, TEXT("Damaged!"));
                }
            }
        }
        else
        {
            // �迭�� ��� �ִ� ��� ����� �α�
            UE_LOG(LogTemp, Warning, TEXT("No actors overlapping in explosion area"));
        }
    }
    else
    {
        // ExplosionCollision�� ��ȿ���� ������ �α� ���
        UE_LOG(LogTemp, Warning, TEXT("ExplosionCollision component is invalid"));
    }

    // �̻��� �ı�
    Destroy();
}
