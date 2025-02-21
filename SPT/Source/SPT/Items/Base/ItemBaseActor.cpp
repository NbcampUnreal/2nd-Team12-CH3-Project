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
}

// ������ ����
AItemBaseActor* AItemBaseActor::CreateItemCopy() const
{
	AItemBaseActor* NewItem = NewObject<AItemBaseActor>();
	if (NewItem)
	{
		NewItem->SetItemData(ItemData);
		NewItem->SetQuantity(Quantity);
	}

	return NewItem;
}

// ���� �������� �� ���� ��ȯ
float AItemBaseActor::GetItemStackWeight() const
{
	return ItemData.NumericData.Weight * Quantity;
}

// ������ ���� ���� ��ȯ
float AItemBaseActor::GetItemSingleWeight() const
{
	return ItemData.NumericData.Weight;
}

// �ִ� ���� �������� Ȯ��
FORCEINLINE bool AItemBaseActor::IsFullItemStack() const
{
	return Quantity >= ItemData.NumericData.MaxStackSize;
}

// ������ ���� ����
void AItemBaseActor::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, ItemData.NumericData.bIsStackable ? ItemData.NumericData.MaxStackSize : 1);

		/*
		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveItem(this);
			}
		}
		*/
	}
}

// ������ ���
void AItemBaseActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Item Used: %s"), *ItemData.TextData.Name.ToString());
}

// �� ������ �����ε� (������ ID ��)
bool AItemBaseActor::operator==(const FName& OtherID) const
{
	return this->ItemData.ItemID == OtherID;
}

UStaticMeshComponent* AItemBaseActor::GetMeshComponent() const
{
	return StaticMeshComponent;
}

FItemData AItemBaseActor::GetItemData() const
{
	return ItemData;
}

void AItemBaseActor::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
	Quantity = 1;
}

bool AItemBaseActor::IsWeapon() const
{
	// return Cast<AWeapon>(this) != nullptr;
	// return ItemData.bIsWeapon;
	return false;
}
