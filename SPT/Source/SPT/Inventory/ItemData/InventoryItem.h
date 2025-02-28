// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/ItemActor.h"
#include "SPT/Items/Base/ItemBase.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"



UCLASS()
class SPT_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int32 ItemPrice;

	// 아이템을 생성할 때 필요(DropItem을 사용할 때)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TSubclassOf<AItemBase> ItemActorClass;


	FString GetItemName() const { return ItemName; }
	void UseItem();
};
