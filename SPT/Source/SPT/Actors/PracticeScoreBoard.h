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
	// AI ����� ���۰� ���� ������ �����ϰ� �����ϴ� �Լ�
	void StartScoreBoard(float Time);
	void EndScoreBoard();

	// AI�� �׾��� ��� ������ �ø���, �����ϵ��� �ϴ� �Լ�
	// ��������Ʈ�� ���� ���ε� �� �Լ���
	UFUNCTION()
	void AddScore();

private:
	// AI ����� ���� �ð��� �����ϰ� ���� �ð��� Ȯ���ϴ� �Լ�
	void ReduceRemainingTime();

public:
	// AI ����� ������ �� �����ؾ��� �Լ��� ���ε��� ��������Ʈ
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnPracticeTestEndDelegate OnPracticeTestEndDelegate;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> ScoreWidget;

	// ���� �ð����� 1�ʸ��� ����� Ÿ�̸�
	FTimerHandle ScoreBoardTimerHandle;

	// ���� �ð�
	float RemainingTime;
	// ���� ����
	int Score;
};
