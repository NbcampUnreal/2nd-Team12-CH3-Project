// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuButton.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMainMenuButton::SetButtonName(FText NewButtonName)
{
	ButtonName->SetText(NewButtonName);
}

UButton* UMainMenuButton::GetMainMenuButton()
{

	return MainMenuButton;
}
