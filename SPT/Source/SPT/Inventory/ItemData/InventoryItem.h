// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/ItemActor.h"
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

	// 액터를 저장하고 불러오는 용도
	UPROPERTY()
	AItemActor* ItemActor;  // 아이템 액터를 저장

	void SetItemActor(AItemActor* NewActor) { ItemActor = NewActor; }
	AItemActor* GetItemActor() const { return ItemActor; }

};
