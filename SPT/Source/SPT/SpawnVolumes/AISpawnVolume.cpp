// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawnVolume.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AAISpawnVolume::AAISpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

void AAISpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAISpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AAISpawnVolume::GetRandomXYPointInVolume() const
{
	// 박스 컴포넌트의 스케일된 Extent, 즉 x/y/z 방향으로 반지름(절반 길이)을 구함
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	FVector Pointlocation = BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		-BoxExtent.Z);

	return Pointlocation;
}

ACharacter* AAISpawnVolume::SpawnAI()
{
	// 현재 스폰시키는 클래스가 멤버변수로 있다.
	// 나중에 데이터 테이블로 관리한다면 수정해야한다.
	if (!SpawnAIClass) return nullptr;

	FVector SpawnLocation = GetRandomXYPointInVolume();
	FRotator SpawnRotation = GetActorRotation();
	ACharacter* SpawnAI = GetWorld()->SpawnActor<ACharacter>(SpawnAIClass, SpawnLocation, SpawnRotation);

	return SpawnAI;
}

