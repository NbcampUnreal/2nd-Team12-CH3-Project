// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeRangeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "AISpawnVolume.h"

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
	ProficiencyTestingStart(ProficiencyTestSpawnAICount);
}

void APracticeRangeGameState::ProficiencyTestingStart(int32 SpawnCount)
{
    // �����ؾ��ϴ� Ƚ����ŭ �ݺ�
	for (int32 i = 0; i < SpawnCount; ++i)
	{
        if (ProficiencyTestSpawnVolume)
        {
            ProficiencyTestSpawnVolume->SpawnAI();
        }
	}
}
