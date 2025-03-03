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
    if (!PreviewCharacterClass)
    {
        UE_LOG(LogTemp, Error, TEXT("PreviewCharacterClass is not set!"));
        return;
    }

    if (!PreviewCharacter)
    {
        FActorSpawnParameters SpawnParams;
        FVector SpawnLocation = FVector(0.0f, 11184650.0f, 0.0f);
        FRotator SpawnRotation = FRotator(0.0f, 90.0f, 0.0f);
        PreviewCharacter = GetWorld()->SpawnActor<APreviewCharacter>(PreviewCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
    else
    {;
        FRotator SpawnRotation = FRotator(0.0f, 90.0f, 0.0f);
        PreviewCharacter->SetActorRotation(SpawnRotation);
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
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = !bIsDragging; // 마우스 버튼이 올라갈 때 입력 누락이 발생하여 내려갈 때 전환하도록 변경
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
}
