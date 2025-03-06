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
	// 인벤토리 내부의 목록을 업데이트
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateInventoryList(const TArray<UInventoryItem*>& Items);
	// 장비슬롯을 업데이트
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateEquipmentSlots(AEquipmentSlotInventory* EquipmentSlotInventory);

	// 인벤토리 필터 클리 시 상용되는 함수
	UFUNCTION()
	void OnAllFilterClicked();
	UFUNCTION()
	void OnEquipmentFilterClicked();
	UFUNCTION()
	void OnConsumableFilterClicked();

	// 캐릭터 프리뷰이미지를 할당
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;

	// 장비 슬롯
	UPROPERTY(meta = (BindWidget))
	UEquipmentPanelWidget* EquipmentPanel;

	// 인벤토리에서 표시할 항목을 정하는 필터 버튼
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

	// 필터 상태가 바뀌었을 때 호출할 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryFilterChanged OnInventoryFilterChanged;


	// 프리뷰 캐릭터에게 장비를 반영시키기 위한 용도의 변수 (기능은 미구현)
	UMaterialInstanceDynamic* DynamicMaterial;
	


	// 프리뷰 캐릭터 할당
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Preview")
	TSubclassOf<APreviewCharacter> PreviewCharacterClass;
	APreviewCharacter* PreviewCharacter;



	// 캐릭터를 회전시킬 때
	FVector2D LastMousePosition;
	// 프리뷰 캐릭터가 없을 시 생성하고 있을 시에는 회전 값을 초기화
	void FindPreviewCharacter();
	// 프리뷰 캐릭터를 마우스 움직임으로 회전시키는 함수
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// 마우스 클릭 시 프리뷰 캐릭터를 회전 모드로 전환하는 함수
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	APreviewCharacter* GetPreviewCharacter();

	// 마우스 드래그가 끝날 시에도 호출이 되지않는 문제가 발생하여 주석처리
	//FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

protected:
	virtual void NativeConstruct() override;

	// 리스트 뷰를 인벤토리 위젯에서 사용하기 위한 용도
	UPROPERTY(meta = (BindWidget))
	UListView* InventoryListView;
};
