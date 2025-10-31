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

	// 루트
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(SceneRoot);
	CollisionComp->InitSphereRadius(0.1f);
	// ?? 고정된 장판 (기본 영역 표시)
	CastingMeshIn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	CastingMeshIn->SetupAttachment(CollisionComp);

	// ?? 점점 커지는 장판 (캐스팅 시 시각적 연출)
	CastingMeshOut = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CastingMesh"));
	CastingMeshOut->SetupAttachment(CollisionComp);

	// 기본값
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

	// GroundMesh는 이미 완전한 크기로 표시
	CastingMeshIn->SetRelativeScale3D(FVector(MaxScale, MaxScale, 0.001f));

	// CastingMesh는 작게 시작
	CastingMeshOut->SetRelativeScale3D(FVector(CurrentScale, CurrentScale, 0.001f));

}

void AShootingLaser::Tick(float DeltaTime)
{
	if (bIsLaserShot) return;

	// ?? 점점 커지는 장판 처리
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
			false,  // 지속시간
			0.f,    // LifeTime
			0,      // DepthPriority
			2.f     // Thickness
		);
	}

	// ?? 커지는 장판이 GroundMesh 크기와 같아지면 폭발
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

	// ?? 원형 범위 데미지 적용
	UGameplayStatics::ApplyRadialDamage(
		World,
		Damage,
		Origin,
		DamageRadius,
		UDamageType::StaticClass(),
		IgnoreActors, // 제외할 액터 없음
		this,
		GetInstigatorController(),
		true
	);

	// ?? 랜덤 폭발 이펙트 10개
	if (ExplosionEffect)
	{
		for (int32 i = 0; i < 50; ++i)
		{
			float Delay = FMath::RandRange(0.1f, 0.5f);
			FTimerHandle TimerHandle;
			ExplosionTimers.Add(TimerHandle); // 핸들 유지

			World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, World]()
				{
					if (!World || !ExplosionEffect) return;

					float Angle = FMath::RandRange(0.f, 360.f);
					float Radius = FMath::RandRange(0.f, DamageRadius * 0.8f);

					FVector SpawnPos = GetActorLocation() + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
						FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius,
						50.f); // 높이 고정

					UGameplayStatics::SpawnEmitterAtLocation(World, ExplosionEffect, SpawnPos);

				}), Delay, false);
		}
	}

	SetLifeSpan(2.f);
}

