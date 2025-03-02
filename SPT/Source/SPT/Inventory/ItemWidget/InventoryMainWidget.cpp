// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMainWidget.h"
#include "InventorySlotWidget.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "SPT/Inventory/ItemWidget/PreviewCharacter.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "EngineUtils.h"                    // TActorIterator 를 사용하기 위해 필요
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
        break; // 첫 번째 찾은 프리뷰 캐릭터를 저장
    }

    if (PreviewCharacter)
    {
        UE_LOG(LogTemp, Log, TEXT("프리뷰 캐릭터를 찾았습니다: %s"), *PreviewCharacter->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("프리뷰 캐릭터를 찾을 수 없습니다."));
    }
}

FReply UInventoryMainWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && PreviewCharacter)
    {
        FVector2D MouseDelta = InMouseEvent.GetCursorDelta(); // 마우스 이동 거리
        float DeltaYaw = MouseDelta.X * 0.5f; // 회전 감도 조절
        PreviewCharacter->RotatePreview(DeltaYaw);
    }

    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UInventoryMainWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("NativeOnMouseButtonDown call!"));
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = !bIsDragging; // 마우스 버튼이 올라갈 때 입력 누락이 발생하여 내려갈 때 전환하도록 변경
        UE_LOG(LogTemp, Warning, TEXT("bIsDragging true!"));
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

// 마우스 드래그 시 드래그를 마칠 때 호출되어야하지만 알 수 없는 이유로 호출이 누락됨
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
