#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
//추가
#include "Components/WidgetComponent.h"
//끝
#include "AI_Monsters.generated.h"

class UCapsuleComponent;

UCLASS()
class TEAM15CH3PROJECT_API AAI_Monsters : public ACharacter
{
	GENERATED_BODY()

public:

	AAI_Monsters();

	bool CanAttack(APawn* Target) const;
	bool ReangCanAttak(APawn* Traget) const;
	void PerformAttack(APawn* Target);
	void BulletAttack(APawn* Target);
	bool IsDead() const { return CurrentHP <= 0.f; }

	UPROPERTY(EditAnyWhere, Category = "AI")
	float WalkSpeed = 400.0f; // AI 몬스터 속도값을 정해주는 기능.

	UPROPERTY(EditAnyWhere, Category = "AI")
	float RunSpeed = 500.0f;
	//추가
	void UpdateOverheadHP();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HPUI")
	class UWidgetComponent* HealthBarComp;
	//끝

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	TSubclassOf<AMonsterBullet> Bullets;





protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float RangeAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat") float CurrentHP;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

private:

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	class USoundBase* AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
	class UParticleSystem* HitEffect;

	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	void HandleDeath();
	
	float LastAttackTime;          

	void SetMovementSpeed(float NewSpeed);
};
