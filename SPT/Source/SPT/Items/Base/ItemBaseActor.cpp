// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBaseActor.h"
#include "SPTPlayerCharacter.h"

AItemBaseActor::AItemBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component ����
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Skeletal Mesh Component ���� (����)
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	Quantity = 1;
	ItemState = EItemState::EIS_World; // �⺻������ ���忡 ����
}

// ������ ������ ����
void AItemBaseActor::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
	Quantity = 1;

	UpdateMeshForState(EItemState::EIS_World);
}


void AItemBaseActor::UpdateMeshForState(EItemState NewState)
{
	ItemState = NewState;
	UE_LOG(LogTemp, Log, TEXT("ItemBaseActor:: Item state updated: %d"), static_cast<int32>(ItemState));

	if (NewState == EItemState::EIS_Equipped)
	{
		// ���� ���� : ������ Skeletal Mesh �켱 ���
		if (ItemData.ItemType == EItemType::EIT_Weapon && ItemData.AssetData.SkeletalMesh)
		{
			SkeletalMeshComponent->SetSkeletalMesh(ItemData.AssetData.SkeletalMesh);
			SkeletalMeshComponent->SetVisibility(true);
			StaticMeshComponent->SetVisibility(false);
		}
		else
		{
			// ���Ⱑ �ƴϰų� SkeletalMesh�� ���� ���� Static Mesh ���
			if (ItemData.AssetData.StaticMesh)
			{
				StaticMeshComponent->SetStaticMesh(ItemData.AssetData.StaticMesh);
				StaticMeshComponent->SetVisibility(true);
				SkeletalMeshComponent->SetVisibility(false);
			}
		}
	}
	else
	{
		// ���� Ȥ�� �κ��丮 ���� : �⺻������ Static Mesh ���
		if (ItemData.AssetData.StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(ItemData.AssetData.StaticMesh);
			StaticMeshComponent->SetVisibility(true);
			SkeletalMeshComponent->SetVisibility(false);
		}
	}
}

void AItemBaseActor::UpdateItemState(EItemState NewState)
{
	ItemState = NewState;
	UE_LOG(LogTemp, Log, TEXT("ItemBaseActor:: Item state updated: %d"), static_cast<int32>(ItemState));
	// ���¿� ���� �޽��� ������Ʈ
	UpdateMeshForState(NewState);
}

// ������ ����
AItemBaseActor* AItemBaseActor::CreateItemCopy() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateItemCopy failed: World is null."));
		return nullptr;
	}

	// ���� ������ transform�� �״�� ����Ͽ� ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItemBaseActor* NewItem = World->SpawnActor<AItemBaseActor>(GetClass(), GetActorTransform(), SpawnParams);
	if (NewItem)
	{
		NewItem->SetItemData(ItemData);
		NewItem->Quantity = Quantity;
		NewItem->UpdateItemState(ItemState);
	}

	return NewItem;
}

// ������ ���
void AItemBaseActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemBaseActor:: Item Used: %s"), *ItemData.TextData.Name.ToString());


	// TO DO: ������ ��� ���� ����
}

UPrimitiveComponent* AItemBaseActor::GetMeshComponent() const
{
	return (IsWeapon() && SkeletalMeshComponent)
		? static_cast<UPrimitiveComponent*>(SkeletalMeshComponent)
		: static_cast<UPrimitiveComponent*>(StaticMeshComponent);
}

FItemData AItemBaseActor::GetItemData() const
{
	return ItemData;
}

bool AItemBaseActor::IsWeapon() const
{
	return ItemData.ItemType == EItemType::EIT_Weapon;
}
