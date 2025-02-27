// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldConsumable.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Consumables/ConsumableItem.h"
#include "Engine/World.h"

AWorldConsumable::AWorldConsumable()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AWorldConsumable::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetSimulatePhysics(true);  // ���� �ùķ��̼� Ȱ��ȭ
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // �浹 ���� �����ϰ� ����
		StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // �÷��̾�� �浹 ����
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		StaticMeshComponent->SetEnableGravity(true);   // �߷� Ȱ��ȭ
	}
}

void AWorldConsumable::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldConsumable::OnPickup - PlayerCharacter is null!"));
		return;
	}
	
	// �Һ� ������ ������ ��ü ����
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	if (NewItemData)
	{
		// �÷��̾� �κ��丮�� �߰� (���� �ʿ�)
		// PlayerCharacter->AddToInventory(NewItemData);
		UE_LOG(LogTemp, Log, TEXT("Consumable picked up and added to inventory: %s"), *NewItemData->GetItemData().TextData.ItemName.ToString());
	}

	// ���忡�� ����
	Destroy();
}

void AWorldConsumable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// ����� ������Ƽ�� �����ϴ��� Ȯ��
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

	// ItemID�� ����� ��쿡�� ������ ���̺��� ������Ʈ
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldConsumable, ItemID))
	{
		if (!ItemData)
		{
			ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
		}

		ItemData->InitializeItemData(ItemID);
		UpdateMesh();
		UE_LOG(LogTemp, Log, TEXT("PostEditChangeProperty - ItemData updated for %s"), *ItemID.ToString());
	}
}

void AWorldConsumable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!ItemDataTable || ItemID.IsNone())
	{
		UE_LOG(LogTemp, Log, TEXT("AWorldConsumable::OnConstruction: There is No ItemDataTable or ItemID for %s"), *GetName());
		return;
	}

	if (!ItemData)
	{
		ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
	}

	ItemData->SetItemData(ItemData->GetItemData());
	UE_LOG(LogTemp, Log, TEXT("OnConstruction: Item data loaded for %s"), *GetName());

	UpdateMesh();
}
