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
	
	// 아이템 데이터를 DataTable에서 로드
	InitializeItemFromDataTable();
}

void AWorldItemBase::InitializeItemFromDataTable()
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("WorldItemBase::ItemDataTable is not assigned!"));
		return;
	}

	if (ItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("WorldItemBase::No Vaild ItemID!"));
		return;
	}

	const FString ContextStromg = TEXT("ItemDataTable");
	FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextStromg);
	if (FoundItemData)
	{
		if (!ItemData)
		{
			UE_LOG(LogTemp, Error, TEXT("WorldItemBase::InitializeItemFromDataTable() - ItemData is null!"));
			return;
		}
		ItemData->SetItemData(*FoundItemData);
		UE_LOG(LogTemp, Log, TEXT("World item initialized: %s"), *ItemID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable does not contain data for ItemID: %s"), *ItemID.ToString());
	}
}

void AWorldItemBase::InitializeItem(const FItemData& NewItemData)
{
	if (ItemData)
	{
		ItemData->SetItemData(NewItemData);
	}
}

void AWorldItemBase::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 플레이어의 인벤토리에 추가 (인벤토리 시스템이 구현되어 있어야 함)
	UE_LOG(LogTemp, Log, TEXT("%s picked up by player"), *GetName());

	// 아이템을 제거 (또는 숨김 처리)
	Destroy();
}

void AWorldItemBase::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 드롭할 위치 계산
	FVector DropLocation = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * 100.0f;
	FRotator DropRotation = PlayerCharacter->GetActorRotation();

	// 새로운 드롭 아이템 생성
	AWorldItemBase* DroppedItem = GetWorld()->SpawnActor<AWorldItemBase>(GetClass(), DropLocation, DropRotation);
	if (!DroppedItem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn dropped item!"));
		return;
	}

	// 원본 아이템의 데이터를 새로운 아이템에 복사
	if (ItemData)
	{
		DroppedItem->InitializeItem(ItemData->GetItemData()); // 기존 아이템 데이터 유지
	}

	/* 플레이어 인벤토리에서 제거 (필요의 경우) */
	// PlayerCharacter->RemoveItemFromInventory(this);

	// 원본 객체 제거
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

void AWorldItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 변경된 프로퍼티가 존재하는지 확인
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

	// ItemID가 변경된 경우에만 데이터 테이블을 업데이트
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldItemBase, ItemID))
	{
		InitializeItemFromDataTable();
	}
}

void AWorldItemBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!ItemDataTable || ItemID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnConstruction: ItemDataTable is missing or ItemID is not set for %s"), *GetName());
		return;
	}

	const FString ContextString = TEXT("ItemDataTable Lookup");
	FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
	if (FoundItemData)
	{
		ItemData->SetItemData(*FoundItemData);
		UE_LOG(LogTemp, Log, TEXT("OnConstruction: Item data loaded for %s"), *GetName());
	}
}
