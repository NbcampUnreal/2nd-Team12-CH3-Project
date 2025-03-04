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
	// 숙련도 테스트 기능 시작
	void ProficiencyTestingStart();

	// 숙련도 테스트 기능 끝내기
	void ProficiencyTestingEnd();

protected:
	// 숙련도 테스트 기능을 위한 AI 스폰
	UFUNCTION()
	void ProficiencyTestingAISpawn();
	// 숙련도 테스트 기능이 끝난 경우 AI 제거
	UFUNCTION()
	void ProficiencyTestingAIDestroy();

private:
	int32 ProficiencyTestSpawnAICount;

	TObjectPtr<AAISpawnVolume> ProficiencyTestSpawnVolume;

	TObjectPtr<APracticeScoreBoard> PracticeScoreBoardActor;
};
