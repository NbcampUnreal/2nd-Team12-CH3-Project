// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
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
// 데미지 처리
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

public:
	// 체력 수정 함수
	void SetHealth(float NewHP);

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnDeathDelegate OnDeathDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnHealthChangedDelegate OnHealthChangedDelegate;

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// 애니메이션 몽타주

private:
	UFUNCTION()
	void PlayDeathAnim();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DeathAnim;

///////////////////////////////////////////////////////////////////////
protected:
	// 기본 최대 체력
	float MaxHealth;
	// 현재 체력
	float Health;

	// 기본 걷기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float NormalSpeed;
	// 기본 걷기 속도 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float SprintSpeedMultiplier;
	// 실제 달리기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float SprintSpeed;
};
