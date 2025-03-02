// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

class UListView;
class UInventoryItem;
class APreviewCharacter;

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

	// 캐릭터 프리뷰
	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterPreviewImage;

	// 프리뷰 캐릭터에게 장비를 반영시키기 위한 용도의 변수 (기능은 미구현)
	UMaterialInstanceDynamic* DynamicMaterial;
	
	// 프리뷰 캐릭터 할당
	APreviewCharacter* PreviewCharacter;
	// 
	FVector2D LastMousePosition;

	void FindPreviewCharacter();


	// 프리뷰 캐릭터를 마우스 움직임으로 회전시키는 함수
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	// 마우스 드래그가 끝날 시에도 호출이 되지않는 문제가 발생하여 주석처리
	//FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

protected:
	virtual void NativeConstruct() override;

	// 리스트 뷰를 인벤토리 위젯에서 사용하기 위한 용도
	UPROPERTY(meta = (BindWidget))
	UListView* InventoryListView;
};
