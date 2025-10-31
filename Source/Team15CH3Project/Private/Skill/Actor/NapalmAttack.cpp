#include "Skill/Actor/NapalmAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Math/UnrealMathUtility.h"
#include "CollisionQueryParams.h" 
#include "CollisionShape.h"

ANapalmAttack::ANapalmAttack()
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
	GrowSpeed = 1.0f;
	MaxXScale = 5.0f;
	BaseYScale = 1.0f;
	DamageRadius = 500.f;
	Damage = 80.f;
	bExploded = false;

	CurrentXScale = 0.1f;
}

void ANapalmAttack::BeginPlay()
{
	Super::BeginPlay();

	// GroundMesh�� �̹� ������ ũ��� ǥ��
	CastingMeshIn->SetRelativeScale3D(FVector(MaxXScale, BaseYScale, 0.001f));

	// CastingMesh�� �۰� ����
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentXScale, BaseYScale, 0.001f));
	
}

void ANapalmAttack::Tick(float DeltaTime)
{
	if (bExploded) return;

	// ?? ���� Ŀ���� ���� ó��
	CurrentXScale += GrowSpeed * DeltaTime;
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentXScale, BaseYScale, 0.001f));

	// ?? Ŀ���� ������ GroundMesh ũ��� �������� ����
	if (CurrentXScale >= MaxXScale)
	{
		Explode();
	}
}

void ANapalmAttack::Explode()
{
	bExploded = true;

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
		for (int32 i = 0; i < 20; ++i)
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

