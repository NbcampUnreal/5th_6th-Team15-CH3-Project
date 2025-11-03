#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Monster/MonsterBullet.h"
#include "Drone.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API ADrone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADrone();


    virtual void Tick(float DeltaTime) override;

    void SetOwnerCharacter(ACharacter* OwnerChar) { OwnerCharacter = OwnerChar; }

    void SetOrbitTarget(AActor* Target);

    UPROPERTY(EditAnywhere, Category = "Attack") // 여기 코드 재활용
    TSubclassOf<AMonsterBullet> BulletClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMeshComponent* DroneMesh;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackRadius = 600.f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackInterval = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float OrbitRadius = 200.f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float OrbitSpeed = 50.f; 

protected:
    virtual void BeginPlay() override;

private:
    ACharacter* OwnerCharacter;
    float CurrentAngle = 0.f;
    float LastAttackTime = 0.f;
    AActor* OrbitTarget = nullptr;

    void AttackNearbyEnemies();
};
