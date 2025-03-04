// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryInterface.h"
#include "EquipmentSlotInventory.generated.h"

UCLASS()
class SPT_API AEquipmentSlotInventory : public AActor, public IInventoryInterface
{
	GENERATED_BODY()
	

public:	// ������ private�� ��������� �׽�Ʈ ������ ��� �ٲ��
	// int32 ��� �������� ����ϰ� ������ ������ ������Ʈ�� ������ �������� ��� �����;����� ���� �ϴ� ���ڷ� ��ü
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipSlot")
	TMap<int32, UInventoryItem*> EquippedItems;

public:	
	// Sets default values for this actor's properties
	AEquipmentSlotInventory();

	virtual void AddItem(UInventoryItem* Item) override;
	virtual void DropItem(UInventoryItem* Item) override;
	virtual bool HasItem(FName ItemName) override;
	virtual bool RemoveItem(UInventoryItem* Item) override;
	virtual const TArray<UInventoryItem*>& GetInventory() const override;

	bool EquipItem(UInventoryItem* Item, int32 Slot);
	UInventoryItem* UnequipItem(int32 Slot);
	UInventoryItem* IsEquippedSlot(UInventoryItem* Item);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
