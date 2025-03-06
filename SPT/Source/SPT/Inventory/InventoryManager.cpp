// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryInterface.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "SPTPlayerCharacter.h"         // 임시용
#include "SPT/Items/Weapons/WeaponBase.h"   // 임시용
#include "SPT/Items/Weapons/FirearmWeapon.h"    // 임시용
#include "SPT/Items/Worlds/WorldWeapon.h"   // 임시용
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

    // 인벤토리 배열에 데이터를 저장(장비, 소모품 구분해서 저장)
    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory)
        {
            // 모든 인벤토리에서 AddItem 호출
            Inventory->AddItem(Item);
            break;
        }
    }

    // UI 갱신
    if (InventoryMainWidgetInstance)
    {
        TArray<UInventoryItem*> DisplayItems = GetDisplayInventoryItems();
        InventoryMainWidgetInstance->UpdateInventoryList(DisplayItems);
    }
}

void AInventoryManager::UseItem(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : ItemData is null"));
        return;
    }

    // 아이템 타입에 따라 행동을 다르게 처리
    if (Item->IsWeapon())
    {
        EquipItem(Item);
    }
    else if (Item->IsConsumable())
    {
        Item->UseItem();
    }

    // UI 갱신
    if (InventoryMainWidgetInstance)
    {
        UpdateInventoryUI();
    }
}

// 아이템을 캐릭터의 앞에 생성하여 떨어뜨림
void AInventoryManager::DropItem(UInventoryItem* Item, FVector DropLocation)
{
    if (!Item || !Item->GetItemBaseClass())
    {
        return;
    }


    // 인벤토리의 데이터를 복제
    UItemDataObject* ItemCopy = Item->ItemDataObject->CreateItemCopy();
    if (!ItemCopy)
    {
        UE_LOG(LogTemp, Error, TEXT("DropItem : Failed to create item copy!"));
        return;
    }


    // 아이템을 월드에 스폰
    AItemBase* SpawnedItem = GetWorld()->SpawnActor<AItemBase>(Item->GetItemBaseClass(), DropLocation, FRotator::ZeroRotator);
    // 아이템이 월드에 생성 되었을 시
    if (SpawnedItem)
    {
        // 복사된 인벤토리 데이터를 스폰된 아이템에 복사
        SpawnedItem->SetItemData(ItemCopy);

        // 아이템의 에셋을 적용
        if (ItemCopy->GetItemData().AssetData.SkeletalMesh)
        {
            USkeletalMeshComponent* SkeletalMeshComp = NewObject<USkeletalMeshComponent>(SpawnedItem);
            if (SkeletalMeshComp)
            {
                SkeletalMeshComp->SetSkeletalMesh(ItemCopy->GetItemData().AssetData.SkeletalMesh);
                SkeletalMeshComp->RegisterComponent();

                // 기존 루트가 있으면 변경하지 않고 Attach만 수행
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

        // 인벤토리에서 아이템 제거
        RemoveItemToInventory(Item);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn dropped item."));
    }
}

void AInventoryManager::EquipItem(UInventoryItem* Item)
{        // 기존 장착된 무기가 있다면 해제 처리
    if (EquipmentSlotInventory)
    {
        TSubclassOf<AWeaponBase> WeaponClass = Item->GetWeaponClass();
        if (!WeaponClass)
        {
            UE_LOG(LogTemp, Error, TEXT("UseItem - ItemBaseClass is null!"));
            return;
        }

        ASPTPlayerCharacter* PlayerCharacter = Cast<ASPTPlayerCharacter>(GetOwner());
        if (!PlayerCharacter) {
            UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem : PlayerCharacter is null"));
            return;
        }

        FVector SpawnLocation = PlayerCharacter->GetActorLocation(); // 플레이어 위치 또는 원하는 위치
        FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

        ///////////////////////////////////////////////
        //// 현재 장착중인 아이템 제거 (등 뒤에 무기가 중첩되는 것 방지 용도로 추가한 임시 코드 입니다. 필요 시 활성화)
        //if (AWeaponBase* CurrentWeapon = PlayerCharacter->GetEquippedWeapon())
        //{
        //    CurrentWeapon->Destroy(); // 기존 무기 액터 제거
        //}
        ///////////////////////////////////////////////

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

        AItemBase* ItemBase = Item->GetItemBase();
        AWorldWeapon* WorldWeapon = Cast<AWorldWeapon>(ItemBase);

        if (WorldWeapon && ItemBase->GetItemData()->WeaponData.WeaponType == EWeaponType::EWT_Firearm)
        {
            WorldWeapon->OnPickup(PlayerCharacter);
        }
    }

    for (auto& Inventory : Inventories)
    {
        if (Inventory.GetObject() != EquipmentSlotInventory)
        {
            Inventory->RemoveItem(Item);
        }
    }
}

// 인벤토리에서 아이템을 제거하는 함수
void AInventoryManager::RemoveItemToInventory(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItem: Item is nullptr!"));
        return;
    }

    bool bItemRemoved = false;

    // 모든 인벤토리를 순회하면서 아이템 제거 시도
    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory->RemoveItem(Item))  // 인터페이스를 통해 제거
        {
            bItemRemoved = true;
            break;
        }
    }

    // 아이템이 제거되었으면 UI 갱신
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

    if (InventoryMainWidgetInstance)
    {
        // 델리게이트에 바인딩 (동적 바인딩을 위해 UFUNCTION이어야 함)
        InventoryMainWidgetInstance->OnInventoryFilterChanged.AddDynamic(this, &AInventoryManager::UpdateInventoryUI);
    }
}

TArray<UInventoryItem*> AInventoryManager::GetDisplayInventoryItems() const
{
    TArray<UInventoryItem*> DisplayItems;

    if (InventoryMainWidgetInstance->IsEquipmentFilter)
    {
        DisplayItems = EquipmentInventory->GetInventory();
        UE_LOG(LogTemp, Warning, TEXT("Display Weapon"));

        if (InventoryMainWidgetInstance->IsConsumableFilter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Display All"));
            DisplayItems.Append(ConsumableInventory->GetInventory());
        }
    }
    else if (InventoryMainWidgetInstance->IsConsumableFilter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Display consumable"));

        DisplayItems = ConsumableInventory->GetInventory();
    }

    return DisplayItems;
}


void AInventoryManager::UpdateInventoryUI()
{
    if (InventoryMainWidgetInstance)
    {
        // 필터링 된 인벤토리 갱신
        TArray<UInventoryItem*> DisplayItems = GetDisplayInventoryItems();
        InventoryMainWidgetInstance->UpdateInventoryList(DisplayItems);

        // 장착된 아이템(장비 슬롯) 갱신
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

