// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "WeaponDataStructs.h"
#include "ItemDataObject.generated.h"

UCLASS()
class SPT_API UItemDataObject : public UObject
{
	GENERATED_BODY()

public:
	/* 기본 생성자 */
	UItemDataObject();

	/* 데이터 테이블에서 아이템 초기화 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializeFromDataTable(FName RowName);

	/* 아이템 복사 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* CreateItemCopy() const;

	/* 아이템 데이터 가져오기 / 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetItemData() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemData(const FItemData& NewItemData);

	/* 무기 데이터 가져오기 / 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	const FWeaponItemData& GetWeaponData() const;
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	void SetWeaponData(const FWeaponItemData& NewWeaponData);
	UFUNCTION(BlueprintCallable, Category = "Item|Weapon")
	bool IsWeapon() const;

	/* 아이템 개수 가져오기 / 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* 아이템이 스택 가능한지 확인 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanStackWith(const UItemDataObject* OtherItem) const;

	/* 아이템 병합 및 분할 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 MergeStack(UItemDataObject* OtherItem);
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* SplitStack(int32 SplitQuantity);

protected:
	/* 아이템 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;

	/* 아이템 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* ItemDataTable;

	/* 무기 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FWeaponItemData WeaponData;

	/* 무기 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Data")
	UDataTable* WeaponDataTable;

	/* 현재 아이템 개수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	/* 아이템 고유 ID 비교 연산자 */
	bool operator==(const FName& OtherID) const;

};
