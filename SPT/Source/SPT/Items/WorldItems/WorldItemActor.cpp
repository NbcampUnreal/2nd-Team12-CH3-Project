// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemActor.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Base/EquipableItem.h"
#include "SPT/Items/Weapons/WeaponActor.h"
// #include "InventoryComponent.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 픽업 아이템의 메쉬 설정
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
    PickupMesh->SetSimulatePhysics(true);
    SetRootComponent(PickupMesh);
}

void AWorldItemActor::BeginPlay()
{
    Super::BeginPlay();

    // 아이템 데이터를 DataTable에서 로드
    InitializeItemFromDataTable();
}

void AWorldItemActor::InitializeItemFromDataTable()
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataTable is not assigned in WorldItemActor!"));
        return;
    }

    const FString ContextString = TEXT("ItemDataTable Lookup");
    FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);

    if (!FoundItemData)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemID '%s' not found in ItemDataTable!"), *ItemID.ToString());
        return;
    }

    // 데이터를 가져와서 적용
    ItemData = *FoundItemData;
    UE_LOG(LogTemp, Log, TEXT("Item '%s' initialized with ItemType: %d"), *ItemID.ToString(), static_cast<int32>(ItemData.ItemType));

    // Static Mesh 설정 (데이터 기반)
    if (ItemData.AssetData.StaticMesh)
    {
        PickupMesh->SetStaticMesh(ItemData.AssetData.StaticMesh);
    }
}

void AWorldItemActor::InitializeItem(const FItemData& NewItemData)
{
    SetItemData(NewItemData);

    UE_LOG(LogTemp, Log, TEXT("AWorldItemActor Initialized with ItemType: %d (Expected Weapon: %d, Consumable: %d)"),
        static_cast<int32>(GetItemData().ItemType),
        static_cast<int32>(EItemType::EIT_Weapon),
        static_cast<int32>(EItemType::EIT_Consumable));

    // StaticMesh 설정 (데이터 기반)
    if (NewItemData.AssetData.StaticMesh)
    {
        PickupMesh->SetStaticMesh(NewItemData.AssetData.StaticMesh);
    }
}

void AWorldItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    FItemData CurrentItemData = GetItemData();
    // 무기인지 확인
    if (CurrentItemData.ItemType == EItemType::EIT_Weapon)
    {
        AWeaponActor* NewWeapon = GetWorld()->SpawnActor<AWeaponActor>(AWeaponActor::StaticClass());
        if (NewWeapon)
        {
            if (PlayerCharacter->EquipItem(NewWeapon))
            {
                UE_LOG(LogTemp, Log, TEXT("WorldItemActor: %s equip %s"), *PlayerCharacter->GetName(), *GetName());
                Destroy(); // 장착 후 월드에서 제거
                return;
            }
        }
    }
    else
    {
        // 인벤토리 컴포넌트 가져오기
        /*
        UInventoryComponent* Inventory = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
        if (!Inventory)
        {
            UE_LOG(LogTemp, Warning, TEXT("WorldItemActor: %s의 인벤토리 컴포넌트를 찾을 수 없음!"), *PlayerCharacter->GetName());
            return;
        }
        */

        // 아이템 데이터 객체 생성
        UItemDataObject* NewItemDataObject = NewObject<UItemDataObject>();
        NewItemDataObject->SetItemData(ItemData);
        NewItemDataObject->SetQuantity(1);

        /* 인벤토리 컴포넌트 생성 시 해제
        if (Inventory->AddItem(NewItemDataObject))
        {
            // 아이템 주운 후 제거
            Destroy();
            UE_LOG(LogTemp, Log, TEXT("WorldItemActor: %s 아이템을 인벤토리에 추가 완료!"), *ItemData.TextData.Name.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WorldItemActor: 인벤토리에 공간이 부족함!"));
        }
        */

        UE_LOG(LogTemp, Warning, TEXT("%s has picked up %s"), *PlayerCharacter->GetName(), *ItemData.TextData.Name.ToString());
        Destroy(); // 나중에 인벤토리로 넣을것
    }
}

void AWorldItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    // 아이템을 버릴 때 월드에 다시 생성
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PlayerCharacter;

    AWorldItemActor* DroppedItem = GetWorld()->SpawnActor<AWorldItemActor>(GetClass(), PlayerCharacter->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

    if (DroppedItem)
    {
        // 아이템 개수 등 정보 유지 가능
        DroppedItem->SetItemData(ItemData);
        DroppedItem->SetQuantity(Quantity);

        UE_LOG(LogTemp, Warning, TEXT("%s has dropped %s (Quantity: %d)"),
            *PlayerCharacter->GetName(),
            *DroppedItem->GetItemData().TextData.Name.ToString(),
            DroppedItem->GetItemData().NumericData.MaxStackSize);
    }
}

void AWorldItemActor::BeginFocus()
{
    if (PickupMesh)
    {
        PickupMesh->SetRenderCustomDepth(true);
    }
}

void AWorldItemActor::EndFocus()
{
    if (PickupMesh)
    {
        PickupMesh->SetRenderCustomDepth(false);
    }
}

void AWorldItemActor::BeginInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

void AWorldItemActor::EndInteract()
{
    UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

void AWorldItemActor::Interact(ASPTPlayerCharacter* PlayerCharacter)
{
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));
        OnPickup(PlayerCharacter);
    }
}

void AWorldItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

    if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldItemActor, ItemID))
    {
        if (ItemDataTable)
        {
            const FString ContextString{ ItemID.ToString() };

            if (const FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ItemID.ToString()))
            {
                PickupMesh->SetStaticMesh(FoundItemData->AssetData.StaticMesh);
            }
        }
    }
}
