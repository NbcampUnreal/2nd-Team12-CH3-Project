// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMenu.h"
#include "MainMenuButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDeathMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ReStartButton)
	{
		ReStartButton->SetButtonName(FText::FromString("RESTART"));
		ReStartButton->GetMainMenuButton()->OnClicked.AddDynamic(this, &UDeathMenu::HandleReStartButtonClicked);
	}
	if (MainMenuButton)
	{
		MainMenuButton->SetButtonName(FText::FromString("MAINMENU"));
		MainMenuButton->GetMainMenuButton()->OnClicked.AddDynamic(this, &UDeathMenu::HandleMainMenuButtonClicked);
	}
}

void UDeathMenu::HandleReStartButtonClicked()
{
	if (APlayerController* CurPlayerController = GetOwningPlayer())
	{
		CurPlayerController->SetPause(false);
		CurPlayerController->SetInputMode(FInputModeGameOnly());
		CurPlayerController->SetShowMouseCursor(false);
	}

	UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetCurrentLevel()->GetFName());
}

void UDeathMenu::HandleMainMenuButtonClicked()
{
	if (APlayerController* CurPlayerController = GetOwningPlayer())
	{
		CurPlayerController->SetPause(true);
		CurPlayerController->SetInputMode(FInputModeUIOnly());
		CurPlayerController->SetShowMouseCursor(true);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuLevel"));
}
