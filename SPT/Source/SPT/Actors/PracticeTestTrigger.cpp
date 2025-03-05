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
	// 폰만 오버랩되도록 설정
	Collision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	// Overlap 이벤트 바인딩
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
			// 게임 스테이트를 통해 AI 사격 시작
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
			// 게임 스테이트를 통해 AI 사격 종료
			PracticeRangeGameState->ProficiencyTestingEnd();
		}
	}
}

