#include "../Other/TestPrefix.h"


TEST_CASE("Observer",
          "[Observer]")
{
    IT("Emits items pushed to onNext")
    {
        auto o = Observable<var>::create([](Observer<var> observer) {
            observer.onNext(3);
            observer.onNext("Hello");
        });

        Array<var> items;
        varxCollectItems(o, items);

        varxRequireItems(items, var(3), var("Hello"));
    }

    IT("emits an error when calling onError")
    {
        auto o = Observable<int>::create([](Observer<int> observer) {
            observer.onError(std::exception_ptr());
        });

        bool onErrorCalled = false;
        o.subscribe([](int) {}, [&](std::exception_ptr) { onErrorCalled = true; });
        REQUIRE(onErrorCalled);
    }

    IT("notifies onCompleted")
    {
        auto o = Observable<float>::create([](Observer<float> observer) {
            observer.onCompleted();
        });

        bool completed = false;
        o.subscribe([](float) {}, [](std::exception_ptr) {}, [&]() { completed = true; });

        REQUIRE(completed);
    }

    IT("can subscribe to an Observable")
    {
        DisposeBag disposeBag;

        // Create subject
        BehaviorSubject<var> subject("Initial Value");
        CHECK(subject.getLatestItem() == "Initial Value");

        // Collect items from subject
        Array<var> items;
        varxCollectItems(subject, items);

        // Subscribe observer to some observable
        Observer<var> observer = subject;
        Observable<var>::from({ 3, 4, 5 }).subscribe(observer).disposedBy(disposeBag);

        // Subject should received items from Observable
        varxRequireItems(items, var("Initial Value"), var(3), var(4), var(5));
        REQUIRE(subject.getLatestItem() == var(5));
    }
}
