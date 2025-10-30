#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ranged_MonsterController.generated.h"

class UCapsuleComponent;

UCLASS()
class TEAM15CH3PROJECT_API ARanged_MonsterController : public AAIController
{
	GENERATED_BODY()

public:
	ARanged_MonsterController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// �ٰŸ� ��Ʈ�ѷ��� �Ȱ��� �̸� ����
	void StartChaseLoop();
	void UpdateChaseLoop();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UCapsuleComponent* CapsuleComp;

private:
	UPROPERTY()
	APawn* PlayerPawnCached = nullptr;

	FTimerHandle ChaseTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float MoveUpdateInterval = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinKeepDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float AcceptanceRadius = 200.f;
};
