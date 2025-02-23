// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
#include "SPT/Items/ConsumableItems/ConsumableItemActor.h"
#include "Engine/World.h"

UItemDataObject::UItemDataObject()
{
	Quantity = 1;
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

void UItemDataObject::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemObject: Cannot Use - Character is null!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using item: %s"), *ItemData.TextData.Name.ToString());

	// 소비 아이템일 경우 먼저 액터 소환
	if (IsConsumable())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerCharacter;
		SpawnParams.Instigator = PlayerCharacter;

		AConsumableItemActor* SpawnedItem = PlayerCharacter->GetWorld()->SpawnActor<AConsumableItemActor>(
			AConsumableItemActor::StaticClass(),
			PlayerCharacter->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (SpawnedItem)
		{
			SpawnedItem->SetItemData(ItemData);
			SpawnedItem->Use(PlayerCharacter);
		}
	}

	// 아이템 개수 감소 후, 0이 되면 인벤토리에서 삭제
	SetQuantity(Quantity - 1);
	if (Quantity <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s has been consumed completely! Removing from inventory."), *ItemData.TextData.Name.ToString());
		// TODO: 인벤토리에서 아이템 삭제 처리 필요
	}
}

FItemData UItemDataObject::GetItemData() const
{
	return ItemData;
}

void UItemDataObject::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
	Quantity = 1;
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
		ItemData.NumericData.bIsStackable ? ItemData.NumericData.MaxStackSize : 1);
}

bool UItemDataObject::IsWeapon() const
{
	return ItemData.ItemType == EItemType::EIT_Weapon;
}

bool UItemDataObject::IsConsumable() const
{
	return ItemData.ItemType == EItemType::EIT_Consumable;
}

float UItemDataObject::GetItemStackWeight() const
{
	return ItemData.NumericData.Weight * Quantity;
}

float UItemDataObject::GetItemSingleWeight() const
{
	return ItemData.NumericData.Weight;
}

bool UItemDataObject::IsFullItemStack() const
{
	return Quantity >= ItemData.NumericData.MaxStackSize;
}

bool UItemDataObject::CanMergeWith(const UItemDataObject* OtherItem) const
{
	if (!OtherItem)
	{
		return false;
	}
	// 두 아이템이 스택 가능, 같은 아이템 ID일 경우 병합 가능
	return ItemData.NumericData.bIsStackable && (ItemData.ItemID == OtherItem->GetItemData().ItemID);
}
int32 UItemDataObject::MergeStack(UItemDataObject* OtherItem)
{
	if (!OtherItem || !CanMergeWith(OtherItem))
	{
		// 병합 불가능한 경우 OtherItem의 현재 수량 그대로 반환
		return OtherItem ? OtherItem->GetQuantity() : 0;
	}

	// 최대 스택 크기와 현재 수량에 따른 여유 공간 계산
	int32 MaxStack = ItemData.NumericData.MaxStackSize;
	int32 AvailableSpace = MaxStack - Quantity;
	int32 OtherQuantity = OtherItem->GetQuantity();
	int32 MergedAmount = FMath::Min(AvailableSpace, OtherQuantity);

	// 현재 스택에 병합
	SetQuantity(Quantity + MergedAmount);
	// OtherItem의 수량 차감
	OtherItem->SetQuantity(OtherQuantity - MergedAmount);

	// 병합 후 OtherItem에 남은 수량 반환
	return OtherItem->GetQuantity();
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

/*
bool UItemDataObject::IsInInventory() const
{
	return OwningInventory != nullptr;
}
*/

bool UItemDataObject::operator==(const FName& OtherID) const
{
	return  this->ItemData.ItemID == OtherID;
}
