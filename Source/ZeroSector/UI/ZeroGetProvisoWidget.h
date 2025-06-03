#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ZeroProvisoDataTable.h" 
#include "ZeroGetProvisoWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnProvisoConfirmed, const FZeroProvisoDataTable&);
DECLARE_MULTICAST_DELEGATE(FOnProvisoRejected);

UCLASS()
class ZEROSECTOR_API UZeroGetProvisoWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UFUNCTION(Category = "UI")
	void ShowWidget();

	// �ؽ�Ʈ/�̹��� ���� �Լ�
	void SetProvisoInfo(const FString& ProvisoName, const FString& ProvisoDescription);
	UFUNCTION(BlueprintCallable)
	void SetProvisoImage(UTexture2D* InImage);
	void SetCurrentProvisoData(const FZeroProvisoDataTable& InData);

	// ��������Ʈ
	FOnProvisoConfirmed OnProvisoConfirmed;
	FOnProvisoRejected OnProvisoRejected;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProvisoNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProvisoDescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ProvisoImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WriteButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ThrowButton;

	FTimerHandle WidgetTimerHandle;

	// ���ο� ����� �ܼ� ������
	FZeroProvisoDataTable CurrentProvisoData;

	// ��ư Ŭ�� ó�� �Լ�
	UFUNCTION()
	void OnWriteClicked();

	UFUNCTION()
	void OnThrowClicked();
};
