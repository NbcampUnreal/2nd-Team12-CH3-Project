// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryManager.generated.h"

class IInventoryInterface;
class UInventoryItem;
class UEquipmentItem;
class UConsumableItem;
class UInventoryMainWidget;


UCLASS()
class SPT_API AInventoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryManager();

	void RegisterInventory(AActor* InventoryActor);
	void AddItemToInventory(UInventoryItem* Item);

	void SetInventoryWidget(UInventoryMainWidget* NewWidget);

	UPROPERTY()
	UInventoryMainWidget* InventoryMainWidgetInstance;


private:
	UPROPERTY()
	TArray<TScriptInterface<IInventoryInterface>> Inventories;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
