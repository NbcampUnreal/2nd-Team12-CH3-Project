// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "ItemDataObject.generated.h"

class ASPTPlayerCharacter;
class UInventoryComponent;

UCLASS()
class SPT_API UItemDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	/* 기본 생성자 */
	UItemDataObject();

	/* 아이템 복사 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* CreateItemCopy() const;

	/* 아이템 사용 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* 아이템 데이터 가져오기 / 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetItemData() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemData(const FItemData& NewItemData);

	/* 아이템 개수 가져오기 / 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* 아이템이 타입 확인 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsConsumable() const;

	/* 무게 관련 함수 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemStackWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsFullItemStack() const;

	/* 아이템이 인벤토리에 속해 있는지 확인 */
	// UFUNCTION(BlueprintCallable, Category = "Item")
	// bool IsInInventory() const;

	/* 스택 관련 함수 */
	/* 두 아이템이 병합 가능한지 확인 (동일 ItemID, 스택 가능) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanMergeWith(const UItemDataObject* OtherItem) const;

	/* 다른 아이템의 스택을 현재 아이템에 병합하고, 병합 후 OtherItem에 남은 수량을 반환 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 MergeStack(UItemDataObject* OtherItem);

	/* 현재 스택에서 SplitQuantity 만큼 분리하여 새로운 아이템 객체를 생성 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	UItemDataObject* SplitStack(int32 SplitQuantity);

protected:
	/* 아이템 고유 ID 비교 연산자 */
	bool operator==(const FName& OtherID) const;

	/* 아이템 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;

	/* 현재 아이템 개수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	/* 아이템이 속한 인벤토리 */
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;

};
