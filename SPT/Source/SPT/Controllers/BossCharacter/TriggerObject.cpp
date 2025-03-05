// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/TriggerObject.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"  // Character Ŭ������ ����
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

ATriggerObject::ATriggerObject()
{
    PrimaryActorTick.bCanEverTick = false;


    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(10.0f);

    // �浹 �̺�Ʈ Ȱ��ȭ
    CollisionComponent->SetNotifyRigidBodyCollision(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);


    // �޽� ������Ʈ ���� �� ��Ʈ ����
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    Health = 100;
    MaxHealth = 100;
}

void ATriggerObject::BeginPlay()
{
    Super::BeginPlay();
}

void ATriggerObject::TriggerHit()
{
    OnHitTrigger.Broadcast();  // BossSpawnerTrigger���� ��ȣ ����
}


float ATriggerObject::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    float NewHealth = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

    SetHealth(NewHealth);
    FString Message = "Hitted!";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
    if (Health <= 0.f)
    {
        TriggerHit();
        //�ı�
        Destroy();
    }

    return 0.0f;
}


void ATriggerObject::SetHealth(float NewHealth)
{
    Health = NewHealth;
}