// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeRangeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "AISpawnVolume.h"
#include "BaseCharacter.h"

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
    ProficiencyTestingStart();
}

void APracticeRangeGameState::ProficiencyTestingStart()
{
    // 특정 시간 안에 AI를 잡는 로직 구현
    
    // 스폰해야하는 횟수만큼 반복
    for (int32 i = 0; i < ProficiencyTestSpawnAICount; ++i)
    {
        // AI 스폰
        ProficiencyTestingAISpawn();
    }
}

void APracticeRangeGameState::ProficiencyTestingAISpawn()
{
    if (ProficiencyTestSpawnVolume)
    {
        ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ProficiencyTestSpawnVolume->SpawnAI());

        if (BaseCharacter)
        {
            // 스폰 시킨 AI가 사망된 후 스폰 함수 다시 호출
            BaseCharacter->OnDeathDelegate.AddDynamic(this, &APracticeRangeGameState::ProficiencyTestingAISpawn);
            BaseCharacter->OnDeathDelegate.AddDynamic(this, &APracticeRangeGameState::ProficiencyTestingAIKillCount);
        }
    }
}

void APracticeRangeGameState::ProficiencyTestingAIKillCount()
{
    // AI가 처치됐다면 UI에 반영하는 로직
}
