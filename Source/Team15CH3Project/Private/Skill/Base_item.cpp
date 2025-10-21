#include "Skill/Base_item.h"
#include "Components/SphereComponent.h"

ABase_item::ABase_item()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
}

void ABase_item::OnItemOverlap(AActor* OverlapActor)
{

}
void ABase_item::OnItemEndOverlap(AActor* OverlapActor)
{

}
void ABase_item::ActivateItem(AActor* Acivator)
{

}
FName ABase_item::GetIemType() const
{
	return ItemType;
}

void ABase_item::DestroyItem()
{
	Destroy();
}


