// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPT/Items/Data/ItemDataStructs.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "ItemBaseActor.generated.h"

class ASPTPlayerCharacter;


/* 아이템 상태 Enum*/
UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_World		UMETA(DisplayName = "World"),    // 아이템이 월드에 떨어진 상태
	EIS_Inventory   UMETA(DisplayName = "Inventory"),   // 아이템이 인벤토리에 있지만 장착되지 않은 상태
	EIS_Equipped	UMETA(DisplayName = "Equipped"),  // 아이템이 장착된 상태
	EIS_QuickSlot	UMETA(DisplayName = "QuickSlot"),
	EIS_MAX			UMETA(Hidden)
};

UCLASS()
class SPT_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/* 기본 생성자 */
	AItemBaseActor();

	/* 아이템 데이터 설정 */
	void SetItemData(const FItemData& NewItemData);

	/* 상태에 따라 메쉬를 갱신 */
	void UpdateMeshForState(EItemState NewState);

	/* 아이템 상태 업데이트 */
	void UpdateItemState(EItemState NewState);

	/* 아이템 복사 기능 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	AItemBaseActor* CreateItemCopy() const;

	/* 아이템 사용 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* Getter & Setter */
	UStaticMeshComponent* GetMeshComponent() const;
	FItemData GetItemData() const;
	bool IsWeapon() const;

	// 아이템 상태 (월드, 인벤토리, 장착)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EItemState ItemState;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ShowOnlyInnerProperties))
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

};
