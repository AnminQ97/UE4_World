#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItem.generated.h"

UCLASS()
class U02_ACTION_API ACItem : public AActor
{
	GENERATED_BODY()
	
public:

	ACItem();



	//�������� �ִ� ����
	UPROPERTY(Transient)
		class UWorld* World;

	//������ ��� �ؽ�Ʈ (����, �Ա� ��)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText UseActionText;
	
	//������ �����
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item")
		class UTexture2D* Thumbnail;

	//�κ��丮�� ���� ������ �̸�
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText ItemDisplayName;

	//������ ����
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText ItemDescription;

	//����
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		float Weight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		int32 count = 0;

	//�κ��丮 (�� �������� ������)
	UPROPERTY()
		class UCInventoryComponent* OwningInventroy;

	//�����ۿ� ���� ���(action)
	virtual void Use(class ACPlayer* Character) {};

	//�������� ���� �����ϰ�
	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class ACPlayer* Character);

protected:
	virtual void BeginPlay() override;

private:
	//�浹ü
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

};
