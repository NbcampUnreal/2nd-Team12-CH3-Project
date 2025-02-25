// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"

UItemDataObject::UItemDataObject()
{
	Quantity = 1;
}

void UItemDataObject::InitializeFromDataTable(UDataTable* ItemDataTable, FName RowName)
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable"));
		return;
	}

	// FItemData ����ü���� ������ �ε�
	FItemData* FoundData = ItemDataTable->FindRow<FItemData>(RowName, TEXT(""));
	if (FoundData)
	{
		ItemData = *FoundData;  // ������ �����͸� �ε�
		Quantity = 1; // �⺻ �� ����
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item row with name %s not found in DataTable!"), *RowName.ToString());
	}
}

UItemDataObject* UItemDataObject::CreateItemCopy() const
{
	UItemDataObject* NewItem = NewObject<UItemDataObject>();
	if (NewItem)
	{
		NewItem->SetItemData(ItemData);
		NewItem->SetQuantity(Quantity);
	}
	return NewItem;
}

FItemData UItemDataObject::GetItemData() const
{
	return ItemData;
}

void UItemDataObject::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
}

int32 UItemDataObject::GetQuantity() const
{
	return Quantity;
}

void UItemDataObject::SetQuantity(const int32 NewQuantity)
{
	Quantity = FMath::Clamp(
		NewQuantity,
		0,
		ItemData.NumericData.MaxStackSize);
}

bool UItemDataObject::CanStackWith(const UItemDataObject* OtherItem) const
{
	// ���� ���� ���� Ȯ�� (�������� ���� Ÿ���̰�, ���� ���� ����)
	return (ItemData.ItemID == OtherItem->ItemData.ItemID) && ItemData.NumericData.bIsStackable;
}

int32 UItemDataObject::MergeStack(UItemDataObject* OtherItem)
{
	if (!CanStackWith(OtherItem)) return 0;

	// ���� ������ �ִ� ������ŭ�� ����
	int32 MergedAmount = FMath::Min(ItemData.NumericData.MaxStackSize - Quantity, OtherItem->Quantity);
	Quantity += MergedAmount;
	OtherItem->SetQuantity(OtherItem->Quantity - MergedAmount);

	return MergedAmount;
}
UItemDataObject* UItemDataObject::SplitStack(int32 SplitQuantity)
{
	if (SplitQuantity <= 0 || SplitQuantity >= Quantity)
	{
		return nullptr;
	}

	// ���� ���ÿ��� �и��� ��ŭ ����
	SetQuantity(Quantity - SplitQuantity);

	// ���ο� UItemDataObject ���� (Outer�� ���� ��ü�� Outer ���)
	UItemDataObject* NewStack = NewObject<UItemDataObject>(this->GetOuter(), this->GetClass());
	if (NewStack)
	{
		NewStack->SetItemData(ItemData);
		NewStack->SetQuantity(SplitQuantity);
	}
	return NewStack;
}

bool UItemDataObject::operator==(const FName& OtherID) const
{
	return  this->ItemData.ItemID == OtherID;
}
