// ItemDataStructs.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
// #include "WeaponDataStructs.h"
// #include "ConsumableDataStructs.h"
// #include "AttachmentDataStructs.h"
#include "ItemDataStructs.generated.h"

class AItemBase;

/* 아이템 타입 Enum */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon		UMETA(DisplayName = "Weapon"),			// 무기
	EIT_Consumable	UMETA(DisplayName = "Consumable"),		// 소비 아이템
	EIT_Attachment	UMETA(DisplayName = "Attachment"),		// 부착물
	EIT_MAX			UMETA(Hidden)
};

/* 아이템 문자 데이터 */
USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	/* 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	/* 상호작용 시 메시지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText InteractionText;

	/* 아이템 사용 시 메시지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText UsageText;
};

/* 아이템 수치 데이터 */
USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	/* 아이템 무게 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Weight;

	/* 최대 스택 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStackSize;

	/* 스택 가능 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;
};

/* 아이템 에셋 데이터 */
USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	/* 아이템 아이콘 (인벤토리에 표시할 아이콘 - 없으면 제거) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;

	/* 월드에서 보여질 스태틱 메시 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMesh* StaticMesh;

	/* 장착 시 사용될 무기 스켈레탈 메시 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EIT_Weapon"))
	USkeletalMesh* SkeletalMesh;

	/* 아이템을 장착할 소켓 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName AttachSocketName = NAME_None;
};

/* 아이템 애니메이션 데이터 구조체 */
USTRUCT(BlueprintType)
struct FItemAnimationData
{
	GENERATED_USTRUCT_BODY()

	/* 아이템 사용 애니메이션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	UAnimMontage* UseAnimation;

	/* 애니메이션 종료 후 아이템 제거 지연 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	float AnimationDelayAfterUse;
};

/* 전체 아이템 데이터 */
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/* 아이템 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FName ItemID;

	/* 아이템 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	EItemType ItemType;

	/* 아이템 문자 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemTextData TextData;

	/* 아이템 수치 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemNumericData NumericData;

	/* 아이템 에셋 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemAssetData AssetData;

	/* 아이템 애니메이션 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Animation")
	FItemAnimationData AnimationData;

	/* 
	 * 우선 각 아이템의 데이터는 각 아이템 베이스에서 관리하는 방향으로 수정하였습니다.
	 * 한 곳에서 데이터 관리가 필요하다고 판단될 경우 아래의 주석을 해제하고 사용할 수 있습니다.
	**/

	/* 무기 데이터 (무기인 경우에만 할당) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon", meta = (EditCondition = "ItemType == EIT_Weapon"))
	// FWeaponItemData WeaponData;

	/* 소비 아이템 데이터(소비 아이템인 경우에만 할당) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Consumable", meta = (EditCondition = "ItemType == EIT_Consumable"))
	// FConsumableItemData ConsumableData;

	/* 부착 아이템 데이터(부착 아이템인 경우에만 할당) */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Attachment", meta = (EditCondition = "ItemType == EIT_Attachment"))
	// FAttachmentItemData AttachmentData;



	// 아이템을 생성할 때 필요(DropItem을 사용할 때)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	TSubclassOf<AItemBase> ItemActorClass;
};
