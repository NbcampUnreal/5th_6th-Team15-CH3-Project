#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Skill/PassiveItem.h"
#include "Skill/ActiveSkillItem.h"
#include "Blueprint/UserWidget.h"

// HUD 위젯에서 사용되는 컴포넌트
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

// PlayerCharacter, StatsComponent
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"

#include "CommonUserWidget_BattleGameHUD.generated.h"


UCLASS()
class TEAM15CH3PROJECT_API UCommonUserWidget_BattleGameHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

    //==Passive==
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
    class UTextBlock* PassiveStackCnt_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PassiveStackCnt_4;

    UPROPERTY()
    EPassiveItemType SkillSlot_1_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_2_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_3_Type = EPassiveItemType::None;

    UPROPERTY()
    EPassiveItemType SkillSlot_4_Type = EPassiveItemType::None;

    UPROPERTY()
    TArray<EPassiveItemType> AssignedSkillTypesPassive;

    //==Active==
    //Image
    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_5;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_6;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_7;

    UPROPERTY(meta = (BindWidget))
    class UImage* Image_Skill_8;

    //Stack
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_2;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_3;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveStackCnt_4;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_1_Active = EActiveSkillItemType::None;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_2_Active = EActiveSkillItemType::None;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_3_Active = EActiveSkillItemType::None;

    UPROPERTY()
    EActiveSkillItemType SkillSlot_4_Active = EActiveSkillItemType::None;

    UPROPERTY()
    TArray<EActiveSkillItemType> AssignedSkillTypesActive;


    //HP & MP 바인드
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HPText;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* MPBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MPText;

    //플레이어 캐릭터 참조
    UPROPERTY()
    APlayerCharacter* MyPlayerChar;

    //NativeTick 매프레임 업데이트 할 때 사용
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    //초기화
    UFUNCTION(BlueprintCallable)
    void InitHUD();
    

    //타이머
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentTime;

    UPROPERTY(VisibleAnywhere, Category = "Game Time")
    float GameTimeElapsed = 0.0f;











private:

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> DefeatWidgetClass;

    bool bIsDefeatUIShown = false;

    TArray<FString> DefeatQuotes = {
        TEXT("The stars say nothing; they merely shine."),
        TEXT("The universe is vast, but meaning is always made by humans."),
        TEXT("A signal came in the silence, and then nothing happened."),
        TEXT("Life is not a calculation, but the end was always precise."),
        TEXT("An old satellite is still repeatedly transmitting a 'return home' signal."),
        TEXT("We found God, but it was just an ancient algorithm."),
        TEXT("The Earth is gone, and we still call out names."),
        TEXT("Even in a vacuum, memory remains. It is the only evidence of life."),
        TEXT("Time is not a straight line; only we believe it to be."),
        TEXT("The last human looked at the stars and powered down.")
    };

};