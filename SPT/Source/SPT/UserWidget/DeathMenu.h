// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathMenu.generated.h"

class UMainMenuButton;

UCLASS()
class SPT_API UDeathMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HandleReStartButtonClicked();
	UFUNCTION()
	void HandleMainMenuButtonClicked();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UMainMenuButton> ReStartButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UMainMenuButton> MainMenuButton;
};
