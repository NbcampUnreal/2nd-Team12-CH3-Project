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
	// �ڽ� ������Ʈ�� �����ϵ� Extent, �� x/y/z �������� ������(���� ����)�� ����
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
	// ���� ������Ű�� Ŭ������ ��������� �ִ�.
	// ���߿� ������ ���̺�� �����Ѵٸ� �����ؾ��Ѵ�.
	if (!SpawnAIClass) return nullptr;

	FVector SpawnLocation = GetRandomXYPointInVolume();
	FRotator SpawnRotation = GetActorRotation();
	ACharacter* SpawnAI = GetWorld()->SpawnActor<ACharacter>(SpawnAIClass, SpawnLocation, SpawnRotation);

	return SpawnAI;
}

