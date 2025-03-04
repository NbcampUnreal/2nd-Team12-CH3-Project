// Fill out your copyright notice in the Description page of Project Settings.


#include "PracticeScoreBoard.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

APracticeScoreBoard::APracticeScoreBoard()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	ScoreWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScoreWidget"));
	ScoreWidget->SetupAttachment(StaticMesh);
}

void APracticeScoreBoard::BeginPlay()
{
	Super::BeginPlay();
	
	RemainingTime = 0.f;
	Score = 0;
}

void APracticeScoreBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APracticeScoreBoard::StartScoreBoard(float Time)
{
	// 값 초기화
	RemainingTime = Time;
	Score = 0;

	// UI 갱신
	if (ScoreWidget)
	{
		UUserWidget* ScoreWidgetInstance = ScoreWidget->GetUserWidgetObject();
		if (ScoreWidgetInstance)
		{
			if (UTextBlock* RemainingTimeTextBlock = Cast<UTextBlock>(ScoreWidgetInstance->GetWidgetFromName(TEXT("RemainingTimeTextBlock"))))
			{
				RemainingTimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(RemainingTime))));
			}

			if (UTextBlock* ScoreTextBlock = Cast<UTextBlock>(ScoreWidgetInstance->GetWidgetFromName(TEXT("ScoreTextBlock"))))
			{
				ScoreTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
			}
		}
	}

	// 타이머 시작
	GetWorldTimerManager().SetTimer(ScoreBoardTimerHandle, this, &APracticeScoreBoard::ReduceRemainingTime, 1.f, false);
}

void APracticeScoreBoard::EndScoreBoard()
{
	// UI 갱신
	if (ScoreWidget)
	{
		UUserWidget* ScoreWidgetInstance = ScoreWidget->GetUserWidgetObject();
		if (ScoreWidgetInstance)
		{
			if (UTextBlock* RemainingTimeTextBlock = Cast<UTextBlock>(ScoreWidgetInstance->GetWidgetFromName(TEXT("RemainingTimeTextBlock"))))
			{
				RemainingTimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("0"))));
			}
		}
	}

	GetWorldTimerManager().ClearTimer(ScoreBoardTimerHandle);
	OnPracticeTestEndDelegate.Clear();
}

void APracticeScoreBoard::ReduceRemainingTime()
{
	// 남은 시간 감소
	--RemainingTime;

	// UI 갱신
	if (ScoreWidget)
	{
		UUserWidget* ScoreWidgetInstance = ScoreWidget->GetUserWidgetObject();
		if (ScoreWidgetInstance)
		{
			if (UTextBlock* RemainingTimeTextBlock = Cast<UTextBlock>(ScoreWidgetInstance->GetWidgetFromName(TEXT("RemainingTimeTextBlock"))))
			{
				RemainingTimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(RemainingTime))));
			}
		}
	}

	// 시간이 남았다면 반복
	if (RemainingTime > 0.f)
	{
		GetWorldTimerManager().SetTimer(ScoreBoardTimerHandle, this, &APracticeScoreBoard::ReduceRemainingTime, 1.f, false);
	}
	// 남은 시간이 없다면 바인딩된 함수 호출
	else
	{
		OnPracticeTestEndDelegate.Broadcast();
	}
}

void APracticeScoreBoard::AddScore()
{
	// 점수 변경
	++Score;

	// UI 갱신
	if (ScoreWidget)
	{
		UUserWidget* ScoreWidgetInstance = ScoreWidget->GetUserWidgetObject();
		if (ScoreWidgetInstance)
		{
			if (UTextBlock* ScoreTextBlock = Cast<UTextBlock>(ScoreWidgetInstance->GetWidgetFromName(TEXT("ScoreTextBlock"))))
			{
				ScoreTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
			}
		}
	}
}

