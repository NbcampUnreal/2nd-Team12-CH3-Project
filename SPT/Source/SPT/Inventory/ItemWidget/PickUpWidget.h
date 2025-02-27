// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PickUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPT_API UPickUpWidget : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PickUpText;
};
