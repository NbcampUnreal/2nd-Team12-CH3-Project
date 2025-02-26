#pragma once

#include "CoreMinimal.h"
#include "SPTEnemyCharacter.h"
#include "BanditEnemy.generated.h"

class UAnimMontage;

UCLASS()
class SPT_API ABanditEnemy : public ASPTEnemyCharacter
{
	GENERATED_BODY()

public:

	ABanditEnemy();

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
