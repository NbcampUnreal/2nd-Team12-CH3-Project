#pragma once

#include "BaseCharacter.h"
#include "SPTEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
//DECLARE_MULTICAST_DELEGATE(FOnAttack_Temp);

class APatrolRoute;

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	Idle       UMETA(DisplayName = "Idle"),
	Walking    UMETA(DisplayName = "Walking"),
	Jogging    UMETA(DisplayName = "Jogging"),
	Sprinting  UMETA(DisplayName = "Sprinting")
};

UCLASS()
class SPT_API ASPTEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASPTEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack();

	// 공격 종료 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnAttackEnd OnAttackEnd;

	virtual void EquippedWeapon();
	virtual void UnEquippedWeapon();

	bool IsEquippedWeapon;

	//FOnAttack_Temp TryAttack;

	//void Try();

	UFUNCTION(BlueprintCallable)
	void SetMovementSpeed(EMovementSpeed NewSpeed);

	// 가만히 서있는 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float IdleSpeed;

	// 걷는 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float WalkSpeed;

	// 순찰 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float JogSpeed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	APatrolRoute* PatrolRoute;

};