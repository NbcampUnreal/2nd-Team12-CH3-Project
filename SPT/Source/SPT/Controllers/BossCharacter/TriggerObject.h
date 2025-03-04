// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitDelegate); // OnHit �̺�Ʈ ����

UCLASS()
class SPT_API ATriggerObject : public AActor
{
	GENERATED_BODY()
	
public:
    // �⺻ ������
    ATriggerObject();

    // �޽� ������Ʈ (�������� ���¸� ��Ÿ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // �浹 ������ ���� ������Ʈ
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    // OnHit �̺�Ʈ (BossSpawnerTrigger���� ��ȣ�� ����)
    UPROPERTY(BlueprintAssignable)
    FOnHitDelegate OnHitTrigger;

    // �浹 �� ȣ��� �Լ� (OnHit �̺�Ʈ �߻�)
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
    
protected:
    // ���� ���� �� ȣ��Ǵ� �Լ�
    virtual void BeginPlay() override;

private:
    // �浹 �� Ʈ���Ÿ� �߻���Ű�� �Լ�
    void TriggerHit();
};