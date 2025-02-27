#pragma once

#include "CoreMinimal.h"
#include "SPTEnemyCharacter.h"
#include "KnightEnemy.generated.h"

UCLASS()
class SPT_API AKnightEnemy : public ASPTEnemyCharacter
{
	GENERATED_BODY()

public:
	AKnightEnemy();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Attack() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* AttackMontage;

	void EquippedWeapon() override;
	void UnEquippedWeapon() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* UnEquipMontage;

};
