// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Warp.generated.h"

/**
 * 
 */
UCLASS()
class U02_ACTION_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Tick(float DeltaTime) override;

private:
	//�÷��̾� Warp���� Ȯ��
	class ACPlayer* UserCursorLocation(); 

	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator);

private:
	//��Į
	class UDecalComponent* Decal; 
	
	//���콺��ġ
	FVector Location;
};
