// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "ItemDataObject.generated.h"

class ASPTPlayerCharacter;
class UInventoryComponent;

UCLASS()
class SPT_API UItemDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	/* �⺻ ������ */
	UItemDataObject();

	/* ������ ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* CreateItemCopy() const;

	/* ���� ���� �Լ� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemStackWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsFullItemStack() const;

	/* ������ ��� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* ������ ������ �������� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetItemData() const;

	/* ������ ������ ���� */
	void SetItemData(const FItemData& NewItemData);

	/* ������ ���� �������� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;

	/* ������ ���� ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* �������� �������� Ȯ�� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsWeapon() const;

	/* �������� �Ҹ�ǰ���� Ȯ�� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsConsumable() const;

	/* �������� �κ��丮�� ���� �ִ��� Ȯ�� */
	// UFUNCTION(BlueprintCallable, Category = "Item")
	// bool IsInInventory() const;

protected:
	/* ������ ���� ID �� ������ */
	bool operator==(const FName& OtherID) const;

	/* ������ ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;

	/* �������� ���� �κ��丮 */
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;

	/* ���� ������ ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

};
