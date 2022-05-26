// Fill out your copyright notice in the Description page of Project Settings.


#include "ReflectionSearchTool.h"
#include "Widgets/Views/SListView.h"


void ReflectionSearchTool::Construct(const FArguments& InArgs)
{

	// SearchTextFilter = MakeShareable(new FReflectionObjectItemTextFilter(
	// 	FReflectionObjectItemTextFilter::FItemToStringArray::CreateStatic(&PlacementViewFilter::GetBasicStrings)
	// 	));
	
	TSharedRef<SScrollBar> ScrollBar = SNew(SScrollBar)
		.Thickness(FVector2D(9.0f, 9.0f));

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		  .Padding(4)
		  .AutoHeight()
		[
			SAssignNew(SearchBoxPtr, SSearchBox)
			.HintText(NSLOCTEXT("PlacementMode", "SearchPlaceables", "Search Classes"))
			.OnTextChanged(this, &ReflectionSearchTool::OnSearchChanged)
			.OnTextCommitted(this, &ReflectionSearchTool::OnSearchCommitted)
		]

		+ SVerticalBox::Slot()
		.Padding(0)
		[
			SNew(SHorizontalBox)


			+ SHorizontalBox::Slot()
			[
				SNew(SBorder)
				.Padding(FMargin(3))
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
				[
					SNew(SOverlay)

					+ SOverlay::Slot()
					  .HAlign(HAlign_Center)
					  .VAlign(VAlign_Fill)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("PlacementMode", "NoResultsFound", "No Results Found"))
						.Visibility(this, &ReflectionSearchTool::GetFailedSearchVisibility)
					]


					+ SOverlay::Slot()
					[
						SAssignNew(DataDrivenContent, SBox)
						[
							SNew(SHorizontalBox)

							+ SHorizontalBox::Slot()
							[
								SAssignNew(ListView, SListView<TSharedPtr<FReflectionObjectItem>>)
								.ListItemsSource(&FilteredItems)
								.OnGenerateRow(this, &ReflectionSearchTool::OnGenerateWidgetForItem)
								.ExternalScrollbar(ScrollBar)
							]

							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ScrollBar
							]
						]
					]
				]
			]
		]
	];
}

ReflectionSearchTool::~ReflectionSearchTool()
{
}

void ReflectionSearchTool::OnSearchChanged(const FText& InFilterText)
{
	
}

void ReflectionSearchTool::OnSearchCommitted(const FText& InFilterText, ETextCommit::Type InCommitType)
{
	OnSearchChanged(InFilterText);
}

EVisibility ReflectionSearchTool::GetFailedSearchVisibility() const
{
	if (FilteredItems.Num())
	{
		return EVisibility::Collapsed;
	}
	return EVisibility::Visible;
}

TSharedRef<ITableRow> ReflectionSearchTool::OnGenerateWidgetForItem(TSharedPtr<FReflectionObjectItem> InItem,
                                                                    const TSharedRef<STableViewBase>& OwnerTable) const
{
	return SNew(STableRow<TSharedPtr<FReflectionObjectItem>>, OwnerTable)
	[
		// SNew(SPlacementAssetEntry, InItem.ToSharedRef())
		// .HighlightText(this, &SPlacementModeTools::GetHighlightText)
		SNew(STextBlock)
		.Text(InItem->Name)
	];
}
