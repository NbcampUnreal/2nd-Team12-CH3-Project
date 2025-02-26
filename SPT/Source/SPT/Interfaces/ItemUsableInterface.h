// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemUsableInterface.generated.h"

class ASPTPlayerCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemUsableInterface : public UInterface
{
	GENERATED_BODY()
};

class SPT_API IItemUsableInterface
{
	GENERATED_BODY()

public:
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter) = 0;

};
