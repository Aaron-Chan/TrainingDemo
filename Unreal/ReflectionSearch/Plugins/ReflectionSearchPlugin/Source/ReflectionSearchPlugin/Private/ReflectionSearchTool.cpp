// Fill out your copyright notice in the Description page of Project Settings.


#include "ReflectionSearchTool.h"
#include "Widgets/Views/SListView.h"
#include "SlateOptMacros.h"


void ReflectionSearchTool::OnMouseClick(TSharedPtr<FReflectionObjectItem> ItemClicked)
{
	if (ItemClicked->IsObject)
	{
		if (ExpandNameArray.Contains(ItemClicked->PropertyName))
		{
			ExpandNameArray.Remove(ItemClicked->PropertyName);
		}
		else
		{
			ExpandNameArray.Add(ItemClicked->PropertyName);
		}
		OnSearch(SearchStr);
	}
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

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
								.OnMouseButtonClick(this, &ReflectionSearchTool::OnMouseClick)
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

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

ReflectionSearchTool::~ReflectionSearchTool()
{
}


void ReflectionSearchTool::UpdateView()
{
	OnSearch(SearchStr, 0);
}

void ReflectionSearchTool::OnSearchChanged(const FText& InFilterText)
{
	// SearchTextFilter->SetRawFilterText(InFilterText);
}

void ReflectionSearchTool::CollectItems(UClass* const Class, int32 Level)
{
	if (Level > 1)
	{
		return;
	}
	auto Prefix = Level ? FString("    ") : FString("");
	for (TFieldIterator<FProperty> It(Class); It; ++It)
	{
		FReflectionObjectItem Item;
		auto PropertyClass = It->GetClass();
		UObjectProperty* ObjectProperty = Cast<UObjectProperty>(*It);
		const auto PropertyName = It->GetName();
		FText ShowName;
		if (ObjectProperty)
		{
			ShowName = FText::FromString(
				FString::Format(TEXT("{0}{1}-{2}"), {
					                Prefix, It->GetName(), FString("Object")
				                }));
		}
		else
		{
			ShowName = FText::FromString(
				FString::Format(TEXT("{0}{1}"), {Prefix, It->GetName()}));
		}

		Item.Name = ShowName;
		Item.PropertyName = PropertyName;
		Item.IsObject = ObjectProperty && Level == 0 ? true : false;
		// if(PropertyClass->IsChildOf())

		FilteredItems.Add(MakeShared<FReflectionObjectItem>(Item));
		if (ExpandNameArray.Contains(PropertyName))
		{
			CollectItems(ObjectProperty->PropertyClass, Level + 1);
		}
	}
}

void ReflectionSearchTool::SetSearchStr(FString SearchString)
{
	this->SearchStr = SearchString;
	this->SearchBoxPtr->SetText(FText::FromString(SearchString));
}

void ReflectionSearchTool::OnSearch(const FString FilterStr, int32 Level)
{
	FilteredItems.Reset();
	UObject* ClassPackage = ANY_PACKAGE;
	FString AActorStr = FString("AActor");
	auto ss = AActor::StaticClass()->GetPathName();
	auto ss2 = AActor::StaticClass()->GetName();
	auto ss3 = AActor::StaticClass()->GetFullName();
	// FindObject<UClass>  需要传GetPathName   /Script/Engine.Actor
	// UClass* ActorRef = FindObject<UClass>(ANY_PACKAGE,*ss3);

	TArray<UObject*> result;
	GetObjectsOfClass(UClass::StaticClass(), result);
	for (auto Result : result)
	{
		if (UClass* const Class = Cast<UClass>(Result))
		{
			if (FilterStr.Equals(Class->GetName()))
			{
				for (TFieldIterator<FProperty> It(Class); It; ++It)
				{
					CollectItems(Class, Level);
				}
			}
		}
	}


	ListView->RequestListRefresh();
}


void ReflectionSearchTool::OnSearchCommitted(const FText& InFilterText, ETextCommit::Type InCommitType)
{
	// OnSearchChanged(InFilterText);

	ExpandNameArray.Reset();
	if (InCommitType == ETextCommit::Type::OnEnter)
	{
		//通过名称
		SearchStr = InFilterText.ToString();
		OnSearch(SearchStr);
	}
}

EVisibility ReflectionSearchTool::GetFailedSearchVisibility() const
{
	// !IsSearchActive() ||
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

bool ReflectionSearchTool::IsSearchActive() const
{
	return !SearchTextFilter->GetRawFilterText().IsEmpty();
}
