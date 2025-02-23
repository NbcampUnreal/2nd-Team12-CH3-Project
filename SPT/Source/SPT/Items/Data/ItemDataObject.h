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

	/* ������ ��� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* ������ ������ �������� / ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetItemData() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemData(const FItemData& NewItemData);

	/* ������ ���� �������� / ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* �������� Ÿ�� Ȯ�� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsConsumable() const;

	/* ���� ���� �Լ� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemStackWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsFullItemStack() const;

	/* �������� �κ��丮�� ���� �ִ��� Ȯ�� */
	// UFUNCTION(BlueprintCallable, Category = "Item")
	// bool IsInInventory() const;

	/* ���� ���� �Լ� */
	/* �� �������� ���� �������� Ȯ�� (���� ItemID, ���� ����) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanMergeWith(const UItemDataObject* OtherItem) const;

	/* �ٸ� �������� ������ ���� �����ۿ� �����ϰ�, ���� �� OtherItem�� ���� ������ ��ȯ */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 MergeStack(UItemDataObject* OtherItem);

	/* ���� ���ÿ��� SplitQuantity ��ŭ �и��Ͽ� ���ο� ������ ��ü�� ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* SplitStack(int32 SplitQuantity);

protected:
	/* ������ ���� ID �� ������ */
	bool operator==(const FName& OtherID) const;

	/* ������ ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;

	/* ���� ������ ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	/* �������� ���� �κ��丮 */
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;

};
