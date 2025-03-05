// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWeapon.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Weapons/WeaponBase.h"
#include "SPT/Items/Weapons/FirearmWeapon.h"
// #include "SPT/Items/Weapons/MeleeWeapon.h"
// #include "SPT/Items/Weapons/ThrowableWeapon.h"
/////////////////////////////////////////////
#include "SPT/Inventory/ItemData/InventoryItem.h"
/////////////////////////////////////////////
#include "Engine/World.h"

AWorldWeapon::AWorldWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// Skeletal Mesh 추가
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// 피직스 애셋 설정 (물리 시뮬레이션 활성화)
	if (ItemData && SkeletalMeshComponent)
	{
		if (ItemData->GetWeaponData().PhysicsAsset)
		{
			SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // 피직스 애셋 연결
		}
		SkeletalMeshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
		SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody); // 물리 바디 타입 설정
		SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn 클래스 충돌 무시
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block); // 모든 채널에서 충돌 처리
		SkeletalMeshComponent->SetEnableGravity(true); // 중력 적용
		
		UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon::SetEnableGravi  ty!"));
	}
}

//void AWorldWeapon::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
//{
//    if (!PlayerCharacter)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon::OnPickup - PlayerCharacter is null!"));
//        return;
//    }
//
//    // UInventoryItem 객체 생성 및 데이터 설정 (아이템 데이터 복제)
//    UInventoryItem* NewInventoryItem = NewObject<UInventoryItem>();
//    NewInventoryItem->SetItemData(ItemData->CreateItemCopy());
//    // 무기 아이템임을 표시하는 플래그가 있다면 설정 (예: IsWeapon()이 true를 반환하도록)
//
//    // 인벤토리 매니저를 통해 아이템 사용 (UseItem 호출)
//    if (AInventoryManager* InvManager = PlayerCharacter->GetInventory())
//    {
//        InvManager->UseItem(NewInventoryItem);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon::OnPickup - InventoryManager is null!"));
//    }
//
//    // 월드 아이템 제거
//    Destroy();
//    UE_LOG(LogTemp, Log, TEXT("Weapon picked up and processed by InventoryManager"));
//}

// 견본용으로 남겨둔 백업용 코드
void AWorldWeapon::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon::OnPickup - PlayerCharacter is null!"));
		return;
	}

	// 무기 데이터가 가져오기
	FItemData FoundItemData = ItemData->GetItemData();
	const FWeaponItemData& FoundWeaponData = ItemData->GetWeaponData();

	// 무기 유형에 따라 적절한 클래스로 스폰
	AWeaponBase* SpawnedWeapon = nullptr;
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	switch (FoundWeaponData.WeaponType)
	{
	case EWeaponType::EWT_Firearm:
		SpawnedWeapon = GetWorld()->SpawnActor<AFirearmWeapon>(AFirearmWeapon::StaticClass(), SpawnLocation, SpawnRotation);
		break;
	case EWeaponType::EWT_Melee:
		// SpawnedWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(AMeleeWeapon::StaticClass(), SpawnLocation, SpawnRotation);
		break;
	case EWeaponType::EWT_Throwable:
		// SpawnedWeapon = GetWorld()->SpawnActor<AThrowableWeapon>(AThrowableWeapon::StaticClass(), SpawnLocation, SpawnRotation);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("AWorldWeapon::OnPickup - Unknown weapon type"));
		return;
	}

	if (!SpawnedWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon!"));
		return;
	}

	// 무기 데이터 설정
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	SpawnedWeapon->SetItemData(NewItemData);
	SpawnedWeapon->SetWeaponData(NewItemData->GetWeaponData());
	SpawnedWeapon->SetActorEnableCollision(false);		// 장착 시 비 충돌 상태로 만들어 상호작용을 제거

	// 무기 장착 (각 클래스에서 장착 로직 구현)
	if (AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(SpawnedWeapon))
	{
		FirearmWeapon->BeginEquip(PlayerCharacter);
	}
	else
	{
		SpawnedWeapon->Equip(); // 총기 이외 다른 무기는 미구현
	}

	// 월드 무기 제거
	Destroy();
	UE_LOG(LogTemp, Log, TEXT("Weapon picked up and equipped"));
}

#if WITH_EDITOR
void AWorldWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldWeapon, ItemID))
	{
		if (!ItemData)
		{
			ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
		}

		ItemData->InitializeItemData(ItemID);

		UpdateMesh();
		// 피직스 애셋 설정 (물리 시뮬레이션 활성화)
		if (SkeletalMeshComponent && ItemData->GetWeaponData().PhysicsAsset)
		{
			SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // 피직스 애셋 연결
			SkeletalMeshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화
			SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
			SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody); // 물리 바디 타입 설정
			SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block); // 모든 채널에서 충돌 처리
			SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn 클래스 충돌 무시
			SkeletalMeshComponent->SetEnableGravity(true); // 중력 적용 채널에서 충돌 처리
		}

		UE_LOG(LogTemp, Log, TEXT("PostEditChangeProperty - ItemData updated for %s"), *ItemID.ToString());
	}
}

void AWorldWeapon::OnConstruction(const FTransform& Transform)
{
	if (!ItemDataTable || ItemID.IsNone())
	{
		UE_LOG(LogTemp, Log, TEXT("AWorldWeapon::OnConstruction: There is No ItemDataTable or ItemID for %s"), *GetName());
		return;
	}

	if (!ItemData)
	{
		ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
	}

	ItemData->SetItemData(ItemData->GetItemData());
	UE_LOG(LogTemp, Log, TEXT("OnConstruction: Item data loaded for %s"), *GetName());

	// 메쉬 업데이트
	UpdateMesh();
}
#endif

void AWorldWeapon::UpdateMesh()
{
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateMesh: ItemData is null!"));
		return;
	}

	// 메쉬 업데이트 로직
	const FItemData& FoundItemData = ItemData->GetItemData();
	const FWeaponItemData& FoundWeaponData = ItemData->GetWeaponData();

	// Static Mesh 업데이트
	if (StaticMeshComponent && FoundItemData.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(FoundItemData.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
	}

	// Skeletal Mesh 업데이트
	if (SkeletalMeshComponent && FoundItemData.AssetData.SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(FoundItemData.AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetVisibility(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent is NULL or no mesh found for %s"), *GetName());
	}

	UE_LOG(LogTemp, Log, TEXT("UpdateMesh: Mesh updated!"));
}
