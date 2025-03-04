// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Consumables/ConsumableItem.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"

UItemDataObject::UItemDataObject()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Blueprints/Items/ItemData.ItemData"));
	if (DataTableFinder.Succeeded())
	{
		ItemDataTable = DataTableFinder.Object;
	}

	if (IsWeapon())
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableFinder(TEXT("/Game/Blueprints/Items/WeaponData.WeaponData"));

		if (WeaponDataTableFinder.Succeeded())
		{
			WeaponDataTable = WeaponDataTableFinder.Object;
		}
	}

	Quantity = 1;
	bHasWeaponData = false;
	bHasConsumableData = false;
}

void UItemDataObject::InitializeItemData(FName RowName)
{
	// ItemDataTable이 존재하는지 확인
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataObject::InitializeItemData: ItemDataTable is NULL for %s"), *GetName());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemDataObject::InitializeItemData: Attempting to load %s for %s"), *RowName.ToString(), *GetName());

	// 기본 아이템 데이터 로드
	if (ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataObject::InitializeItemData: %s for %s"), *RowName.ToString(), *GetName());
		if (FItemData* FoundData = ItemDataTable->FindRow<FItemData>(RowName, TEXT("")))
		{
			ItemData = *FoundData;
			Quantity = 1;
			bHasWeaponData = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemDataObject::InitializeItemData: Failed to load the ItemDataTable using %s for %s"), *RowName.ToString(), *GetName());
			return;
		}
	}

	if (ItemData.ItemBaseClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitializeItemData: ItemActorClass for %s is %s"),
			*RowName.ToString(), *ItemData.ItemBaseClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeItemData: ItemActorClass is NULL for %s"), *RowName.ToString());
	}


	// 무기 데이터 로드 (무기일 경우)
	if (IsWeapon())
	{
		if (WeaponDataTable)
		{
			if (FWeaponItemData* FoundWeaponData = WeaponDataTable->FindRow<FWeaponItemData>(RowName, TEXT("")))
			{
				if (!bHasWeaponData)
				{
					WeaponData = *FoundWeaponData;
					bHasWeaponData = true;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ItemDataObject::InitializeItemData: Failed to load the WeaponDataTable for %s"), *GetName());
				return;
			}
		}
	}
	
	// 소비 아이템 데이터 로드 (소비 아이템일 경우)
	if (IsConsumable() && ConsumableDataTable)
	{
		FConsumableItemData* ConsumableInfo = ConsumableDataTable->FindRow<FConsumableItemData>(RowName, TEXT(""));
		if (ConsumableInfo)
		{
			if (!bHasConsumableData)
			{
				ConsumableData = *ConsumableInfo;
				Quantity = ConsumableData.Quantity;
				bHasConsumableData = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ItemDataObject::InitializeItemData: Failed to load the ConsumableDataTable for %s"), *GetName());
				return;
			}
		}
	}
}

UItemDataObject* UItemDataObject::CreateItemCopy() const
{
	UItemDataObject* NewItem = NewObject<UItemDataObject>();
	if (NewItem)
	{
		NewItem->ItemData = ItemData;
		NewItem->Quantity = Quantity;
		NewItem->WeaponData = WeaponData;
		NewItem->ConsumableData = ConsumableData;
		NewItem->bHasWeaponData = bHasWeaponData;
		NewItem->bHasConsumableData = bHasConsumableData;
		NewItem->ItemBaseClass = ItemBaseClass;		// 없어도 되지 않을까 싶은 부분
		NewItem->Icon = Icon;						// 없어도 되지 않을까 싶은 부분
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

void UItemDataObject::SetWeaponData(const FWeaponItemData& NewWeaponData)
{
	WeaponData = NewWeaponData;
}

const FConsumableItemData& UItemDataObject::GetConsumableata() const
{
	return ConsumableData;
}

void UItemDataObject::SetConsumableData(const FConsumableItemData& NewConsumableData)
{
	ConsumableData = NewConsumableData;
}

void UItemDataObject::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UItemDataObject::Use - Invaild PlayerCharacter"));
		return;
	}

	if (IsConsumable())
	{
		FVector SpawnLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * 50.0f;
		FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

		AConsumableItem* UseActor = PlayerCharacter->GetWorld()->SpawnActor<AConsumableItem>(AConsumableItem::StaticClass());
		if (UseActor)
		{
			
			if (ConsumableData.Quantity > 0)
			{
				ConsumableData.Quantity--;
				UseActor->CreateItemCopy();
				UseActor->PrimaryAction(PlayerCharacter);
				UE_LOG(LogTemp, Warning, TEXT("UItemDataObject::Use - Use Consumable Item"));
			}

			if (ConsumableData.Quantity <= 0)
			{
				// PlayerCharacter->RemoveFromInventory(this);
			}
		}
	}
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

