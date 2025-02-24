// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainmenuHUD.generated.h"

class UMainMenuButton;

UCLASS()
class SPT_API UMainmenuHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HandlePlayButtonClicked();
	UFUNCTION()
	void HandleTrainingButtonClicked();
	UFUNCTION()
	void HandleEndButtonClicked();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UMainMenuButton> PlayButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UMainMenuButton> TrainingButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UMainMenuButton> EndButton;
};
