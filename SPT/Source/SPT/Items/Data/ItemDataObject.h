// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "WeaponDataStructs.h"
#include "ConsumableDataStructs.h"
#include "ItemDataObject.generated.h"

class ASPTPlayerCharacter;

UCLASS()
class SPT_API UItemDataObject : public UObject
{
	GENERATED_BODY()

public:
	/* 기본 생성자 */
	UItemDataObject();

	/* 아이템 데이터 초기화 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializeItemData(FName RowName);

	/* 아이템 복사 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* CreateItemCopy() const;

	/* 아이템 데이터 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	const FItemData& GetItemData() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemData(const FItemData& NewItemData);

	/* 무기 데이터 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	const FWeaponItemData& GetWeaponData() const;
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	void SetWeaponData(const FWeaponItemData& NewWeaponData);

	/* 소비 아이템 데이터 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "Item|Caonsumables")
	const FConsumableItemData& GetConsumableata() const;
	UFUNCTION(BlueprintCallable, Category = "Item|Caonsumables")
	void SetConsumableData(const FConsumableItemData& NewConsumableData);

	/* 아이템 사용 (우선 소비 아이템) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* 아이템 개수 가져오기 및 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* 아이템 타입 확인 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsConsumable() const;

	/* 아이템 병합 및 분할 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanStackWith(const UItemDataObject* OtherItem) const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 MergeStack(UItemDataObject* OtherItem);
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* SplitStack(int32 SplitQuantity);

protected:
	/* 아이템 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* ItemDataTable;

	/* 무기 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FWeaponItemData WeaponData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* WeaponDataTable;

	/* 소비 아이템 데이터 (필요할 때 로드됨) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Consumable")
	FConsumableItemData ConsumableData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* ConsumableDataTable;

	/* 현재 아이템 개수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	/* 무기인지 여부 캐싱 */
	bool bHasWeaponData;

	/* 소비 아이템인지 여부 캐싱 */
	bool bHasConsumableData;

};
