// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineAISpawnPoint.h"
#include "SPTEnemyCharacter.h"

ASplineAISpawnPoint::ASplineAISpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
}

void ASplineAISpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnAI();
}

ACharacter* ASplineAISpawnPoint::SpawnAI()
{
	// ���� ������Ű�� Ŭ������ ��������� �ִ�.
	// ���߿� ������ ���̺�� �����Ѵٸ� �����ؾ��Ѵ�.
	if (!SpawnAIClass) return nullptr;

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	ACharacter* SpawnAI = GetWorld()->SpawnActor<ACharacter>(SpawnAIClass, SpawnLocation, SpawnRotation);

	if (ASPTEnemyCharacter* SPTEnemyCharacter = Cast<ASPTEnemyCharacter>(SpawnAI))
	{
		SPTEnemyCharacter->PatrolRoute = this->PatrolRoute;
	}

	return SpawnAI;
}
