// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/TextFilter.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSearchBox.h"

struct FReflectionObjectItem
{
	FText Name;
	bool IsObject;
	FString PropertyName;
};

/**
 *  copy from SPlacementModeTools  PlaceActors的工具栏
 */
class REFLECTIONSEARCHPLUGIN_API ReflectionSearchTool : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(ReflectionSearchTool)
		{
		}

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	virtual ~ReflectionSearchTool();

	void SetSearchStr(FString SearchString);

	void UpdateView();

private:
	/** Called when the search text changes */
	void OnSearchChanged(const FText& InFilterText);
	void CollectItems(UClass* Class, int32 Level);
	void OnSearch(FString SearchStr, int32 Level = 0);
	void OnSearchCommitted(const FText& InFilterText, ETextCommit::Type InCommitType);
	EVisibility GetFailedSearchVisibility() const;

	TSharedRef<ITableRow> OnGenerateWidgetForItem(TSharedPtr<FReflectionObjectItem> InItem,
	                                              const TSharedRef<STableViewBase>& OwnerTable) const;
	void OnMouseClick(TSharedPtr<FReflectionObjectItem> ItemClicked);
	/** Check if a search is active */
	bool IsSearchActive() const;

private:
	TSharedPtr<SBox> DataDrivenContent;
	TSharedPtr<SListView<TSharedPtr<FReflectionObjectItem>>> ListView;
	/** Array of filtered items to show in the list view */
	TArray<TSharedPtr<FReflectionObjectItem>> FilteredItems;

	typedef TTextFilter<const FReflectionObjectItem&> FReflectionObjectItemTextFilter;
	TSharedPtr<FReflectionObjectItemTextFilter> SearchTextFilter;
	TSharedPtr<SSearchBox> SearchBoxPtr;
	TArray<FString> ExpandNameArray;
	FString SearchStr;
};
