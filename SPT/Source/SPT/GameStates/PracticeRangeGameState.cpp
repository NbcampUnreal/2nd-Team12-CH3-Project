// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeRangeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "AISpawnVolume.h"
#include "PracticeScoreBoard.h"
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
    TArray<AActor*> FoundArr;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawnVolume::StaticClass(), FoundArr);

    for (AActor* Found : FoundArr)
    {
        // 찾은 액터가 ProficiencyTesting용 스폰 불륨이라면
        if (Found->ActorHasTag("ProficiencyTest"))
        {
            ProficiencyTestSpawnVolume = Cast<AAISpawnVolume>(Found);
        }
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APracticeScoreBoard::StaticClass(), FoundArr);
    for (AActor* Found : FoundArr)
    {
        // 찾은 액터가 APracticeScoreBoard라면
        if (Found->ActorHasTag("PracticeScoreBoard"))
        {
            PracticeScoreBoardActor = Cast<APracticeScoreBoard>(Found);
        }
    }
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

    // 남은 시간 및 점수 카운트
    if (PracticeScoreBoardActor)
    {
        PracticeScoreBoardActor->StartScoreBoard(10.f);
        PracticeScoreBoardActor->OnPracticeTestEndDelegate.AddDynamic(this, &APracticeRangeGameState::ProficiencyTestingAIDestroy);
    }
}

void APracticeRangeGameState::ProficiencyTestingEnd()
{
    ProficiencyTestingAIDestroy();
    PracticeScoreBoardActor->EndScoreBoard();
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
            BaseCharacter->OnDeathDelegate.AddDynamic(PracticeScoreBoardActor, &APracticeScoreBoard::AddScore);
        }
    }
}

void APracticeRangeGameState::ProficiencyTestingAIDestroy()
{
    // 스폰 AI 찾기
    TArray<AActor*> FoundAI;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), FoundAI);

    for (AActor* Found : FoundAI)
    {
        // 찾은 액터가 ProficiencyTestAI라면
        if (Found->ActorHasTag("ProficiencyTestAI"))
        {
            ABaseCharacter* CurChar = Cast<ABaseCharacter>(Found);
            CurChar->Destroy();
        }
    }
}
