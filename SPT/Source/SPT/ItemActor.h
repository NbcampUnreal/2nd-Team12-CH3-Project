// Fill out your copyright notice in the Description page of Project Settings.

//임시용 아이템 액터 입니다. 정식으로 아이템이 추가될 시 바로 폐기하기

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"	// 아이템 위에 픽업 문구 넣기 용도
#include "ItemActor.generated.h"

class USphereComponent;
class UInventoryItem;

UCLASS()
class SPT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemActor();

	// 아이템 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;


	// 아이템 데이터 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;			// 아이템의 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDescription;	// 아이템에 대한 설명(미구현)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;				// 아이템의 아이디 (아직 미사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemAmount;			// 아이템의 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;		// 아이템의 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UInventoryItem* ItemData;	// 아이템 데이터


	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetItemName();
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetItemAmount();
	UFUNCTION(BlueprintCallable, Category = "Item")
	UInventoryItem* GetItemData() const;

	//void InitializeItem(UInventoryItem* ItemData);


	// 아이템 상호작용
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItem();				// 미구현


	// 아이템 위젯(Press F Key)
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> PickupWidgetClass;

	UPROPERTY()
	UUserWidget* PickupWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	UWidgetComponent* PickupWidgetComponent;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void ShowPickupPrompt(bool bShow);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
