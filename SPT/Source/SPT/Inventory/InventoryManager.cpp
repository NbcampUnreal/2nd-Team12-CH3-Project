// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryInterface.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Inventory/ItemWidget/InventoryMainWidget.h"

// Sets default values
AInventoryManager::AInventoryManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInventoryManager::RegisterInventory(AActor* InventoryActor)
{
	if (InventoryActor && InventoryActor->Implements<UInventoryInterface>())
	{
		Inventories.Add(InventoryActor);
	}
}

void AInventoryManager::AddItemToInventory(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : AddItem: Item is nullptr!"));
        return;
    }

    // �κ��丮 �迭�� �����͸� ����(���, �Ҹ�ǰ �����ؼ� ����)
    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory)
        {
            // ��� �κ��丮���� AddItem ȣ��
            Inventory->AddItem(Item);
            break;
        }
    }

    // UI ����
    if (InventoryMainWidgetInstance)
    {
        TArray<UInventoryItem*> AllItems;
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            if (Inventory)
            {
                TArray<UInventoryItem*> Items = Inventory->GetInventory();
                AllItems.Append(Items);
            }
        }
        InventoryMainWidgetInstance->UpdateInventoryList(AllItems);
    }
}

// �̱���(���ǰ ���� Ȥ�� �Ҹ�ǰ ���)
void AInventoryManager::UseItem(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : ItemData is null"));
        return;
    }
    Item->UseItem();
}

// �������� ĳ������ �տ� �����Ͽ� ����߸�
void AInventoryManager::DropItem(UInventoryItem* Item, FVector DropLocation)
{
    if (!Item || !Item->GetItemBaseClass())
    {
        return;
    }


    // �κ��丮�� �����͸� ����
    UItemDataObject* ItemCopy = Item->ItemDataObject->CreateItemCopy();
    if (!ItemCopy)
    {
        UE_LOG(LogTemp, Error, TEXT("DropItem : Failed to create item copy!"));
        return;
    }


    // �������� ���忡 ����
    AItemBase* SpawnedItem = GetWorld()->SpawnActor<AItemBase>(Item->GetItemBaseClass(), DropLocation, FRotator::ZeroRotator);
    // �������� ���忡 ���� �Ǿ��� ��
    if (SpawnedItem)
    {
        // ����� �κ��丮 �����͸� ������ �����ۿ� ����
        SpawnedItem->SetItemData(ItemCopy);

        // �������� ������ ����
        if (ItemCopy->GetItemData().AssetData.SkeletalMesh)
        {
            USkeletalMeshComponent* SkeletalMeshComp = NewObject<USkeletalMeshComponent>(SpawnedItem);
            if (SkeletalMeshComp)
            {
                SkeletalMeshComp->SetSkeletalMesh(ItemCopy->GetItemData().AssetData.SkeletalMesh);
                SkeletalMeshComp->RegisterComponent();

                // ���� ��Ʈ�� ������ �������� �ʰ� Attach�� ����
                if (!SpawnedItem->GetRootComponent())
                {
                    SpawnedItem->SetRootComponent(SkeletalMeshComp);
                }
                else
                {
                    SkeletalMeshComp->AttachToComponent(SpawnedItem->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
                }
            }
        }

        // �κ��丮���� ������ ����
        RemoveItemToInventory(Item);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn dropped item."));
    }
}

// �κ��丮���� �������� �����ϴ� �Լ�
void AInventoryManager::RemoveItemToInventory(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItem: Item is nullptr!"));
        return;
    }

    bool bItemRemoved = false;

    // ��� �κ��丮�� ��ȸ�ϸ鼭 ������ ���� �õ�
    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory->RemoveItem(Item))  // �������̽��� ���� ����
        {
            bItemRemoved = true;
            break;
        }
    }

    // �������� ���ŵǾ����� UI ����
    if (bItemRemoved && InventoryMainWidgetInstance)
    {
        TArray<UInventoryItem*> AllItems;
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            AllItems.Append(Inventory->GetInventory());
        }
        InventoryMainWidgetInstance->UpdateInventoryList(AllItems);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItem: Item not found in any inventory."));
    }
}

void AInventoryManager::SetInventoryWidget(UInventoryMainWidget* NewWidget)
{
    InventoryMainWidgetInstance = NewWidget;
}

// Called when the game starts or when spawned
void AInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

