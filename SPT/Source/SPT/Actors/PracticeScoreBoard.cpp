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
	// �� �ʱ�ȭ
	RemainingTime = Time;
	Score = 0;

	// UI ����
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

	// Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(ScoreBoardTimerHandle, this, &APracticeScoreBoard::ReduceRemainingTime, 1.f, false);
}

void APracticeScoreBoard::EndScoreBoard()
{
	// UI ����
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
	// ���� �ð� ����
	--RemainingTime;

	// UI ����
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

	// �ð��� ���Ҵٸ� �ݺ�
	if (RemainingTime > 0.f)
	{
		GetWorldTimerManager().SetTimer(ScoreBoardTimerHandle, this, &APracticeScoreBoard::ReduceRemainingTime, 1.f, false);
	}
	// ���� �ð��� ���ٸ� ���ε��� �Լ� ȣ��
	else
	{
		OnPracticeTestEndDelegate.Broadcast();
	}
}

void APracticeScoreBoard::AddScore()
{
	// ���� ����
	++Score;

	// UI ����
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

