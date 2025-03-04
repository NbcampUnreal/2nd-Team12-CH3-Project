// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeRangeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "AISpawnVolume.h"
#include "PracticeScoreBoard.h"
#include "BaseCharacter.h"

APracticeRangeGameState::APracticeRangeGameState()
{
    // ProficiencyTest���� ��ȯ�� AI ��
    ProficiencyTestSpawnAICount = 10;
}

void APracticeRangeGameState::BeginPlay()
{
	Super::BeginPlay();

    // ���� �ҷ� ã��
    TArray<AActor*> FoundArr;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawnVolume::StaticClass(), FoundArr);

    for (AActor* Found : FoundArr)
    {
        // ã�� ���Ͱ� ProficiencyTesting�� ���� �ҷ��̶��
        if (Found->ActorHasTag("ProficiencyTest"))
        {
            ProficiencyTestSpawnVolume = Cast<AAISpawnVolume>(Found);
        }
    }

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APracticeScoreBoard::StaticClass(), FoundArr);
    for (AActor* Found : FoundArr)
    {
        // ã�� ���Ͱ� APracticeScoreBoard���
        if (Found->ActorHasTag("PracticeScoreBoard"))
        {
            PracticeScoreBoardActor = Cast<APracticeScoreBoard>(Found);
        }
    }
}

void APracticeRangeGameState::ProficiencyTestingStart()
{
    // Ư�� �ð� �ȿ� AI�� ��� ���� ����
    
    // �����ؾ��ϴ� Ƚ����ŭ �ݺ�
    for (int32 i = 0; i < ProficiencyTestSpawnAICount; ++i)
    {
        // AI ����
        ProficiencyTestingAISpawn();
    }

    // ���� �ð� �� ���� ī��Ʈ
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
            // ���� ��Ų AI�� ����� �� ���� �Լ� �ٽ� ȣ��
            BaseCharacter->OnDeathDelegate.AddDynamic(this, &APracticeRangeGameState::ProficiencyTestingAISpawn);
            BaseCharacter->OnDeathDelegate.AddDynamic(PracticeScoreBoardActor, &APracticeScoreBoard::AddScore);
        }
    }
}

void APracticeRangeGameState::ProficiencyTestingAIDestroy()
{
    // ���� AI ã��
    TArray<AActor*> FoundAI;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), FoundAI);

    for (AActor* Found : FoundAI)
    {
        // ã�� ���Ͱ� ProficiencyTestAI���
        if (Found->ActorHasTag("ProficiencyTestAI"))
        {
            ABaseCharacter* CurChar = Cast<ABaseCharacter>(Found);
            CurChar->Destroy();
        }
    }
}
