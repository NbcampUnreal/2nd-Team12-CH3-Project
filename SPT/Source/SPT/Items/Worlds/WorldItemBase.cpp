// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItemBase.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Weapons/WeaponBase.h"

AWorldItemBase::AWorldItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemState = EItemState::EIS_World;
}

void AWorldItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldItemBase::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// �÷��̾��� �κ��丮�� �߰� (�κ��丮 �ý����� �����Ǿ� �־�� ��)
	UE_LOG(LogTemp, Log, TEXT("%s picked up by player"), *GetName());

	// �������� ���� (�Ǵ� ���� ó��)
	Destroy();
}

void AWorldItemBase::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldItemBase::OnDrop - Invalid data!"));
		return;
	}

	FVector DropLocation = GetActorLocation();
	FRotator DropRotation = GetActorRotation();;

	// ���ο� AWorldItemBase ����
	AWorldItemBase* DroppedItem = GetWorld()->SpawnActor<AWorldItemBase>(AWorldItemBase::StaticClass(), DropLocation, DropRotation);
	if (!DroppedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn AWorldItemBase!"));
		return;
	}

	// ���� ������ ������ ����
	DroppedItem->SetItemData(GetItemData());

	// ���� ����
	UStaticMeshComponent* ItemMesh = DroppedItem->FindComponentByClass<UStaticMeshComponent>();
	if (ItemMesh)
	{
		ItemMesh->SetSimulatePhysics(true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh->AddImpulse(FVector(FMath::RandRange(-100.f, 100.f), FMath::RandRange(-100.f, 100.f), 200.f), NAME_None, true);
	}

	/* �÷��̾� �κ��丮���� ���� (�ʿ��� ���) */
	// PlayerCharacter->RemoveItemFromInventory(this);

	// ���� ��ü ����
	Destroy();
	UE_LOG(LogTemp, Log, TEXT("%s dropped successfully"), *GetName());
}

void AWorldItemBase::BeginFocus()
{
	UE_LOG(LogTemp, Log, TEXT("WorldItemBase BeginFocus : % s"), *GetName());
}

void AWorldItemBase::EndFocus()
{
	UE_LOG(LogTemp, Log, TEXT("WorldItemBase EndFocus: %s"), *GetName());
}

void AWorldItemBase::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

void AWorldItemBase::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

void AWorldItemBase::Interact(ASPTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor."));
		OnPickup(PlayerCharacter);
	}
}

void AWorldItemBase::UpdateMesh()
{
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateMesh: ItemData is null for %s"), *GetName());
		return;
	}

	// �޽� ������Ʈ ����
	const FItemData& FoundItemData = ItemData->GetItemData();

	// ����ƽ �޽� ������Ʈ
	if (FoundItemData.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(FoundItemData.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
	}
}
