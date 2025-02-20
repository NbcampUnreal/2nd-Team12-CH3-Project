// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// #include "ItemDataStructs.h"
// #include "WeaponDataStructs.h"
#include "ItemBaseActor.generated.h"

class ASPTPlayerCharacter;

UCLASS()
class SPT_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/* 기본 생성자 */
	AItemBaseActor();

	/* 아이템 복사 기능 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	AItemBaseActor* CreateItemCopy() const;

	/* 아이템 스택 관련 함수 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	float GetItemStackWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	float GetItemSingleWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE bool IsFullItemStack() const;
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	/* 아이템 사용 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);

	bool operator==(const FName& OtherID) const;

	/* Getter & Setter */
	UStaticMeshComponent* GetMeshComponent() const;
	// FItemData GetItemData() const;
	// void SetItemData(const FItemData& NewItemData);
	bool IsWeapon() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USkeletalMeshComponent* SkeletalMeshComponent;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	// FItemData ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

};
