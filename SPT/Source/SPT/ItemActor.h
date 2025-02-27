// Fill out your copyright notice in the Description page of Project Settings.

//�ӽÿ� ������ ���� �Դϴ�. �������� �������� �߰��� �� �ٷ� ����ϱ�

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"	// ������ ���� �Ⱦ� ���� �ֱ� �뵵
#include "ItemActor.generated.h"

class USphereComponent;
class UInventoryItem;

UCLASS()
class SPT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemActor();

	// ������ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;


	// ������ ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;			// �������� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDescription;	// �����ۿ� ���� ����(�̱���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;				// �������� ���̵� (���� �̻��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemAmount;			// �������� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;		// �������� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UInventoryItem* ItemData;	// ������ ������


	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetItemName();
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetItemAmount();
	UFUNCTION(BlueprintCallable, Category = "Item")
	UInventoryItem* GetItemData() const;

	//void InitializeItem(UInventoryItem* ItemData);


	// ������ ��ȣ�ۿ�
	UFUNCTION(BlueprintCallable, Category = "Item")
	void UseItem();				// �̱���


	// ������ ����(Press F Key)
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
