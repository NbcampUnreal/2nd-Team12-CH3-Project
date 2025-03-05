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
	void StartScoreBoard(float Time);

	void EndScoreBoard();

	UFUNCTION()
	void AddScore();

private:
	void ReduceRemainingTime();

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnPracticeTestEndDelegate OnPracticeTestEndDelegate;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> ScoreWidget;

	FTimerHandle ScoreBoardTimerHandle;

	// 남은 시간
	float RemainingTime;
	// 현재 점수
	int Score;
};
