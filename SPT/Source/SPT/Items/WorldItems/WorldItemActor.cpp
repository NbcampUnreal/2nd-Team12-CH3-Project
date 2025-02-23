// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemActor.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Weapons/WeaponActor.h"
// #include "InventoryComponent.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = false;
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

    if (ItemID.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("WorldItemActor has no valid ItemID!"));
        return;
    }

    const FString ContextString = TEXT("ItemDataTable Lookup");
    FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
    if (!FoundItemData)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemID '%s' not found in ItemDataTable!"), *ItemID.ToString());
        return;
    }

    // 데이터 가져오기
    SetItemData(*FoundItemData);
    UE_LOG(LogTemp, Log, TEXT("Item '%s' initialized with ItemType: %d"), *ItemID.ToString(), static_cast<int32>(ItemData.ItemType));
}

void AWorldItemActor::InitializeItem(const FItemData& NewItemData)
{
    SetItemData(NewItemData);

    UE_LOG(LogTemp, Log, TEXT("AWorldItemActor Initialized with ItemType: %d (Expected Weapon: %d, Consumable: %d)"),
        static_cast<int32>(GetItemData().ItemType),
        static_cast<int32>(EItemType::EIT_Weapon),
        static_cast<int32>(EItemType::EIT_Consumable));
}

void AWorldItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    FItemData CurrentItemData = GetItemData();

    // 무기 아이템인 경우, 무기 액터를 스폰하여 장착 처리
    if (CurrentItemData.ItemType == EItemType::EIT_Weapon)
    {
        AWeaponActor* NewWeapon = GetWorld()->SpawnActor<AWeaponActor>(AWeaponActor::StaticClass());
        if (NewWeapon)
        {
            NewWeapon->SetItemData(ItemData);
            NewWeapon->UpdateMeshForState(EItemState::EIS_Equipped);

            if (PlayerCharacter->EquipItem(NewWeapon))
            {
                UE_LOG(LogTemp, Log, TEXT("WorldItemActor: %s equip %s"), *PlayerCharacter->GetName(), *GetName());
                Destroy(); // 장착 후 월드에서 제거
                return;
            }
        }
    }
    // 소비 아이템 및 일반 아이템
    else
    {
        // 인벤토리 컴포넌트 가져오기
        /*
        // 인벤토리에 추가
        if (PlayerCharacter->InventoryComponent)
        {
            PlayerCharacter->InventoryComponent->AddItem(ItemData);
            UE_LOG(LogTemp, Log, TEXT("WorldItemActor: %s added %s to inventory"),
                *PlayerCharacter->GetName(),
                *ItemData.TextData.Name.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("WorldItemActor: PlayerCharacter has no InventoryComponent!"));
        }

        */

        UE_LOG(LogTemp, Warning, TEXT("%s has picked up %s"), *PlayerCharacter->GetName(), *ItemData.TextData.Name.ToString());
        Destroy(); // 나중에 인벤토리로 넣을것
    }
}

void AWorldItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnDrop: PlayerCharacter is null"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("OnDrop: World is null"));
        return;
    }

    // 드랍 오프셋을 상수로 정의
    static const FVector DefaultDropOffset(50.f, 0.f, 0.f);

    // 아이템을 버릴 때 월드에 다시 생성
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PlayerCharacter;
    SpawnParams.Instigator = PlayerCharacter;

    AWorldItemActor* DroppedItem = GetWorld()->SpawnActor<AWorldItemActor>(GetClass(),
        PlayerCharacter->GetActorLocation() + DefaultDropOffset,
        FRotator::ZeroRotator,
        SpawnParams);

    if (DroppedItem)
    {
        // 아이템 개수 등 정보 유지 가능
        DroppedItem->SetItemData(ItemData);

        UE_LOG(LogTemp, Warning, TEXT("%s has dropped %s (Quantity: %d)"),
            *PlayerCharacter->GetName(),
            *DroppedItem->GetItemData().TextData.Name.ToString(),
            DroppedItem->GetItemData().NumericData.MaxStackSize);
    }
}

void AWorldItemActor::BeginFocus()
{
    UE_LOG(LogTemp, Log, TEXT("WorldItemActor BeginFocus: %s"), *GetName());
}

void AWorldItemActor::EndFocus()
{
    UE_LOG(LogTemp, Log, TEXT("WorldItemActor EndFocus: %s"), *GetName());
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
                SetItemData(*FoundItemData);
            }
        }
    }
}
