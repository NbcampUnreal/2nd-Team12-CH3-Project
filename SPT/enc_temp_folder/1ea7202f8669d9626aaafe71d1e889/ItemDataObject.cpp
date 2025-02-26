// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"

UItemDataObject::UItemDataObject()
{
	Quantity = 1;
	bHasWeaponData = false;
	bHasConsumableData = false;
}

void UItemDataObject::InitializeFromDataTable(
	UDataTable* InItemDataTable, 
	UDataTable* InWeaponDataTable, 
	UDataTable* InConsumableDataTable,
	FName RowName)
{
	// �⺻ ������ ������ �ε�
	if (InItemDataTable)
	{
		FItemData* FoundData = InItemDataTable->FindRow<FItemData>(RowName, TEXT(""));
		if (FoundData)
		{
			ItemData = *FoundData;
			Quantity = 1;  // �⺻ ���� ����
		}
	}

	// ���� ������ �ε� (������ ���)
	if (IsWeapon() && InWeaponDataTable)
	{
		FWeaponItemData* WeaponInfo = InWeaponDataTable->FindRow<FWeaponItemData>(RowName, TEXT(""));
		if (WeaponInfo)
		{
			WeaponData = *WeaponInfo;
			bHasWeaponData = true;  // ���� �����Ͱ� �ε�Ǿ����� ǥ��
		}
	}
	/*
	// �Һ� ������ ������ �ε� (�Һ� �������� ���)
	if (IsConsumable() && InConsumableDataTable)
	{
		FConsumableItemData* ConsumableInfo = InConsumableDataTable->FindRow<FConsumableItemData>(RowName, TEXT(""));
		if (ConsumableInfo)
		{
			ConsumableData = *ConsumableInfo;
			bHasConsumableData = true;  // �Һ� ������ �����Ͱ� �ε�Ǿ����� ǥ��
		}
	}
	*/
}

UItemDataObject* UItemDataObject::CreateItemCopy() const
{
	UItemDataObject* NewItem = NewObject<UItemDataObject>();
	if (NewItem)
	{
		NewItem->ItemData = ItemData;
		NewItem->Quantity = Quantity;
		NewItem->WeaponData = WeaponData;
		// NewItem->ConsumableData = ConsumableData;
		NewItem->bHasWeaponData = bHasWeaponData;
		NewItem->bHasConsumableData = bHasConsumableData;
	}
	return NewItem;
}

const FItemData& UItemDataObject::GetItemData() const
{
	return ItemData;
}

void UItemDataObject::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
}

const FWeaponItemData& UItemDataObject::GetWeaponData() const
{
	return WeaponData;
}

/*
const FConsumableItemData* UItemDataObject::GetConsumableData() const
{
	return bHasConsumableData ? &ConsumableData : nullptr;
}
*/

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

bool UItemDataObject::IsWeapon() const
{
	return ItemData.ItemType == EItemType::EIT_Weapon;
}

bool UItemDataObject::IsConsumable() const
{
	return ItemData.ItemType == EItemType::EIT_Consumable;
}

/* �Ʒ��� �ӽ÷� �ۼ��� ���� �ڵ��Դϴ�.
 * �κ��丮 �� ������ ��Ȳ�� �°� ������ �ʿ��մϴ�.
**/

bool UItemDataObject::CanStackWith(const UItemDataObject* OtherItem) const
{
	if (!OtherItem)
	{
		return false;
	}

	// ���� ���� ���� Ȯ�� (���� ������ && ���� ���� && ���� ����)
	return (ItemData.ItemID == OtherItem->ItemData.ItemID) &&
		(ItemData.NumericData.bIsStackable) &&
		(ItemData.ItemType == OtherItem->ItemData.ItemType);
}

int32 UItemDataObject::MergeStack(UItemDataObject* OtherItem)
{
	if (!OtherItem || !CanStackWith(OtherItem))
	{
		return 0;
	}

	// ���� ������ �ִ� ������ŭ�� ����
	int32 MaxStack = ItemData.NumericData.MaxStackSize;
	int32 AvailableSpace = MaxStack - Quantity;
	int32 MergedAmount = FMath::Min(AvailableSpace, OtherItem->Quantity);

	// ���� ���ÿ� ����
	Quantity += MergedAmount;
	OtherItem->Quantity -= MergedAmount;

	return MergedAmount; // ���� ���յ� ���� ��ȯ
}
UItemDataObject* UItemDataObject::SplitStack(int32 SplitQuantity)
{
	if (SplitQuantity <= 0 || SplitQuantity >= Quantity)
	{
		return nullptr;
	}

	// ���� ���ÿ��� �и��� ��ŭ ����
	SetQuantity(Quantity - SplitQuantity);

	// ���ο� UItemDataObject ����
	UItemDataObject* NewStack = NewObject<UItemDataObject>(this->GetOuter(), this->GetClass());
	if (NewStack)
	{
		NewStack->ItemData = ItemData;
		NewStack->Quantity = SplitQuantity;
		NewStack->WeaponData = WeaponData;
		// NewStack->ConsumableData = ConsumableData;
		NewStack->bHasWeaponData = bHasWeaponData;
		NewStack->bHasConsumableData = bHasConsumableData;
	}
	return NewStack;
}
