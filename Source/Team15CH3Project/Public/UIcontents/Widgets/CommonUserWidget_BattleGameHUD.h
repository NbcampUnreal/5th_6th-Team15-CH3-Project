#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Skill/PassiveItem.h"
#include "CommonUserWidget_BattleGameHUD.generated.h"


UCLASS()
class TEAM15CH3PROJECT_API UCommonUserWidget_BattleGameHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
    //Image
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_1;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_2;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_3;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_4;

    //Stack
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackCnt_4;

    UPROPERTY()
    TArray<EPassiveItemType> AssignedSkillTypes;

};