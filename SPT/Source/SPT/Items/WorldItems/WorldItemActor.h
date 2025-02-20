// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/ItemBaseActor.h"
#include "WorldItemActor.generated.h"

UCLASS()
class SPT_API AWorldItemActor : public AItemBaseActor
{
	GENERATED_BODY()

public:
	/* �⺻ ������ */
	AWorldItemActor();

	/* ������ �ݱ� �� ������ */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter);
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);
	
};
