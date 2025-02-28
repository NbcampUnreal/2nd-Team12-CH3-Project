// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeRangeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "AISpawnVolume.h"
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
    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawnVolume::StaticClass(), FoundVolumes);

    for (AActor* Found : FoundVolumes)
    {
        // ã�� ���Ͱ� ProficiencyTesting�� ���� �ҷ��̶��
        if (Found->ActorHasTag("ProficiencyTest"))
        {
            ProficiencyTestSpawnVolume = Cast<AAISpawnVolume>(Found);
        }
    }

    // �׽�Ʈ �ڵ�
    ProficiencyTestingStart();
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
            BaseCharacter->OnDeathDelegate.AddDynamic(this, &APracticeRangeGameState::ProficiencyTestingAIKillCount);
        }
    }
}

void APracticeRangeGameState::ProficiencyTestingAIKillCount()
{
    // AI�� óġ�ƴٸ� UI�� �ݿ��ϴ� ����
}
