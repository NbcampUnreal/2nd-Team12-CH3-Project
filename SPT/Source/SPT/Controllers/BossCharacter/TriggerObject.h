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
    float Health;
    float MaxHealth;

    void SetHealth(float NewHealth);


    // �⺻ ������
    ATriggerObject();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    // �޽� ������Ʈ (�������� ���¸� ��Ÿ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // �浹 ������ ���� ������Ʈ
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    // OnHit �̺�Ʈ (BossSpawnerTrigger���� ��ȣ�� ����)
    UPROPERTY(BlueprintAssignable)
    FOnHitDelegate OnHitTrigger;

    
protected:
    // ���� ���� �� ȣ��Ǵ� �Լ�
    virtual void BeginPlay() override;

private:
    // �浹 �� Ʈ���Ÿ� �߻���Ű�� �Լ�
    void TriggerHit();
};