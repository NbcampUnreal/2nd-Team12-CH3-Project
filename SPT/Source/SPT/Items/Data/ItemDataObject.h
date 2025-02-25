// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "ItemDataObject.generated.h"

UCLASS()
class SPT_API UItemDataObject : public UObject
{
	GENERATED_BODY()

public:
	/* �⺻ ������ */
	UItemDataObject();

	/* ������ ���̺��� ������ �ʱ�ȭ */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializeFromDataTable(UDataTable* ItemDataTable, FName RowName);

	/* ������ ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* CreateItemCopy() const;

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

	/* �������� ���� �������� Ȯ�� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanStackWith(const UItemDataObject* OtherItem) const;

	/* ������ ���� �� ���� */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 MergeStack(UItemDataObject* OtherItem);
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* SplitStack(int32 SplitQuantity);

protected:
	/* ������ ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;

	/* ���� ������ ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	/* ������ ���� ID �� ������ */
	bool operator==(const FName& OtherID) const;

};
