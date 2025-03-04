// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

class UListView;
class UInventoryItem;

UCLASS()
class SPT_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateInventoryList(const TArray<UInventoryItem*>& Items);


	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterPreviewImage;

	UMaterialInstanceDynamic* DynamicMaterial;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UListView* InventoryListView;
};
