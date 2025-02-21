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

	// 숙련도 테스트 기능 시작
	void ProficiencyTestingStart(int32 SpawnCount);

private:
	int32 ProficiencyTestSpawnAICount;

	TObjectPtr<AAISpawnVolume> ProficiencyTestSpawnVolume;
};
