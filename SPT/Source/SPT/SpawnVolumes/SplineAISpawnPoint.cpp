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
	// 현재 스폰시키는 클래스가 멤버변수로 있다.
	// 나중에 데이터 테이블로 관리한다면 수정해야한다.
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
