// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineAISpawnPoint.h"
#include "SPTEnemyCharacter.h"

ASplineAISpawnPoint::ASplineAISpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	RespawnDelay = 1.f;
}

void ASplineAISpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnAI();
}

void ASplineAISpawnPoint::SpawnAI()
{
	// ���� ������Ű�� Ŭ������ ��������� �ִ�.
	// ���߿� ������ ���̺�� �����Ѵٸ� �����ؾ��Ѵ�.
	if (!SpawnAIClass) return;

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	ACharacter* SpawnAI = GetWorld()->SpawnActor<ACharacter>(SpawnAIClass, SpawnLocation, SpawnRotation);

	if (ASPTEnemyCharacter* SPTEnemyCharacter = Cast<ASPTEnemyCharacter>(SpawnAI))
	{
		SPTEnemyCharacter->PatrolRoute = this->PatrolRoute;
		SPTEnemyCharacter->OnDeathDelegate.AddDynamic(this, &ASplineAISpawnPoint::Respawn);
	}
}

void ASplineAISpawnPoint::Respawn()
{
	// AI�� Ư�� �ð� �� ������ ��Ű�� ����

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASplineAISpawnPoint::SpawnAI, RespawnDelay, false);
}
