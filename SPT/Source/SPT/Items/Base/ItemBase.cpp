// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// 아이템 데이터 오브젝트 초기화
	ItemData = CreateDefaultSubobject<UItemDataObject>(TEXT("ItemData"));

	// 기본 상태는 월드 상태
	ItemState = EItemState::EIS_World;
}

void AItemBase::InitializeItem(FName ItemRowName, UDataTable* ItemDataTable, UDataTable* WeaponTable, UDataTable* ConsumableTable)
{
	if (ItemData)
	{
		// ItemDataObject에서 데이터 초기화
		ItemData->InitializeFromDataTable(ItemDataTable, WeaponTable, ConsumableTable, ItemRowName);

		const FItemData& LoadedItemData = ItemData->GetItemData();

		// 메시 설정 (블루프린트에서 설정된 메시가 있으면 적용)
		if (LoadedItemData.AssetData.StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(LoadedItemData.AssetData.StaticMesh);
			StaticMeshComponent->SetVisibility(true);
		}
	}
}

void AItemBase::UpdateMeshForState(EItemState NewState)
{
	ItemState = NewState;

	if (!ItemData)
	{
		return;
	}

	const FItemData& Data = ItemData->GetItemData();
	// StaticMesh 사용
	if (Data.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(Data.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
	}
}

AItemBase* AItemBase::CreateItemCopy() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateItemCopy failed: World is null."));
		return nullptr;
	}

	// 현재 액터의 transform을 그대로 사용하여 복제
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItemBase* NewItem = World->SpawnActor<AItemBase>(GetClass(), GetActorTransform(), SpawnParams);
	if (NewItem && ItemData)
	{
		NewItem->SetItemData(ItemData->CreateItemCopy());
		NewItem->SetItemState(ItemState);
	}

	return NewItem;
}

EItemState AItemBase::GetItemState() const
{
	return ItemState;
}

void AItemBase::SetItemState(EItemState NewItemState)
{
	if (NewItemState != EItemState::EIS_MAX)
	{
		ItemState = NewItemState;
		UpdateMeshForState(NewItemState);
	}
}

UItemDataObject* AItemBase::GetItemData() const
{
	return ItemData;
}

void AItemBase::SetItemData(UItemDataObject* NewItemData)
{
	if (NewItemData)
	{
		ItemData = NewItemData;
	}
}
