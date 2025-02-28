// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class ASPTPlayerCharacter;

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
		Name(FText::GetEmpty()),
		Action(FText::GetEmpty()),
		InteractionDuration(0.0f)
	{
	};

	UPROPERTY(EditInstanceOnly)
	FText Name;
	UPROPERTY(EditInstanceOnly)
	FText Action;
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class SPT_API IInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void BeginFocus() = 0;
	virtual void EndFocus() = 0;
	virtual void BeginInteract() = 0;
	virtual void EndInteract() = 0;
	virtual void Interact(ASPTPlayerCharacter* PlayerCharacter) = 0;

	FInteractableData InteractableData;

};
