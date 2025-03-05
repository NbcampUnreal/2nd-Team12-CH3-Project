// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryInterface.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "SPTPlayerCharacter.h"         // �ӽÿ�
#include "SPT/Items/Weapons/WeaponBase.h"   // �ӽÿ�
#include "SPT/Items/Weapons/FirearmWeapon.h"    // �ӽÿ�
#include "SPT/Items/Worlds/WorldWeapon.h"   // �ӽÿ�
#include "SPT/Inventory/EquipmentSlotInventory.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Inventory/ItemWidget/InventoryMainWidget.h"

// Sets default values
AInventoryManager::AInventoryManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
        TArray<UInventoryItem*> DisplayItems = GetDisplayInventoryItems();
        InventoryMainWidgetInstance->UpdateInventoryList(DisplayItems);
    }
}

// �̱���(���ǰ ���� Ȥ�� �Ҹ�ǰ ���)
//void AInventoryManager::UseItem(UInventoryItem* Item)
//{
//    UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Start"));
//    if (!Item)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : ItemData is null"));
//        return;
//    }
//
//    // ������ Ÿ�Կ� ���� �ൿ�� �ٸ��� ó��
//    if (Item->IsWeapon())
//    {
//        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Start"));
//        if (!Item)
//        {
//            UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : ItemData is null"));
//            return;
//        }
//
//        if (Item->IsWeapon())
//        {
//            UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : This is Weapon"));
//
//            // ���� ������ ���Ⱑ �ִٸ� ���� ó��
//            if (EquipmentSlotInventory)
//            {
//                int32 Slot = Item->GetSlotType();
//                UInventoryItem* ExistingItem = EquipmentSlotInventory->GetEquippedSlot(Item);
//                if (ExistingItem)
//                {
//                    EquipmentSlotInventory->UnequipItem(Slot);
//                    if (EquipmentInventory)
//                    {
//                        EquipmentInventory->AddItem(ExistingItem);
//                    }
//                }
//                EquipmentSlotInventory->EquipItem(Item, Slot);
//            }
//
//            //// ���� ���� ���� �� ���� ó��
//            // ���� ���, ������ �����Ϳ� ������ ���� Ŭ������ ����Ǿ� �ִٰ� ����
//            TSubclassOf<AWeaponBase> WeaponClass = Item->GetWeaponClass();
//            if (!WeaponClass)
//            {
//                UE_LOG(LogTemp, Error, TEXT("InventoryManager::UseItem - WeaponClass is not set"));
//                return;
//            }
//
//            UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : WeaponClass is set"));
//
//            // �÷��̾� ĳ���Ϳ� ���� (��: �κ��丮 �Ŵ����� ������ ������ ������ �ִٰ� ����)
//            if (ASPTPlayerCharacter* PlayerCharacter = Cast<ASPTPlayerCharacter>(GetOwner()))
//            {
//                UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Player call"));
//                FVector SpawnLocation = PlayerCharacter->GetMesh()->GetSocketLocation("RightHandSocket");
//                FRotator SpawnRotation = PlayerCharacter->GetActorRotation();
//                AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnLocation, SpawnRotation);
//                if (SpawnedWeapon)
//                {
//                    // ������ ������ ����
//                    SpawnedWeapon->SetItemData(Item->ItemDataObject);
//                    SpawnedWeapon->SetWeaponData(Item->ItemDataObject->GetWeaponData());
//
//                    // ���� ���� ����
//                    SpawnedWeapon->AttachToComponent(PlayerCharacter->GetMesh(),
//                        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
//                        "RightHandSocket");
//
//                    // �÷��̾� ĳ���� �� ������ ���� ������Ʈ
//                    PlayerCharacter->EquipWeapon(SpawnedWeapon);
//                    UE_LOG(LogTemp, Log, TEXT("InventoryManager::UseItem - Weapon equipped: %s"), *SpawnedWeapon->GetName());
//                }
//                else
//                {
//                    UE_LOG(LogTemp, Error, TEXT("InventoryManager::UseItem - Failed to spawn weapon actor"));
//                }
//            }
//
//            // �κ��丮 ��Ͽ��� ���� ������ ���� �� UI ������Ʈ
//            for (auto& Inventory : Inventories)
//            {
//                if (Inventory.GetObject() != EquipmentSlotInventory)
//                {
//                    Inventory->RemoveItem(Item);
//                }
//            }
//        }
//        else if (Item->IsConsumable())
//        {
//            Item->UseItem();
//        }
//
//        // UI ����
//        if (InventoryMainWidgetInstance)
//        {
//            UpdateInventoryUI();
//        }
//    }
//}

