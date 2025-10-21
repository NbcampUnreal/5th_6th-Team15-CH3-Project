#pragma once

#include "CoreMinimal.h"
#include "Skill/Base_item.h"
#include "passiveItem.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API ApassiveItem : public ABase_item
{
	GENERATED_BODY()
public:
	ApassiveItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 StackCnt;
};
