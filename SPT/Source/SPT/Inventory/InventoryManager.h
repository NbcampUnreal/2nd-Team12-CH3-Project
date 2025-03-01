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
