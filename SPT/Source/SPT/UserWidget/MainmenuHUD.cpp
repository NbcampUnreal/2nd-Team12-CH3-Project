// Fill out your copyright notice in the Description page of Project Settings.


#include "MainmenuHUD.h"
#include "MainMenuButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainmenuHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (PlayButton)
	{
		PlayButton->SetButtonName(FText::FromString("PLAY"));
		PlayButton->GetMainMenuButton()->OnClicked.AddDynamic(this, &UMainmenuHUD::HandlePlayButtonClicked);
	}
	if (TrainingButton)
	{
		TrainingButton->SetButtonName(FText::FromString("TRAINING"));
		TrainingButton->GetMainMenuButton()->OnClicked.AddDynamic(this, &UMainmenuHUD::HandleTrainingButtonClicked);
	}
	if (EndButton)
	{
		EndButton->SetButtonName(FText::FromString("EXIT"));
		EndButton->GetMainMenuButton()->OnClicked.AddDynamic(this, &UMainmenuHUD::HandleEndButtonClicked);
	}

}

void UMainmenuHUD::HandlePlayButtonClicked()
{
	// TODO
	// 나중에 플레이 할 레벨이 생성되면 오픈하도록 설정
	//UGameplayStatics::OpenLevel(GetWorld(), FName(""));
}

void UMainmenuHUD::HandleTrainingButtonClicked()
{
	if (APlayerController* CurPlayerController = GetOwningPlayer())
	{
		CurPlayerController->SetPause(false);
		CurPlayerController->SetInputMode(FInputModeGameOnly());
		CurPlayerController->SetShowMouseCursor(false);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("PracticeRangeLevel"));
}

void UMainmenuHUD::HandleEndButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
