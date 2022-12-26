#include <boost/ut.hpp>
#include <pqrs/cf/cf_ptr.hpp>

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "cf_ptr"_test = [] {
    auto cf1 = CFDictionaryCreate(kCFAllocatorDefault, nullptr, nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    auto cf2 = CFDictionaryCreate(kCFAllocatorDefault, nullptr, nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    expect(CFGetRetainCount(cf1) == 1_l);
    expect(CFGetRetainCount(cf2) == 1_l);

    {
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);
      expect(ptr1 == true);

      expect(CFGetRetainCount(ptr1.get()) == 2_l);
      expect(CFGetRetainCount(*ptr1) == 2_l);

      ptr1.reset();
      expect(CFGetRetainCount(cf1) == 1_l);
      expect(ptr1 == false);
    }

    expect(CFGetRetainCount(cf1) == 1_l);

    {
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
    }

    {
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
      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1(cf1);
      expect(CFGetRetainCount(cf1) == 2_l);

      pqrs::cf::cf_ptr<CFDictionaryRef> ptr1_1(ptr1);
      expect(CFGetRetainCount(cf1) == 3_l);

      ptr1 = nullptr;
      expect(CFGetRetainCount(cf1) == 2_l);

      ptr1_1 = nullptr;
      expect(CFGetRetainCount(cf1) == 1_l);
    }

    CFRelease(cf1);
    CFRelease(cf2);
  };

  return 0;
}
