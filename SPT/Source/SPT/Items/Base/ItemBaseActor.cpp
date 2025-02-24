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
	ItemState = EItemState::EIS_World; // 기본적으로 월드에 존재
}

// 아이템 데이터 설정
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
		// 장착 상태 : 무기라면 Skeletal Mesh 우선 사용
		if (ItemData.ItemType == EItemType::EIT_Weapon && ItemData.AssetData.SkeletalMesh)
		{
			SkeletalMeshComponent->SetSkeletalMesh(ItemData.AssetData.SkeletalMesh);
			SkeletalMeshComponent->SetVisibility(true);
			StaticMeshComponent->SetVisibility(false);
		}
		else
		{
			// 무기가 아니거나 SkeletalMesh가 없는 경우는 Static Mesh 사용
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
		// 월드 혹은 인벤토리 상태 : 기본적으로 Static Mesh 사용
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
	// 상태에 따라 메쉬를 업데이트
	UpdateMeshForState(NewState);
}

// 아이템 복제
AItemBaseActor* AItemBaseActor::CreateItemCopy() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateItemCopy failed: World is null."));
		return nullptr;
	}

	// 현재 액터의 transform을 그대로 사용하여 복제
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

// 아이템 사용
void AItemBaseActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemBaseActor:: Item Used: %s"), *ItemData.TextData.Name.ToString());


	// TO DO: 아이템 사용 로직 구현
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
