// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitMarker.generated.h"

/**
 * 
 */
UCLASS()
class SPT_API UHitMarker : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayAnimHitMarker();
	
private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Anim_Hit;
};
