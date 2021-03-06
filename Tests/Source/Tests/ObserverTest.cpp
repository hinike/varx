/*
  ==============================================================================

    ObserverTest.cpp
    Created: 2 May 2017 9:45:03pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "TestPrefix.h"


TEST_CASE("Observer",
		  "[Observer]")
{
	IT("Emits items pushed to onNext") {
		auto o = Observable::create([](Observer observer) {
			observer.onNext(3);
			observer.onNext("Hello");
		});
		
		Array<var> items;
		varxCollectItems(o, items);
		
		varxRequireItems(items, var(3), var("Hello"));
	}
	
	IT("emits an error when calling onError") {
		auto o = Observable::create([](Observer observer) {
			observer.onError(Error());
		});
		
		bool onErrorCalled = false;
		o.subscribe([](var){}, [&](Error) { onErrorCalled = true; });
		REQUIRE(onErrorCalled);
	}
	
	IT("notifies onCompleted") {
		auto o = Observable::create([](Observer observer) {
			observer.onCompleted();
		});
		
		bool completed = false;
		o.subscribe([](var){}, [](Error){}, [&](){ completed = true; });
		
		REQUIRE(completed);
	}
	
	IT("can subscribe to an Observable") {
		DisposeBag disposeBag;
		
		// Create subject
		BehaviorSubject subject("Initial Value");
		CHECK(subject.getLatestItem() == "Initial Value");
		
		// Collect items from subject
		Array<var> items;
		varxCollectItems(subject, items);
		
		// Subscribe observer to some observable
		Observer observer = subject.asObserver();
		Observable::from({3, 4, 5}).subscribe(observer).disposedBy(disposeBag);
		
		// Subject should received items from Observable
		varxRequireItems(items, var("Initial Value"), var(3), var(4), var(5));
		REQUIRE(subject.getLatestItem() == var(5));
	}
}
