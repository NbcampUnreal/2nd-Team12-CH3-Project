#pragma once

#include "BaseCharacter.h"
#include "SPTEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
//DECLARE_MULTICAST_DELEGATE(FOnAttack_Temp);

class APatrolRoute;

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

	// ���� ���� �̺�Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnAttackEnd OnAttackEnd;
	//FOnAttack_Temp TryAttack;

	//void Try();

	// ������ ���ִ� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float IdleSpeed;

	// ���� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float JogSpeed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	APatrolRoute* PatrolRoute;

	//void EquipWeapon(Weapon* weapon);
	// void UnEquipWeapon();
};