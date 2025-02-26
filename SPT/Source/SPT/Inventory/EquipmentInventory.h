// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryInterface.h"
#include "ItemData/EquipmentItem.h"
#include "EquipmentInventory.generated.h"

UCLASS()
class SPT_API AEquipmentInventory : public AActor, public IInventoryInterface
{
	GENERATED_BODY()
	
public:		// 저장 확인용으로 임시로 퍼블릭 (원래는 프라이빗)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UEquipmentItem*> EquipmentItems;

public:	
	AEquipmentInventory();

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
