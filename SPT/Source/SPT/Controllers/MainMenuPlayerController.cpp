// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
	MainMenuWidgetClass = nullptr;
	MainMenuWidgetInstance = nullptr;
}

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);

		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
		}
	}
}
