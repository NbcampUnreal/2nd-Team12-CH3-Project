// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PracticeScoreBoard.generated.h"

class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPracticeTestEndDelegate);

UCLASS()
class SPT_API APracticeScoreBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	APracticeScoreBoard();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	// AI 사격의 시작과 끝에 변수를 세팅하고 관리하는 함수
	void StartScoreBoard(float Time);
	void EndScoreBoard();

	// AI가 죽었을 경우 점수를 올리고, 갱신하도록 하는 함수
	// 델리게이트를 통해 바인딩 된 함수임
	UFUNCTION()
	void AddScore();

private:
	// AI 사격의 남은 시간을 감소하고 남은 시간을 확인하는 함수
	void ReduceRemainingTime();

public:
	// AI 사격이 끝났을 때 실행해야할 함수를 바인딩할 델리게이트
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnPracticeTestEndDelegate OnPracticeTestEndDelegate;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> ScoreWidget;

	// 남은 시간동안 1초마다 실행될 타이머
	FTimerHandle ScoreBoardTimerHandle;

	// 남은 시간
	float RemainingTime;
	// 현재 점수
	int Score;
};
