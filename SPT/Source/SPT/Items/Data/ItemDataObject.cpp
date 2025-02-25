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

	// FItemData 구조체에서 데이터 로드
	FItemData* FoundData = ItemDataTable->FindRow<FItemData>(RowName, TEXT(""));
	if (FoundData)
	{
		ItemData = *FoundData;  // 아이템 데이터를 로드
		Quantity = 1; // 기본 값 설정
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
	// 스택 가능 여부 확인 (아이템이 같은 타입이고, 스택 가능 여부)
	return (ItemData.ItemID == OtherItem->ItemData.ItemID) && ItemData.NumericData.bIsStackable;
}

int32 UItemDataObject::MergeStack(UItemDataObject* OtherItem)
{
	if (!CanStackWith(OtherItem)) return 0;

	// 병합 가능한 최대 수량만큼만 병합
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

	// 현재 스택에서 분리할 만큼 차감
	SetQuantity(Quantity - SplitQuantity);

	// 새로운 UItemDataObject 생성 (Outer는 현재 객체의 Outer 사용)
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
