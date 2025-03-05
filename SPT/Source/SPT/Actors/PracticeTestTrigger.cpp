// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeTestTrigger.h"
#include "Components/BoxComponent.h"
#include "PracticeRangeGameState.h"

APracticeTestTrigger::APracticeTestTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);
	// ���� �������ǵ��� ����
	Collision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	// Overlap �̺�Ʈ ���ε�
	Collision->OnComponentBeginOverlap.AddDynamic(this, &APracticeTestTrigger::OnBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &APracticeTestTrigger::OnEndOverlap);
}

void APracticeTestTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void APracticeTestTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APracticeTestTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (APracticeRangeGameState* PracticeRangeGameState = GetWorld()->GetGameState<APracticeRangeGameState>())
		{
			// ���� ������Ʈ�� ���� AI ��� ����
			PracticeRangeGameState->ProficiencyTestingStart();
		}
	}
}

void APracticeTestTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (APracticeRangeGameState* PracticeRangeGameState = GetWorld()->GetGameState<APracticeRangeGameState>())
		{
			// ���� ������Ʈ�� ���� AI ��� ����
			PracticeRangeGameState->ProficiencyTestingEnd();
		}
	}
}

