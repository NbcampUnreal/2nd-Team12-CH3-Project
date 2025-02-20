// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBaseActor.h"
#include "SPTPlayerCharacter.h"

AItemBaseActor::AItemBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Skeletal Mesh Component 생성 (무기)
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	Quantity = 1;
}

// 아이템 복제
AItemBaseActor* AItemBaseActor::CreateItemCopy() const
{
	AItemBaseActor* NewItem = NewObject<AItemBaseActor>();
	if (NewItem)
	{
		// NewItem->SetItemData(ItemData);
		NewItem->SetQuantity(Quantity);
	}

	return NewItem;
}

// 현재 아이템의 총 무게 반환
float AItemBaseActor::GetItemStackWeight() const
{
	// return ItemData.Weight * Quantity;
	return Quantity;
}

// 아이템 단일 무게 반환
float AItemBaseActor::GetItemSingleWeight() const
{
	// return ItemData.Weight;
	return 0.0f;
}

// 최대 스택 개수인지 확인
FORCEINLINE bool AItemBaseActor::IsFullItemStack() const
{
	// return Quantity >= ItemData.MaxStackSize;
	return false;
}

// 아이템 개수 설정
void AItemBaseActor::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		// Quantity = FMath::Clamp(NewQuantity, 0, ItemData.NumericData.bIsStackable ? ItemData.NumericData.MaxStackSize : 1);

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

// 아이템 사용
void AItemBaseActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	// UE_LOG(LogTemp, Warning, TEXT("Item Used: %s"), *ItemData.ItemName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Item Used"));
}

// 비교 연산자 오버로딩 (아이템 ID 비교)
bool AItemBaseActor::operator==(const FName& OtherID) const
{
	// return this->ItemData.ItemID == OtherID;
	return false;
}

UStaticMeshComponent* AItemBaseActor::GetMeshComponent() const
{
	return StaticMeshComponent;
}

bool AItemBaseActor::IsWeapon() const
{
	// return Cast<AWeapon>(this) != nullptr;
	// return ItemData.bIsWeapon;
	return false;
}
