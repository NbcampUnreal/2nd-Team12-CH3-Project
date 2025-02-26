// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InventorySlotWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UInventoryItem;

UCLASS()
class SPT_API UInventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    class UImage* ItemIconImage;

    UPROPERTY(meta = (BindWidget))
    UButton* ItemSelectButton;

    UPROPERTY(meta = (BindWidget))
    UButton* UseButton;

    UPROPERTY(meta = (BindWidget))
    UButton* DropButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CancelButton;


    // ������ ������ Ȯ�ο�(��������)
    UPROPERTY()
    UInventoryItem* ItemData;

    
    UFUNCTION()
    void UpdateItem(UInventoryItem* InventoryItem);

    // ������ Ŭ�� �� ���� �޴� ��ư ����
    UFUNCTION()
    void OnItemSelectClicked();
    UFUNCTION()
    void OnUseClicked();

    UFUNCTION()
    void OnDropClicked();

    UFUNCTION()
    void OnCancelClicked();
};
