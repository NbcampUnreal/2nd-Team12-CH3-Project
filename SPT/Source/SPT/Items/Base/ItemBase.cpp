// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component ����
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// ������ ������ ������Ʈ �ʱ�ȭ
	ItemData = CreateDefaultSubobject<UItemDataObject>(TEXT("ItemData"));

	// �⺻ ���´� ���� ����
	ItemState = EItemState::EIS_World;
}

void AItemBase::InitializeItem(FName ItemRowName, UDataTable* ItemDataTable, UDataTable* WeaponTable, UDataTable* ConsumableTable)
{
	if (ItemData)
	{
		// ItemDataObject���� ������ �ʱ�ȭ
		ItemData->InitializeFromDataTable(ItemDataTable, WeaponTable, ConsumableTable, ItemRowName);

		const FItemData& LoadedItemData = ItemData->GetItemData();

		// �޽� ���� (�������Ʈ���� ������ �޽ð� ������ ����)
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
	// StaticMesh ���
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

	// ���� ������ transform�� �״�� ����Ͽ� ����
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
