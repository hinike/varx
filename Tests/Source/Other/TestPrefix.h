/*
  ==============================================================================

    TestPrefix.h
    Created: 27 Apr 2017 7:55:11am
    Author:  Martin Finke

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "catch.hpp"

using namespace juce;
using namespace varx;

#define CONTEXT( desc )     SECTION( std::string(" Context: ") + desc, "" )
#define IT( desc )			SECTION( std::string("       It ") + desc, "" )

/** Subscribes to an Observable and collects all emitted items into a given Array. */
#define varxCollectItems(__observable, __arrayName) DisposeBag JUCE_JOIN_MACRO(__arrayName, JUCE_JOIN_MACRO(Disposable_, __LINE__)); (__observable).subscribe([&__arrayName](var v){ __arrayName.add(v); }).disposedBy(JUCE_JOIN_MACRO(__arrayName, JUCE_JOIN_MACRO(Disposable_, __LINE__)));

/** REQUIREs that a given Array is equal to the list of passed items. */
#define varxRequireItems(__arrayName, ...) REQUIRE(__arrayName == Array<var>({__VA_ARGS__}))

/** CHECKs that a given Array is equal to the list of passed items. */
#define varxCheckItems(__arrayName, ...) CHECK(__arrayName == Array<var>({__VA_ARGS__}))


/** Runs the JUCE dispatch loop for a given time, to process async callbacks. */
inline void varxRunDispatchLoop(int millisecondsToRunFor = 0)
{
	MessageManager::getInstance()->runDispatchLoopUntil(millisecondsToRunFor);
}

class TestWindow : public DocumentWindow, private DeletedAtShutdown
{
public:
	static TestWindow& getInstance()
	{
		static TestWindow *window = new TestWindow();
		return *window;
	}
	
	void addAndMakeVisible(Component& component)
	{
		getContentComponent()->addAndMakeVisible(component);
	}
	
private:
	TestWindow()
	: DocumentWindow("varx-Tests", Colours::white, DocumentWindow::TitleBarButtons::closeButton, true)
	{
		ScopedPointer<Component> component(new Component());
		component->setSize(1, 1);
		setContentOwned(component.release(), true);
		setVisible(true);
	}
};
