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
	// 기본 아이템 데이터 로드
	if (InItemDataTable)
	{
		FItemData* FoundData = InItemDataTable->FindRow<FItemData>(RowName, TEXT(""));
		if (FoundData)
		{
			ItemData = *FoundData;
			Quantity = 1;  // 기본 수량 설정
		}
	}

	// 무기 데이터 로드 (무기일 경우)
	if (IsWeapon() && InWeaponDataTable)
	{
		FWeaponItemData* WeaponInfo = InWeaponDataTable->FindRow<FWeaponItemData>(RowName, TEXT(""));
		if (WeaponInfo)
		{
			WeaponData = *WeaponInfo;
			bHasWeaponData = true;  // 무기 데이터가 로드되었음을 표시
		}
	}
	/*
	// 소비 아이템 데이터 로드 (소비 아이템일 경우)
	if (IsConsumable() && InConsumableDataTable)
	{
		FConsumableItemData* ConsumableInfo = InConsumableDataTable->FindRow<FConsumableItemData>(RowName, TEXT(""));
		if (ConsumableInfo)
		{
			ConsumableData = *ConsumableInfo;
			bHasConsumableData = true;  // 소비 아이템 데이터가 로드되었음을 표시
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

/* 아래는 임시로 작성해 놓은 코드입니다.
 * 인벤토리 등 추후의 상황에 맞게 수정이 필요합니다.
**/

bool UItemDataObject::CanStackWith(const UItemDataObject* OtherItem) const
{
	if (!OtherItem)
	{
		return false;
	}

	// 스택 가능 여부 확인 (같은 아이템 && 스택 가능 && 같은 유형)
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

	// 병합 가능한 최대 수량만큼만 병합
	int32 MaxStack = ItemData.NumericData.MaxStackSize;
	int32 AvailableSpace = MaxStack - Quantity;
	int32 MergedAmount = FMath::Min(AvailableSpace, OtherItem->Quantity);

	// 현재 스택에 병합
	Quantity += MergedAmount;
	OtherItem->Quantity -= MergedAmount;

	return MergedAmount; // 실제 병합된 개수 반환
}
UItemDataObject* UItemDataObject::SplitStack(int32 SplitQuantity)
{
	if (SplitQuantity <= 0 || SplitQuantity >= Quantity)
	{
		return nullptr;
	}

	// 현재 스택에서 분리할 만큼 차감
	SetQuantity(Quantity - SplitQuantity);

	// 새로운 UItemDataObject 생성
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
