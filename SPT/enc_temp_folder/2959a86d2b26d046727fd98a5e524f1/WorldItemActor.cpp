// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemActor.h"
#include "SPTPlayerCharacter.h"
// #include "InventoryComponent.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // �Ⱦ� �������� �޽� ����
    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
    PickupMesh->SetSimulatePhysics(true);
    SetRootComponent(PickupMesh);
}

void AWorldItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    UE_LOG(LogTemp, Warning, TEXT("%s has picked up %s"), *PlayerCharacter->GetName(), *ItemData.TextData.Name.ToString());
    Destroy(); // ������ �ݱ� �� ����

	/* �κ��丮 ������Ʈ ���� �� ����
    UInventoryComponent* Inventory = PlayerCharacter->FindComponentByClass<UInventoryComponent>();

    if (Inventory)
    {
        Inventory->AddItem(this);  // �κ��丮�� �߰�
        Destroy();  // ���忡�� ����
    }
	*/ 
}

void AWorldItemActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    // �������� ���� �� ���忡 �ٽ� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = PlayerCharacter;

    AWorldItemActor* DroppedItem = GetWorld()->SpawnActor<AWorldItemActor>(GetClass(), PlayerCharacter->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

    if (DroppedItem)
    {
        // ������ ���� �� ���� ���� ����
        // DroppedItem->SetItemData(ItemData);
        DroppedItem->SetQuantity(Quantity);
        UE_LOG(LogTemp, Warning, TEXT("%s has dropped Item"), *PlayerCharacter->GetName());
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
