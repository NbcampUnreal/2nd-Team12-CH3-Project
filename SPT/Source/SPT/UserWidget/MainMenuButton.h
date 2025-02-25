// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuButton.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class SPT_API UMainMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetButtonName(FText NewButtonName);

	UButton* GetMainMenuButton();


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UButton> MainMenuButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> ButtonName;


};
