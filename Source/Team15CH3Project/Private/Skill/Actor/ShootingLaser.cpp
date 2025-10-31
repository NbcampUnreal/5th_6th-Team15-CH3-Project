#include "Skill/Actor/ShootingLaser.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "CollisionQueryParams.h" 
#include "CollisionShape.h"

AShootingLaser::AShootingLaser()
{
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(SceneRoot);
	CollisionComp->InitSphereRadius(0.1f);
	// ?? ������ ���� (�⺻ ���� ǥ��)
	CastingMeshIn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	CastingMeshIn->SetupAttachment(CollisionComp);

	// ?? ���� Ŀ���� ���� (ĳ���� �� �ð��� ����)
	CastingMeshOut = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastingMesh"));
	CastingMeshOut->SetupAttachment(CollisionComp);

	// �⺻��
	GrowSpeed = 1.8f;
	MaxScale = 5.0f;
	BaseScale = 1.0f;
	DamageRadius = 500.f;
	Damage = 80.f;
	bIsLaserShot = false;

	CurrentScale = 0.1f;
}

void AShootingLaser::BeginPlay()
{
	Super::BeginPlay();

	// GroundMesh�� �̹� ������ ũ��� ǥ��
	CastingMeshIn->SetRelativeScale3D(FVector(MaxScale, MaxScale, 0.001f));

	// CastingMesh�� �۰� ����
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentScale, CurrentScale, 0.001f));

}

void AShootingLaser::Tick(float DeltaTime)
{
	if (bIsLaserShot) return;

	// ?? ���� Ŀ���� ���� ó��
	CurrentScale += GrowSpeed * DeltaTime;
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentScale, CurrentScale, 0.001f));

	if (bShowDamageRadius)
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			DamageRadius,
			32,
			FColor::Red,
			false,  // ���ӽð�
			0.f,    // LifeTime
			0,      // DepthPriority
			2.f     // Thickness
		);
	}

	// ?? Ŀ���� ������ GroundMesh ũ��� �������� ����
	if (CurrentScale >= MaxScale)
	{
		Shoting();
	}
}

void AShootingLaser::Shoting()
{
	bIsLaserShot = true;

	if (CastingMeshIn)
		CastingMeshIn->SetVisibility(false);
	if (CastingMeshOut)
		CastingMeshOut->SetVisibility(false);

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation();

	TArray<AActor*> IgnoreActors;
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
	{
		IgnoreActors.Add(PlayerPawn);
	}

	// ?? ���� ���� ������ ����
	UGameplayStatics::ApplyRadialDamage(
		World,
		Damage,
		Origin,
		DamageRadius,
		UDamageType::StaticClass(),
		IgnoreActors, // ������ ���� ����
		this,
		GetInstigatorController(),
		true
	);

	// ?? ���� ���� ����Ʈ 10��
	if (ExplosionEffect)
	{
		for (int32 i = 0; i < 50; ++i)
		{
			float Delay = FMath::RandRange(0.1f, 0.5f);
			FTimerHandle TimerHandle;
			ExplosionTimers.Add(TimerHandle); // �ڵ� ����

			World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, World]()
				{
					if (!World || !ExplosionEffect) return;

					float Angle = FMath::RandRange(0.f, 360.f);
					float Radius = FMath::RandRange(0.f, DamageRadius * 0.8f);

					FVector SpawnPos = GetActorLocation() + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
						FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius,
						50.f); // ���� ����

					UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEffect, SpawnPos);

				}), Delay, false);
		}
	}

	SetLifeSpan(2.f);
}

