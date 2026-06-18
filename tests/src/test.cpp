#include <boost/ut.hpp>
#include <pqrs/cf/cf_ptr.hpp>
#include <utility>

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "cf_ptr"_test = [] {
    // Use CFDictionary because its retain count is stable enough for ownership tests.
    auto cf1 = CFDictionaryCreate(kCFAllocatorDefault, nullptr, nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    auto cf2 = CFDictionaryCreate(kCFAllocatorDefault, nullptr, nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    expect(CFGetRetainCount(cf1) == 1_l);
    expect(CFGetRetainCount(cf2) == 1_l);

    {
      // Construction retains the CF object, and reset releases it.
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);
      expect(ptr1 == true);

      // Accessors expose the retained raw reference.
      expect(CFGetRetainCount(ptr1.get()) == 2_l);
      expect(CFGetRetainCount(*ptr1) == 2_l);

      ptr1.reset();
      expect(CFGetRetainCount(cf1) == 1_l);
      expect(ptr1 == false);
    }

    expect(CFGetRetainCount(cf1) == 1_l);

    {
      // adopt takes ownership of an already-retained Create/Copy result without retaining it again.
      auto cf3 = CFDictionaryCreate(kCFAllocatorDefault, nullptr, nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
      expect(CFGetRetainCount(cf3) == 1_l);

      auto ptr1 = pqrs::cf::adopt_cf_ptr(cf3);
      expect(ptr1 == true);
      expect(CFGetRetainCount(cf3) == 1_l);

      ptr1.reset();
    }

    {
      // Assigning nullptr releases the currently retained object.
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
    }

    {
      // Copy assignment retains the new object before releasing the old one.
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr2(cf2);
      expect(CFGetRetainCount(cf2) == 2_l);

      ptr1 = ptr2;
      expect(CFGetRetainCount(cf1) == 1_l);
      expect(CFGetRetainCount(cf2) == 3_l);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
      expect(CFGetRetainCount(cf2) == 2_l);

      ptr2 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
      expect(CFGetRetainCount(cf2) == 1_l);
    }

    {
      // Copy construction creates an independent retaining owner.
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);

      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1_1(ptr1);
      expect(CFGetRetainCount(cf1) == 3_l);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cf1) == 2_l);

      ptr1_1 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
    }

    {
      // Move construction and move assignment transfer ownership without changing the retain count.
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);

      pqrs::cf::cf_ptr<CFDictionaryRef> ptr2(std::move(ptr1));
      expect(ptr1 == false);
      expect(ptr2 == true);
      expect(CFGetRetainCount(cf1) == 2_l);

      pqrs::cf::cf_ptr<CFDictionaryRef> ptr3;
      ptr3 = std::move(ptr2);
      expect(ptr2 == false);
      expect(ptr3 == true);
      expect(CFGetRetainCount(cf1) == 2_l);

      ptr3 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
    }

    {
      // Self copy-assignment must keep the object alive and preserve the retain count.
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);

      const auto& same_ptr = ptr1;
      ptr1 = same_ptr;
      expect(ptr1 == true);
      expect(CFGetRetainCount(cf1) == 2_l);
    }

    CFRelease(cf1);
    CFRelease(cf2);
  };

  return 0;
}
