// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "ItemBase.generated.h"

class ASPTPlayerCharacter;
class USphereComponent;
class UWidgetComponent;

// 아이템 상태 Enum
UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_World		UMETA(DisplayName = "World"),    // 아이템이 월드에 떨어진 상태
	EIS_Inventory   UMETA(DisplayName = "Inventory"),   // 아이템이 인벤토리에 있지만 장착되지 않은 상태
	EIS_QuickSlot	UMETA(DisplayName = "QuickSlot"),	// 퀵슬롯에 장착된 상태
	EIS_Equipped	UMETA(DisplayName = "Equipped"),  // 아이템이 손에 장착된 상태
	EIS_MAX			UMETA(Hidden)
};

UCLASS()
class SPT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	/* 기본 생성자 */
	AItemBase();

	virtual void BeginPlay() override;

	/* 아이템 초기화 (데이터 테이블에서 로드) */
	void InitializeItem(FName ItemRowName);

	/* 마우스 좌클릭 시 실행할 기본 동작 */
	virtual void PrimaryAction(class ASPTPlayerCharacter* PlayerCharacter) PURE_VIRTUAL(AItemBase::PrimaryAction, );

	/* 상태에 따라 메시 업데이트 */
	void UpdateMeshForState(EItemState NewState);

	/* 아이템 복제 */
	AItemBase* CreateItemCopy() const;

	/* Getter & Setter */
	EItemState GetItemState() const;
	void SetItemState(EItemState NewItemState);
	UItemDataObject* GetItemData() const;
	void SetItemData(UItemDataObject* NewItemData);




	// 위젯 추가
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> PickupWidgetClass;

	UPROPERTY()
	UUserWidget* PickupWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	UWidgetComponent* PickupWidgetComponent;

	// 콜리전 영역에 캐릭터 접근 시
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void ShowPickupPrompt(bool bShow);



protected:

	/* 아이템 상태 (월드, 인벤토리, 장착) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EItemState ItemState;

	/* 아이템 데이터 (동적 상태 포함) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UItemDataObject* ItemData;

	/* 아이템 메시 (월드에서 시각적 표현) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	UStaticMeshComponent* StaticMeshComponent;


	// 임의로 추가한 콜리전 영역
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Mesh")
	USphereComponent* Collision;

};
