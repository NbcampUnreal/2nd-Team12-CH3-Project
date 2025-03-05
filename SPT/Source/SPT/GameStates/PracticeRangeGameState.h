// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PracticeRangeGameState.generated.h"

class AAISpawnVolume;
class APracticeScoreBoard;
class APracticeTestTrigger;

UCLASS()
class SPT_API APracticeRangeGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	APracticeRangeGameState();

protected:
	virtual void BeginPlay() override;

public:
	// ���õ� �׽�Ʈ ��� ����
	void ProficiencyTestingStart();

	// ���õ� �׽�Ʈ ��� ������
	void ProficiencyTestingEnd();

protected:
	// ���õ� �׽�Ʈ ����� ���� AI ����
	UFUNCTION()
	void ProficiencyTestingAISpawn();
	// ���õ� �׽�Ʈ ����� ���� ��� AI ����
	UFUNCTION()
	void ProficiencyTestingAIDestroy();

private:
	int32 ProficiencyTestSpawnAICount;

	TObjectPtr<AAISpawnVolume> ProficiencyTestSpawnVolume;

	TObjectPtr<APracticeScoreBoard> PracticeScoreBoardActor;
};
