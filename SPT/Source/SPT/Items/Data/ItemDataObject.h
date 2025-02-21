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

	/* 무게 관련 함수 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemStackWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsFullItemStack() const;

	/* 아이템 사용 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	/* 아이템 데이터 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FItemData GetItemData() const;

	/* 아이템 데이터 설정 */
	void SetItemData(const FItemData& NewItemData);

	/* 아이템 개수 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;

	/* 아이템 개수 설정 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* 아이템이 무기인지 확인 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsWeapon() const;

	/* 아이템이 소모품인지 확인 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsConsumable() const;

	/* 아이템이 인벤토리에 속해 있는지 확인 */
	// UFUNCTION(BlueprintCallable, Category = "Item")
	// bool IsInInventory() const;

protected:
	/* 아이템 고유 ID 비교 연산자 */
	bool operator==(const FName& OtherID) const;

	/* 아이템 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Data")
	FItemData ItemData;

	/* 아이템이 속한 인벤토리 */
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;

	/* 현재 아이템 개수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

};
