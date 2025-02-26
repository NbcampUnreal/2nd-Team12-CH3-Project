// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "WeaponDataStructs.h"
#include "ConsumableDataStructs.h"
#include "ItemDataObject.generated.h"

class ASPTPlayerCharacter;

UCLASS()
class SPT_API UItemDataObject : public UObject
{
	GENERATED_BODY()

public:
	/* �⺻ ������ */
	UItemDataObject();

	/* ������ ������ �ʱ�ȭ */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializeItemData(FName RowName);

	/* ������ ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* CreateItemCopy() const;

	/* ������ ������ �������� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	const FItemData& GetItemData() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemData(const FItemData& NewItemData);

	/* ���� ������ �������� */
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	const FWeaponItemData& GetWeaponData() const;
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	void SetWeaponData(const FWeaponItemData& NewWeaponData);

	/* �Һ� ������ ������ �������� */
	UFUNCTION(BlueprintCallable, Category = "Item|Caonsumables")
	const FConsumableItemData& GetConsumableata() const;
	UFUNCTION(BlueprintCallable, Category = "Item|Caonsumables")
	void SetConsumableData(const FConsumableItemData& NewConsumableData);

	/* ������ ��� (�켱 �Һ� ������) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* ������ ���� �������� �� ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* ������ Ÿ�� Ȯ�� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsConsumable() const;

	/* ������ ���� �� ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanStackWith(const UItemDataObject* OtherItem) const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 MergeStack(UItemDataObject* OtherItem);
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* SplitStack(int32 SplitQuantity);

protected:
	/* ������ ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* ItemDataTable;

	/* ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FWeaponItemData WeaponData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* WeaponDataTable;

	/* �Һ� ������ ������ (�ʿ��� �� �ε��) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Consumable")
	FConsumableItemData ConsumableData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* ConsumableDataTable;

	/* ���� ������ ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	/* �������� ���� ĳ�� */
	bool bHasWeaponData;

	/* �Һ� ���������� ���� ĳ�� */
	bool bHasConsumableData;

};
