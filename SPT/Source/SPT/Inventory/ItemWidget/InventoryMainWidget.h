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
	// �κ��丮 ������ ����� ������Ʈ
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateInventoryList(const TArray<UInventoryItem*>& Items);

	// ĳ���� ������
	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterPreviewImage;

	// ������ ĳ���Ϳ��� ��� �ݿ���Ű�� ���� �뵵�� ���� (����� �̱���)
	UMaterialInstanceDynamic* DynamicMaterial;
	
	// ������ ĳ���� �Ҵ�
	APreviewCharacter* PreviewCharacter;
	// 
	FVector2D LastMousePosition;

	void FindPreviewCharacter();


	// ������ ĳ���͸� ���콺 ���������� ȸ����Ű�� �Լ�
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	// ���콺 �巡�װ� ���� �ÿ��� ȣ���� �����ʴ� ������ �߻��Ͽ� �ּ�ó��
	//FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

protected:
	virtual void NativeConstruct() override;

	// ����Ʈ �並 �κ��丮 �������� ����ϱ� ���� �뵵
	UPROPERTY(meta = (BindWidget))
	UListView* InventoryListView;
};
