// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryManager.generated.h"

class IInventoryInterface;
class UInventoryItem;
class UEquipmentItem;
class UConsumableItem;
class AEquipmentInventory;
class AConsumableInventory;
class UInventoryMainWidget;
class AEquipmentSlotInventory;
class AItemBase;

class UItemDataObject;

UCLASS()
class SPT_API AInventoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryManager();

	// 인벤토리를 캐릭터의 등록시키는 함수
	void RegisterInventory(AActor* InventoryActor);
	// 아이템을 인벤토리에 추가하는 함수
	void AddItemToInventory(UInventoryItem* Item);
	// 미구현(장비아이템 착용이나 소모품 사용에 사용될 예정)
	void UseItem(UInventoryItem* Item);
	// 인벤토리의 아이템을 바닥에 생성하는 함수
	void DropItem(UInventoryItem* Item, FVector DropLocation);
	// 아이템을 사용하거나 드랍 시 인벤토리에서 제거하는 함수
	void RemoveItemToInventory(UInventoryItem* Item);
	// 인벤토리 위젯을 할당하는 함수
	void SetInventoryWidget(UInventoryMainWidget* NewWidget);
	// UI에서 표시될 인벤토리를 설정하는 함수(필터링)
	TArray<UInventoryItem*> GetDisplayInventoryItems() const;
	// UI를 업데이트 하는 함수
	void UpdateInventoryUI();

	UPROPERTY()
	UInventoryMainWidget* InventoryMainWidgetInstance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AEquipmentInventory> EquipmentInventoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AConsumableInventory> ConsumableInventoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<AEquipmentSlotInventory> EquipmentSlotInventoryClass;

	UPROPERTY()
	AEquipmentInventory* EquipmentInventory;

	UPROPERTY()
	AConsumableInventory* ConsumableInventory;

	UPROPERTY()
	AEquipmentSlotInventory* EquipmentSlotInventory;

private:
	UPROPERTY()
	TArray<TScriptInterface<IInventoryInterface>> Inventories;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
