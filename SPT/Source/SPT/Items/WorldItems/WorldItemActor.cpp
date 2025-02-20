// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemActor.h"
#include "SPTPlayerCharacter.h"
// #include "InventoryComponent.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWorldItemActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return;

    // UE_LOG(LogTemp, Warning, TEXT("%s has picked up %s"), *PlayerCharacter->GetName(), *ItemData.ItemName.ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s has picked up item"), *PlayerCharacter->GetName());
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
