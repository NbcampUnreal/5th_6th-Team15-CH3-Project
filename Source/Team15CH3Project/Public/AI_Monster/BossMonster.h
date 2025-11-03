#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossMonster.generated.h"


UCLASS()
class TEAM15CH3PROJECT_API ABossMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABossMonster();

	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	void BasicAttack(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
	void UseSkill();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	void HandleDeath();   // 충돌 해제, 컨트롤러 분리, 골드 드랍, 제거 타이머
	void DropGold();

	// ---- 스탯 ----
	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MaxHP = 1000.f;

	UPROPERTY(VisibleAnywhere, Category = "Boss|Stat")
	float CurrentHP = 0.f;  // BeginPlay에서 MaxHP로 채움(미설정 시)

	// ---- 기본 공격 ----
	UPROPERTY(EditAnywhere, Category = "Boss|Combat")
	float AttackDamage = 100.f;

	UPROPERTY(EditAnywhere, Category = "Boss|Combat")
	float AttackRange = 200.f;

	// ---- 스킬(광역) ----
	UPROPERTY(EditAnywhere, Category = "Boss|Skill")
	float SkillDamage = 200.f;

	UPROPERTY(EditAnywhere, Category = "Boss|Skill")
	float SkillRadius = 500.f;

	// ---- 골드 드랍(고정 개수) ----
	UPROPERTY(EditAnywhere, Category = "Boss|Drop")
	int32 GoldAmount = 50;  // 이 개수만큼 드랍

	UPROPERTY(EditAnywhere, Category = "Boss|Drop")
	TSubclassOf<AActor> GoldPickupClass; // 미지정 시 로그만

private:
	bool IsTargetInRange(AActor* Target, float Range) const;
};
