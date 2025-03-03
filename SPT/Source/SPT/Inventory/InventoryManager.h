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
