// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PracticeRangeGameState.generated.h"

class AAISpawnVolume;

UCLASS()
class SPT_API APracticeRangeGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	APracticeRangeGameState();

protected:
	virtual void BeginPlay() override;

	// ���õ� �׽�Ʈ ��� ����
	void ProficiencyTestingStart();
	// ���õ� �׽�Ʈ ����� ���� AI ����
	UFUNCTION()
	void ProficiencyTestingAISpawn();
	// ���õ� �׽�Ʈ ����� ���� ų ī��Ʈ
	UFUNCTION()
	void ProficiencyTestingAIKillCount();

private:
	int32 ProficiencyTestSpawnAICount;

	TObjectPtr<AAISpawnVolume> ProficiencyTestSpawnVolume;
};
