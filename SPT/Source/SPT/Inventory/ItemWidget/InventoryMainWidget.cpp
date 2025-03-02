// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMainWidget.h"
#include "InventorySlotWidget.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "SPT/Inventory/ItemWidget/PreviewCharacter.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EngineUtils.h"                    // TActorIterator �� ����ϱ� ���� �ʿ�
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

void UInventoryMainWidget::UpdateInventoryList(const TArray<UInventoryItem*>& Items)
{
	if (!InventoryListView) {
		UE_LOG(LogTemp, Error, TEXT("InventoryListView is NULL!"));
		return;
	}

	InventoryListView->SetListItems(Items); 
}

void UInventoryMainWidget::FindPreviewCharacter()
{
    if (GetWorld() == nullptr) return;

    for (TActorIterator<APreviewCharacter> It(GetWorld()); It; ++It)
    {
        PreviewCharacter = *It;
        break; // ù ��° ã�� ������ ĳ���͸� ����
    }

    if (PreviewCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("������ ĳ���͸� ã�ҽ��ϴ�: %s"), *PreviewCharacter->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("������ ĳ���͸� ã�� �� �����ϴ�."));
    }
}

FReply UInventoryMainWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && PreviewCharacter)
    {
        FVector2D MouseDelta = InMouseEvent.GetCursorDelta(); // ���콺 �̵� �Ÿ�
        float DeltaYaw = MouseDelta.X * 0.5f; // ȸ�� ���� ����
        PreviewCharacter->RotatePreview(DeltaYaw);
    }

    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UInventoryMainWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown call!"));
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = !bIsDragging; // ���콺 ��ư�� �ö� �� �Է� ������ �߻��Ͽ� ������ �� ��ȯ�ϵ��� ����
        UE_LOG(LogTemp, Warning, TEXT("bIsDragging true!"));
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

// ���콺 �巡�� �� �巡�׸� ��ĥ �� ȣ��Ǿ�������� �� �� ���� ������ ȣ���� ������
//FReply UInventoryMainWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//    UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonUp call!"));
//    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
//    {
//        bIsDragging = false;
//        UE_LOG(LogTemp, Warning, TEXT("bIsDragging false!"));
//    }
//
//    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
//}

void UInventoryMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
    FindPreviewCharacter();
}
