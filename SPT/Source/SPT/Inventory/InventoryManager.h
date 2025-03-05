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

	// �κ��丮�� ĳ������ ��Ͻ�Ű�� �Լ�
	void RegisterInventory(AActor* InventoryActor);
	// �������� �κ��丮�� �߰��ϴ� �Լ�
	void AddItemToInventory(UInventoryItem* Item);
	// �̱���(�������� �����̳� �Ҹ�ǰ ��뿡 ���� ����)
	void UseItem(UInventoryItem* Item);
	// �κ��丮�� �������� �ٴڿ� �����ϴ� �Լ�
	void DropItem(UInventoryItem* Item, FVector DropLocation);
	// �������� ����ϰų� ��� �� �κ��丮���� �����ϴ� �Լ�
	void RemoveItemToInventory(UInventoryItem* Item);
	// �κ��丮 ������ �Ҵ��ϴ� �Լ�
	void SetInventoryWidget(UInventoryMainWidget* NewWidget);
	// UI���� ǥ�õ� �κ��丮�� �����ϴ� �Լ�(���͸�)
	TArray<UInventoryItem*> GetDisplayInventoryItems() const;
	// UI�� ������Ʈ �ϴ� �Լ�
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
