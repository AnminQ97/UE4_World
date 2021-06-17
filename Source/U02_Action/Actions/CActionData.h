#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionData.generated.h"


UCLASS()
class U02_ACTION_API UCActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	//Spawn을 위한 함수, ActionComponent Beginplay때 실행	     리턴된 결가를 넣어줄곳
	void BeginPlay(class ACharacter* InOwnerCharacter,class UCAction** OutAction);
	
private:
	//라벨
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	//spawn에 필요한 클래스
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACSkill> SKillClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FSkillData SkillQ;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FSkillData SkillW;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FSkillData SkillE;
	
};
