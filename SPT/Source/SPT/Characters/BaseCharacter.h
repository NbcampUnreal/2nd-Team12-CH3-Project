// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDethMulticastDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, NewHP, float, MaxHP);

UCLASS()
class SPT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

///////////////////////////////////////////////////////////////////////
// ������ ó��
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// ��� ó�� �Լ� (ü���� 0 ���ϰ� �Ǿ��� �� ȣ��)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

public:
	// ü�� ���� �Լ�
	void SetHealth(float NewHP);

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnDethMulticastDelegate OnDethMulticastDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnHealthChangedDelegate OnHealthChangedDelegate;

///////////////////////////////////////////////////////////////////////

protected:
	// �⺻ �ִ� ü��
	float MaxHealth;
	// ���� ü��
	float Health;

	// �⺻ �ȱ� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float NormalSpeed;
	// �⺻ �ȱ� �ӵ� ��� �� ��� ������ �޸��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float SprintSpeedMultiplier;
	// ���� �޸��� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float SprintSpeed;
};
