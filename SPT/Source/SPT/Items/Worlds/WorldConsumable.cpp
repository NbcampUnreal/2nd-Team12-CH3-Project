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
		StaticMeshComponent->SetSimulatePhysics(true);  // 물리 시뮬레이션 활성화
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // 충돌 감지 가능하게 변경
		StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // 플레이어와 충돌 무시
		StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		StaticMeshComponent->SetEnableGravity(true);   // 중력 활성화
	}
}

void AWorldConsumable::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldConsumable::OnPickup - PlayerCharacter is null!"));
		return;
	}
	
	// 소비 아이템 데이터 객체 생성
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	if (NewItemData)
	{
		// 플레이어 인벤토리에 추가 (구현 필요)
		// PlayerCharacter->AddToInventory(NewItemData);
		UE_LOG(LogTemp, Log, TEXT("Consumable picked up and added to inventory: %s"), *NewItemData->GetItemData().TextData.ItemName.ToString());
	}

	// 월드에서 제거
	Destroy();
}

void AWorldConsumable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티가 존재하는지 확인
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

	// ItemID가 변경된 경우에만 데이터 테이블을 업데이트
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
