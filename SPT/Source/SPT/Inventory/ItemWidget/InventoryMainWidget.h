// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

class UListView;
class UInventoryItem;
class APreviewCharacter;
class UEquipmentPanelWidget;
class AEquipmentSlotInventory;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryFilterChanged);

UCLASS()
class SPT_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	bool bIsDragging = false;


public:
	// �κ��丮 ������ ����� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateInventoryList(const TArray<UInventoryItem*>& Items);
	// ��񽽷��� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateEquipmentSlots(AEquipmentSlotInventory* EquipmentSlotInventory);

	// �κ��丮 ���� Ŭ�� �� ���Ǵ� �Լ�
	UFUNCTION()
	void OnAllFilterClicked();
	UFUNCTION()
	void OnEquipmentFilterClicked();
	UFUNCTION()
	void OnConsumableFilterClicked();

	// ĳ���� �������̹����� �Ҵ�
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;

	// ��� ����
	UPROPERTY(meta = (BindWidget))
	UEquipmentPanelWidget* EquipmentPanel;

	// �κ��丮���� ǥ���� �׸��� ���ϴ� ���� ��ư
	UPROPERTY(meta = (BindWidget))
	UButton* AllFilterButton;
	UPROPERTY(meta = (BindWidget))
	UButton* EquipmentFilterButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ConsumableFilterButton;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool IsEquipmentFilter = true;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool IsConsumableFilter = true;

	// ���� ���°� �ٲ���� �� ȣ���� ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryFilterChanged OnInventoryFilterChanged;


	// ������ ĳ���Ϳ��� ��� �ݿ���Ű�� ���� �뵵�� ���� (����� �̱���)
	UMaterialInstanceDynamic* DynamicMaterial;
	


	// ������ ĳ���� �Ҵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Preview")
	TSubclassOf<APreviewCharacter> PreviewCharacterClass;
	APreviewCharacter* PreviewCharacter;



	// ĳ���͸� ȸ����ų ��
	FVector2D LastMousePosition;
	// ������ ĳ���Ͱ� ���� �� �����ϰ� ���� �ÿ��� ȸ�� ���� �ʱ�ȭ
	void FindPreviewCharacter();
	// ������ ĳ���͸� ���콺 ���������� ȸ����Ű�� �Լ�
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// ���콺 Ŭ�� �� ������ ĳ���͸� ȸ�� ���� ��ȯ�ϴ� �Լ�
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	APreviewCharacter* GetPreviewCharacter();

	// ���콺 �巡�װ� ���� �ÿ��� ȣ���� �����ʴ� ������ �߻��Ͽ� �ּ�ó��
	//FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

protected:
	virtual void NativeConstruct() override;

	// ����Ʈ �並 �κ��丮 �������� ����ϱ� ���� �뵵
	UPROPERTY(meta = (BindWidget))
	UListView* InventoryListView;
};
