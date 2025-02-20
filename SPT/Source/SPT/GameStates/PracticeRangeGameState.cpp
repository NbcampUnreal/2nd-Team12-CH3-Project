// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeRangeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "AISpawnVolume.h"

APracticeRangeGameState::APracticeRangeGameState()
{
    // ProficiencyTest에서 소환할 AI 수
    ProficiencyTestSpawnAICount = 10;
}

void APracticeRangeGameState::BeginPlay()
{
	Super::BeginPlay();

    // 스폰 불륨 찾기
    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawnVolume::StaticClass(), FoundVolumes);

    for (AActor* Found : FoundVolumes)
    {
        // 찾은 액터가 ProficiencyTesting용 스폰 불륨이라면
        if (Found->ActorHasTag("ProficiencyTest"))
        {
            ProficiencyTestSpawnVolume = Cast<AAISpawnVolume>(Found);
        }
    }

    // 테스트 코드
	ProficiencyTestingStart(ProficiencyTestSpawnAICount);
}

void APracticeRangeGameState::ProficiencyTestingStart(int32 SpawnCount)
{
    // 스폰해야하는 횟수만큼 반복
	for (int32 i = 0; i < SpawnCount; ++i)
	{
        if (ProficiencyTestSpawnVolume)
        {
            ProficiencyTestSpawnVolume->SpawnAI();
        }
	}
}