// �ߺ������� ���ܵ� ���� �Լ�
void AInventoryManager::UseItem(UInventoryItem* Item)
{
    UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Start"));
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : ItemData is null"));
        return;
    }

    // ������ Ÿ�Կ� ���� �ൿ�� �ٸ��� ó��
    if (Item->IsWeapon())
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Start"));
        if (!Item)
        {
            UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : ItemData is null"));
            return;
        }

        if (Item->IsWeapon())
        {
            UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : This is Weapon"));

            // ���� ������ ���Ⱑ �ִٸ� ���� ó��
            if (EquipmentSlotInventory)
            {
                TSubclassOf<AWeaponBase> WeaponClass = Item->GetWeaponClass();
                if (!WeaponClass)
                {
                    UE_LOG(LogTemp, Error, TEXT("UseItem - ItemBaseClass is null!"));
                    return;
                }

                UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Get WeaponClass"));

                ASPTPlayerCharacter* PlayerCharacter = Cast<ASPTPlayerCharacter>(GetOwner());
                if (!PlayerCharacter) {
                    UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : PlayerCharacter is null"));
                    return;
                }

                UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Get PlayerCharacter"));

                FVector SpawnLocation = PlayerCharacter->GetActorLocation(); // �÷��̾� ��ġ �Ǵ� ���ϴ� ��ġ
                FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

                // ���� ���� ����
                AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, SpawnLocation, SpawnRotation);
                if (!SpawnedWeapon)
                {
                    UE_LOG(LogTemp, Error, TEXT("UseItem - Failed to spawn weapon!"));
                    return;
                }

                UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Spawn SpawnedWeapon"));


                // �κ��丮�� UItemDataObject�� ���� ���Ϳ� ����
                SpawnedWeapon->SetItemData(Item->ItemDataObject);
                SpawnedWeapon->SetWeaponData(Item->ItemDataObject->GetWeaponData());

                // ������� �浹�� ������ ó�� (�̹� ������ ����� �浹�� �ʿ䰡 �����Ƿ�)
                SpawnedWeapon->SetActorEnableCollision(false);

                UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Set Data"));

                int32 Slot = Item->GetSlotType();
                UInventoryItem* ExistingItem = EquipmentSlotInventory->GetEquippedSlot(Item);
                if (ExistingItem)
                {
                    EquipmentSlotInventory->UnequipItem(Slot);
                    PlayerCharacter->UnEquipWeapon();
                    if (EquipmentInventory)
                    {
                        EquipmentInventory->AddItem(ExistingItem);
                    }
                }
                EquipmentSlotInventory->EquipItem(Item, Slot);
                AItemBase* ItemBase = Item->GetItemBase(); // ����� AItemBase ��������


                AWorldWeapon* WorldWeapon = Cast<AWorldWeapon>(ItemBase);
                if (WorldWeapon && ItemBase->GetItemData()->WeaponData.WeaponType == EWeaponType::EWT_Firearm)
                {
                    WorldWeapon->OnPickup(PlayerCharacter);
                }

                UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : Equip SpawnedWeapon"));
            }

            for (auto& Inventory : Inventories)
            {
                if (Inventory.GetObject() != EquipmentSlotInventory)
                {
                    Inventory->RemoveItem(Item);
                }
            }
        }
        else if (Item->IsConsumable())
        {
            Item->UseItem();
        }

        // UI ����
        if (InventoryMainWidgetInstance)
        {
            UpdateInventoryUI();
        }
    }
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
        UpdateInventoryUI();
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

TArray<UInventoryItem*> AInventoryManager::GetDisplayInventoryItems() const
{
    TArray<UInventoryItem*> DisplayItems = EquipmentInventory->GetInventory();
    // ���� �Ҹ�ǰ �κ��丮�� �ִٸ� �߰�
    DisplayItems.Append(ConsumableInventory->GetInventory());

    // ���� EquipmentSlotInventory�� �ִ� �������� ����
    TArray<UInventoryItem*> EquippedItems = EquipmentSlotInventory->GetInventory();
    for (UInventoryItem* EquippedItem : EquippedItems)
    {
        DisplayItems.Remove(EquippedItem);
    }
    return DisplayItems;
}


void AInventoryManager::UpdateInventoryUI()
{
    if (InventoryMainWidgetInstance)
    {
        // �������� ���� ������ ����Ʈ ����
        TArray<UInventoryItem*> DisplayItems = GetDisplayInventoryItems();
        InventoryMainWidgetInstance->UpdateInventoryList(DisplayItems);

        // ������ ������(��� ����) ����
        InventoryMainWidgetInstance->UpdateEquipmentSlots(EquipmentSlotInventory);
    }
}

// Called when the game starts or when spawned
void AInventoryManager::BeginPlay()
{
	Super::BeginPlay();


    if (EquipmentInventoryClass)
    {
        EquipmentInventory = GetWorld()->SpawnActor<AEquipmentInventory>(EquipmentInventoryClass);
        RegisterInventory(EquipmentInventory);
    }

    if (ConsumableInventoryClass)
    {
        ConsumableInventory = GetWorld()->SpawnActor<AConsumableInventory>(ConsumableInventoryClass);
        RegisterInventory(ConsumableInventory);
    }

    if (EquipmentSlotInventoryClass)
    {
        EquipmentSlotInventory = GetWorld()->SpawnActor<AEquipmentSlotInventory>(EquipmentSlotInventoryClass);
        RegisterInventory(EquipmentSlotInventory);
    }
}

// Called every frame
void AInventoryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

