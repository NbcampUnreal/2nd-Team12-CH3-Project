// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/TriggerObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"  // Character 클래스를 포함
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

ATriggerObject::ATriggerObject()
{
    PrimaryActorTick.bCanEverTick = false;


    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(10.0f);

    // 충돌 이벤트 활성화
    CollisionComponent->SetNotifyRigidBodyCollision(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ATriggerObject::OnHit);

    // 메시 컴포넌트 생성 및 루트 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
}

void ATriggerObject::BeginPlay()
{
    Super::BeginPlay();
}

void ATriggerObject::TriggerHit()
{
    OnHitTrigger.Broadcast();  // BossSpawnerTrigger에게 신호 전달
}

void ATriggerObject::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    FString Message = "Hitted!";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
    // 데미지 적용
    //UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);

    TriggerHit();
    //파괴
    Destroy();
}