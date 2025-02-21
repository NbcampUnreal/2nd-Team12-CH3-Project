// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemActor.h"
#include "SPTPlayerCharacter.h"
// #include "InventoryComponent.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 픽업 아이템의 메쉬 설정
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
    PickupMesh->SetSimulatePhysics(true);
    SetRootComponent(PickupMesh);
}

void AWorldItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    // UE_LOG(LogTemp, Warning, TEXT("%s has picked up %s"), *PlayerCharacter->GetName(), *ItemData.ItemName.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s has picked up item"), *PlayerCharacter->GetName());
    Destroy(); // 아이템 줍기 후 제거

	/* 인벤토리 컴포넌트 생성 시 해제
    UInventoryComponent* Inventory = PlayerCharacter->FindComponentByClass<UInventoryComponent>();

    if (Inventory)
    {
        Inventory->AddItem(this);  // 인벤토리에 추가
        Destroy();  // 월드에서 제거
    }
	*/ 
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
        // DroppedItem->SetItemData(ItemData);
        DroppedItem->SetQuantity(Quantity);
        UE_LOG(LogTemp, Warning, TEXT("%s has dropped Item"), *PlayerCharacter->GetName());
    }
}

void AWorldItemActor::BeginFocus()
{
}

void AWorldItemActor::EndFocus()
{
}

void AWorldItemActor::BeginInteract()
{
}

void AWorldItemActor::EndInteract()
{
}

void AWorldItemActor::Interact(ASPTPlayerCharacter* PlayerCharacter)
{
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
