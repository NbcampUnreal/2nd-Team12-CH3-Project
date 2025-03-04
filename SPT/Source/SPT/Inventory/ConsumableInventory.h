// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryInterface.h"
#include "ConsumableInventory.generated.h"

UCLASS()
class SPT_API AConsumableInventory : public AActor, public IInventoryInterface
{
	GENERATED_BODY()
	
private:
	TArray<UInventoryItem*> ConsumableItems;

public:	
	AConsumableInventory();

	virtual void AddItem(UInventoryItem* Item) override;
	virtual void DropItem(UInventoryItem* Item) override;
	virtual bool HasItem(FName ItemName) override;
	virtual bool RemoveItem(UInventoryItem* Item) override;
	virtual const TArray<UInventoryItem*>& GetInventory() const override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
