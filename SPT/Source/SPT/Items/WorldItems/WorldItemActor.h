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
	/* 기본 생성자 */
	AWorldItemActor();

	/* 아이템 줍기 및 버리기 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter);
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);
	
};
