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

	// �⺻ ���´� ���� ����
	ItemState = EItemState::EIS_World;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData is nullptr. Creating new ItemDataObject."));
		ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());

		if (!ItemData)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create ItemDataObject!"));
			return;  // �浹 ����
		}
	}

	ItemData->InitializeItemData(ItemData->GetItemData().ItemID);
}

void AItemBase::InitializeItem(FName ItemRowName)
{
	if (ItemData)
	{
		// ItemDataObject���� ������ �ʱ�ȭ
		ItemData->InitializeItemData(ItemRowName);

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
		UE_LOG(LogTemp, Warning, TEXT("AItemBase::SetItemData called. Keeping existing item state."));
	}
}
